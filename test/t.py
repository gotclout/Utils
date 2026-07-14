from decimal import Decimal
import psycopg2
import logging
from querybuilder.query import Query
from django.db.models import Q
from gc_common import GcGlobalCommon
import sqlalchemy


class GcQuery (Query):
  """
  The Django querybuilder did not implement builders for insert, update and delete.
  this class extends querybuilder.query.Query and implements the methods.
  """

  def create_delete_sql(self, table=None, pk_field = None, pk_value=None,
                        **kwargs):
    """
    Creates delete SQL
    """
    #TODO if pk_value instanceof() conversion
    if kwargs and False:
      print('')

    sql = 'DELETE FROM ' + table + ' WHERE ' + pk_field + ' = ' + "'" + \
          str(pk_value) + "'"
    self.gc_sql = sql
    self.gc_args = None

    return self

  def create_update_sql(self, table=None, field_names=None, values=None,
                        pk_field = None, pk_value=None, **kwargs):
    """
    Creates update SQL
    """
    if kwargs and False:
      print('')

    sql = 'UPDATE ' + table
    field_str = ''
    if field_names and len(field_names) > 0 :
      for field in field_names :
        if len(field_str) > 0 :
          field_str += ','
        field_str += field + '=' + '%s'

    sql += ' SET ' + field_str + ' WHERE ' + pk_field + " = '" + str(pk_value) + "';"

    self.gc_sql = sql
    self.gc_args = values

    return self

  def create_insert_sql(self, table=None, field_names=None, values=None,
                        **kwargs):
    """
    Creates insert SQL
    """
    if kwargs and False:
      print('')

    sql = 'INSERT INTO ' + table
    field_str = ''
    if field_names and len(field_names) > 0 :
      for field in field_names :
        if len(field_str) > 0 :
          field_str += ','
        field_str += field

    value_str = ''
    if values and len(values) > 0 :
      counter = 0
      while counter < len(values) :
        if len(value_str) > 0 :
          value_str += ','
        value_str += '%s'
        counter += 1

    sql += ' (' + field_str + ') VALUES (' + value_str + ')'

    self.gc_sql = sql
    self.gc_args = values

    return self

  def get_sql(self, debug=False, use_cache=True):
    """
    Overrides the default get_sql methods so it return sql for insert,
    update and delete.
    """
    if self.gc_sql and len (self.gc_sql) > 0:
      return self.gc_sql
    else :
      return Query.get_sql(self, debug=debug, use_cache=use_cache)

  def get_args(self):
    """
    Overrides the default get_args methods so it return args for insert,
    update and delete.
    """
    if self.gc_args and len (self.gc_args) > 0:
      return self.gc_args
    else :
      return Query.get_args(self)

class PostgresAccessor:
  """
  Generic database accessor class for Postgres
  """

  def __init__(self, config, log=None):
    """
    Initializes the class
    """
    self.lmcommon     = GcGlobalCommon
    self._logger      = self.lmcommon.get_global_logger() if not log else log
    self._host        = config['host']
    self._port        = config['port']
    self._credentials = config['credentials']
    self._user   = config['user']
    self._dbname = config['database']
    self._connection_string = "dbname='" + self._dbname + "' user='" + \
                              self._user + "' host='" + self._host + \
                              "' password='" + self._credentials + \
                              "' port='" + str(self._port) + "'"
    self._logger.info("Connection String: " + self._connection_string)
    # dialect+driver://username:password@host:port/database
    self._engine = sqlalchemy.create_engine(
      'postgresql+psycopg2://{user}:{password}@{host}:{port}/{db}'.format(
              user=self._user, password=self._credentials,
              host=self._host, db=self._dbname, port=self._port
      )
    )

  @staticmethod
  def create_django_insert_builder (table_name, field_names, values) :
    """
    Creates DJango Query class for insert statements
    """
    return GcQuery().create_insert_sql(table_name, field_names, values)

  @staticmethod
  def create_django_update_builder (table_name, field_names, values,
                                    pk_field, pk_value) :
    """
    Creates DJango Query class for update statements
    """
    return GcQuery().create_update_sql(table_name, field_names, values,
                                        pk_field, pk_value)

  @staticmethod
  def create_django_delete_builder (table_name, pk_field, pk_value) :
    """
    Creates DJango Query class for delete statements
    """
    return GcQuery().create_delete_sql(table_name, pk_field, pk_value)

  @staticmethod
  def create_django_query_builder (table_name, return_columns=None,
                                   and_conditions=None, or_conditions=None,
                                   order_by_condition=None):
    """
    Creates DJango Query class for select statements
    """
    if return_columns and len(return_columns) > 0 :
      query = Query().from_table(table_name, return_columns)
    else :
      query = Query().from_table(table_name)

    if and_conditions and len(and_conditions) > 0:
      for cond in and_conditions:
        query.where(**cond)

    # TODO: Test
    '''
    if or_conditions and len(or_conditions) > 0:
      for cond in or_conditions:
        query.where(**cond, where_type='OR')
    '''
    if or_conditions and len(or_conditions) > 0:
      for or_cond in or_conditions:
        for key, value in or_cond.items():
          # criteria
          q = Q(**{key: value})
          # include q into query statement
          query._where.wheres.children.extend(q.children)
          # add connector
          query._where.wheres.add(q, 'OR')

    if order_by_condition and len(order_by_condition) > 0:
      query.order_by(order_by_condition,table_name,False)

    return query

  def _execute_django_dml (self, query, raise_exception=False):
    """
    Executes a DJango Query class for insert, update or delete
    """
    self._execute_dml(query.get_sql(self), query.get_args(), raise_exception)

  def _execute_django_query (self, query):
    """
    Executes a DJango Query class for selects
    """
    return self._execute_query(query.get_sql(), query.get_args())

  def _execute_dml (self, query, args, raise_exception=False):
    """
    Executes a an insert, update or delete sql statement
    """
    conn = None
    cur = None
    try :
      conn = psycopg2.connect(self._connection_string)
      cur  = conn.cursor()
      cur.process_job(query, args)
      conn.commit()
    except Exception as e:
      if raise_exception :
        raise e
      else :
        logging.info('Error: Encountered exception ' + str(e))
    finally :
      if cur :
        cur.close()
      if conn :
        conn.close()

  def _execute_query (self, query, args):
    """
    Executes a an insert, update or delete sql statement
    """
    response = None
    conn = None
    cur = None
    try :
      logging.debug('Executing query: ' + query + '\n args: ' + str(args))
      conn = psycopg2.connect(self._connection_string)
      cur = conn.cursor()
      cur.process_job(query, args)
      column_names = [desc[0] for desc in cur.description]
      query_data = cur.fetchall()
      if query_data and len(query_data) > 0 :
        response = []
        for row in query_data :
          row_dict = {}
          counter = 0
          for column in column_names :
            value = row[counter]
            #Decimal type will not convert to JSON
            if isinstance(value, Decimal) :
              value = float(value)
            row_dict[column] = value
            counter += 1
          response.append(row_dict)
    except Exception as e:
      logging.info('Error: Encountered exception ' + str(e))
    finally :
      if cur :
        cur.close()
      if conn :
        conn.close()

    return response

  def _make_large_object(self, ldata):
    """
    Creates a large object storing the provided data

    :param ldata: The data to be stored in the blob
    :return: The large object id created or None if error
    """
    loid = None

    try:
      conn = psycopg2.connect(self._connection_string)
      lobj = conn.lobject(0, 'w', 0)
      data = str.encode(ldata)
      len_value  = lobj.write(data)

      logging.debug('largeobject bytes written: ' + str(len_value))

      conn.commit()
      conn.close()

      loid = lobj.oid
    except Exception as e:
      logging.error('Error: Encountered exception ' + str(e))

    return loid

  def _del_large_object(self, loid):
    """
    Removes the specified large object from the DB

    :param loid: The id of the large object to be removed
    :return: True if success, False otherwise
    """
    rval = True

    try:
      conn = psycopg2.connect(self._connection_string)
      lobj = conn.lobject(loid, 'r', 0)

      lobj.unlink()
      logging.debug('largeobject ' + str(loid) + ' removed')

      conn.commit()
      conn.close()
    except Exception as e:
      logging.error('Encountered exception: ' + str(e) +
                    '\nRetrieving blob id: ' + str(loid))
      rval = False

    return rval

  def _retrieve_large_object(self, loid):
    """
    Retrieves the data stored in the specified large object

    :param loid: The id of the large object to be retrieved
    :return: The data stored i the large object None if error
    """
    data = None

    try:
      conn = psycopg2.connect(self._connection_string)
      lobj = conn.lobject(loid, 'r', 0)

      lobj.seek(0)

      data = lobj.read()

      logging.debug('largeobject bytes read: ' +  str(len(data)))

      conn.commit()
      conn.close()
    except Exception as e:
      logging.error('Error: Encountered exception retrieving object id ' +
                    str(e))

    return data

  def _get_db_tables(self):
    """
    Retrieve a tuple list of all tables in current DB (tuple [0] = db name)

    :return: res is the result tuple list
    """
    conn   = psycopg2.connect(self._connection_string)
    cursor = conn.cursor()
    cursor.process_job("select relname from pg_class where relkind='r' and relname !~ '^(pg_|sql_)';")

    res = cursor.fetchall()

    return res

  def _get_table_fields(self, table):
    """
    Retrieve a list of all fields for the specified db table

    :param table:
    :return: fields is the list of all table fields
    """
    fields = []
    conn   = psycopg2.connect(self._connection_string)
    cursor = conn.cursor()
    try:
      cursor.process_job('select * from ' + table + ' limit 1')
      fields = [desc[0] for desc in cursor.description]
    except Exception as e:
      print(str(e))

    return fields

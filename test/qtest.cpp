#include <time.h>
#include <iostream>
#include <pthread.h>
#include "FifoQueue.h"

#define MAX_ENTRIES 5
#define usleepsec rand() % 500000 + 1

using namespace std;

const static unsigned short len = 16;
static unsigned long sleepsec;

class qobject{

  private:

  public:
    int i;
    long j;
    char* str;

    qobject(){str = NULL;};

    char* toString(){return str;};

    qobject(const char* const & pStr)
    {
      i = 0;
      j = 0;
      str = new char[len+1];
      memset(str, 0, len+1);
      pStr ? memcpy(str, pStr, len) : memcpy(str, "Uninitialized qobject", len);
    };

    ~qobject()
    {
      if(str)
        delete [] str;
    }
};

void* consumer(void* qPtr)
{
  queue<qobject*>* q = (queue<qobject*>*) qPtr;
  bool run = true;
  int i = 0, size = 0;

  while(i < MAX_ENTRIES)
  {
    sleepsec = usleepsec;
    cout << "consumer sleep: " << sleepsec << " usec" << endl;
    usleep(sleepsec);

    size = q->size();
    cout << "consumer qsize:   " << size << endl;
    qobject* o = q->dequeue();
    if(o != NULL)
    {
      cout << "consumer dequeue: " << (char*)o->toString() << endl;
      delete o;
      o = NULL;
      i++;
    }
  }

  cout << "consumer exiting..." << endl;
  return (void*) NULL;
}

void* producer(void* qPtr)
{
  queue<qobject*>* q = (queue<qobject*>*) qPtr;
  int i = 0;

  while(i < MAX_ENTRIES)
  {
    cout << "producer enqueue: " << ++i << endl;
    char name [len];
    sprintf(name, "QObject %i", i);
    qobject* o = new qobject((const char*const)name);
    q->enqueue(o);
    sleepsec = usleepsec;
    cout << "producer sleep: " << sleepsec << " usec" << endl;
    usleep(sleepsec);
  }

  cout << "producer exiting..." << endl;
  return (void*) NULL;
}

void qtest()
{
  queue<qobject*> q;

  pthread_t readThread, writeThread;

  cout << "qtest create producer sleep 1s..." << endl;
  if(pthread_create(&writeThread, 0, producer, (void*)&q) == 0)
  {
    sleep(1);
    cout << "qtest create consumer sleep 2s..." << endl;
    if(pthread_create(&readThread, 0, consumer, (void*)&q) == 0)
    {
      sleep(2);
      pthread_join(readThread, 0);
    }

    pthread_join(writeThread, 0);
  }

  int size = q.size();
  cout << "qtest qsize:" << size << endl << "qtest exiting..." << endl;
}

int main(int argc, char* argv[])
{
  srand(time(NULL));
  qtest();
  return 0;
}

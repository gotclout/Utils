#ifndef _FIFOQUEUE_
#define _FIFOQUEUE_

#include "Mutex.h"

using namespace std;

/**
 * Template linked list node object
 */
template <class Object>
class qnode
{

  public:

    /** Stored data **/
    Object data;

    /** Next element **/
    qnode* next;

    /**
     * Default Constructor
     */
    qnode(const Object & o) { next = NULL; data = o; };
    
    /**
     * TODO:
     */
     //friend ostream& operator << (ostream & o, qnode & n)
};

/**
 * Template linked list FIFO queue implementation
 */
template <class Object>
class queue
{

  private:

    /** The number of nodes **/
    int qsize;

    /** First/Front node **/
    class qnode<Object>* head;

    /** End/Tail node **/
    class qnode<Object>* tail;

    /** Tracking node **/
    class qnode<Object>* current;

    /** Mutex for thread safe operations **/
    Mutex mutex;

  public:

    /**
     * Default Constructor
     */
    queue()
    {
      qsize = 0; head = tail = current = NULL;
    };

    /**
     * Adds an element to the queue
     */
    void enqueue(const Object & o)
    {
      mutex.lock();
      current = new qnode<Object>(o);
      ++qsize;
      if(!head)
      {
        tail = head = current;
        head->next = NULL;
      }
      else
      {
        if(head == tail)
          head->next = tail;
        tail->next = current;
        current->next = NULL;
        tail = current;
      }
      mutex.unlock();
    };

    /**
     * Removes an element from the queue
     */
    Object dequeue()
    {
      Object o = NULL;
      mutex.lock();
      if(head)
      {
        --qsize;
        o = head->data;
        current = head->next;
        delete head;
        if(!current)
          head = tail = current = NULL;
        else
          head = current;
      }
      mutex.unlock();
      return o;
    };

    /**
     * Retrieves the head node of the queue
     */
    qnode<Object>* front() { return head; };

    /**
     * Retrieves the size of the queue
     */
    const int& size() const {return qsize;};

    /**
     * Indicates the whether or not the queue is empty
     */
    bool empty() {return qsize == 0;};

    /**
     * Destructor
     */
    ~queue()
    {
      while(head)
      {
        current = head;
        head = head->next;
        delete current;
        current = NULL;
      }
    };
    
    /**
     *
     * TODO:
     */
     //friend ostream& operator << (ostream & o, queue & q)
};
#endif//_FIFOQUEUE_

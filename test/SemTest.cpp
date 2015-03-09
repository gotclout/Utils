#include "Semaphore.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <time.h>

#define MAX_ENTRIES 5
#define usleepsec rand() % 500000 + 1

Semaphore rsem;
Semaphore wsem("/namedsem");

using namespace std;

static unsigned long sleepsec;

/**
 * Consume Elems
 */
void* consumer(void* qPtr)
{
  vector<int>* q = (vector<int>*) qPtr;
  bool run = true;
  int i = 0, size = 0;

  while(i < MAX_ENTRIES)
  {
    sleepsec = usleepsec;
    cout << "consumer sleep: " << sleepsec << " usec" << endl;
    wsem.wait();
    usleep(sleepsec);
    rsem.post();
    size = q->size();
    cout << "consumer qsize:   " << size << endl;
    if(q->size())
    {
      int o = q->front();
      q->erase(q->begin());
      cout << "consumer dequeue: " << o << endl;
    }
    i++;
  }

  cout << "consumer exiting..." << endl;
  return (void*) NULL;
}

/**
 * Produce elems
 */
void* producer(void* qPtr)
{
  vector<int>* q = (vector<int>*) qPtr;
  int i = 0;

  while(i < MAX_ENTRIES)
  {
    rsem.wait();
    wsem.post();
    cout << "\nproducer enqueue: " << ++i << endl;
    q->push_back(i);
    sleepsec = usleepsec;
    cout << "producer sleep: " << sleepsec << " usec" << endl;
    usleep(sleepsec);
  }

  cout << "producer exiting..." << endl;
  return (void*) NULL;
}

/**
 * Timed wait and consume 1 elem
 */
void* consume(void* qPtr)
{
   vector<int>* q = (vector<int>*) qPtr;
   timespec ts;
   bool w = true;
   while(w)
   {
     clock_gettime(CLOCK_REALTIME, &ts);
     ts.tv_sec += 1;
     wsem.timedwait(&ts);
     rsem.wait();
     rsem.post();
     if(q->size())
     {
       int o = q->front();
       q->erase(q->begin());
       cout << "\nrsem value: " << rsem.value() << " tconsumer dequeue: " << o << endl;
     }
     w = false;
   }
   cout << "tconsumer exiting..." << endl;
}

void qtest()
{
  vector <int> q;

  pthread_t readThread, writeThread, consumeThread;

  cout << "qtest create producer sleep 1s..." << endl;
  if(pthread_create(&writeThread, 0, producer, (void*)&q) == 0)
  {
    sleep(1);
    cout << "qtest create consumer..." << endl;
    if(pthread_create(&readThread, 0, consumer, (void*)&q) == 0)
    {
      cout << "qtest create tconsumer..." << endl;
      if(pthread_create(&consumeThread, 0, consume, (void*)&q) == 0)
      {
        pthread_join(consumeThread, 0);
      }
      pthread_join(readThread, 0);
    }

    pthread_join(writeThread, 0);
  }

  int size = q.size();
  cout << "qtest qsize:" << size << endl << "qtest exiting..." << endl;
}

/**
 * Main: test the Semaphore class with 3 threads
 */
int main(int argc, char** argv)
{
  srand(time(NULL));
  qtest();
  return 0;
}

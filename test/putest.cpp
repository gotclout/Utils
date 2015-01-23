#include<iostream>
#include<string>

#include "Timer.h"
#include "Primes.h"

#define N 100000ULL  //Num Primes
#define C 1299710ULL //Start Prime Initalization Value

Primes* p = 0;

static long long elapsed = 0;

/**
 * Compute num primes np up to start value sp
 */
void compute(tulong sp = C, tulong np = N)
{
  Timer ct; //Computation timer
  ct.start();//Start Computation
  p = new Primes(sp);
  ct.stop();

  cout << "Time to Compute "    << p->nump() << " primes\n\n" << ct
     << endl;
}

/**
 * Compute prime after num primes
 */
void compute_next(tulong np = C + 1)
{
  Timer t;
  t.start();
  tulong next = p->nextprime(np);
  t.stop();
}

/**
 * Rendering
 */
void render(tulong np = N)
{
  Timer t;
  tulong i = 1, pr = i, m = p->maxp();
  t.start();
  do
  {
    pr = p->nextprime(pr);
    i%9 == 0 ? printf("%llu\n", pr) : printf("%llu ", pr);
  }while(++i <= np);
  t.stop();
  cout << "\nTime to Render\n" << t << endl;
}

/**
 * Compute and print first N primes
 */
int main()
{
  compute(10, 4);
  cout << " compute 10:\n" << p << endl;
  tulong sz = p->size() + 1;
  p->genprimes(11);
  cout << "generate 11:\n" << p << endl;
  tulong next = p->nextprime(sz);
  cout << " next prime:\n" << next << endl;
  if(next != 13)
    cerr << "ERROR: next(11): " << next << endl;
  cout << " compute 13:\n" << p << endl;
  if(!p->isprime(37))
    cerr << "ERROR: isprime(37) : false\n";
  cout << " isprime 37:\n" << p << endl;
  next = p->nextprime();
  cout << " next prime:\n" << next << endl;
  cout << p << endl;
  if(next != 41)
    cerr << "ERROR: next(37): " << next << endl << p->getPrimeVString() << endl;
  delete p;
  p = 0;

  return 0;
}

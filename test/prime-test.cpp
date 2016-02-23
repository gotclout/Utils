#include <iostream>
#include <string>
#include <stdio.h>

#include "Timer.h"
#include "Primes.h"

#define N 100000ULL  //Num Primes
#define C 1299710ULL //Start Prime Initalization Value

static long long elapsed = 0;

/**
 * Compute and print first N primes
 */
int main()
{
  //Try keeping initialization times out of the processing times
  tulong i  = 1,
         pr = 1;

  Timer ct, //Computation timer
        pt; //Rendering timer

  ct.start();//Start Computation
  Primes p(C);
  ct.stop();

  cout << "Time to Compute "    << N << " primes\n" << ct
       << "SIZE : " << p.size() << '\n'
       << "COUNT: " << p.nump() << '\n'
       << "MAX:   " << p.maxp() << '\n'; //Misc Stats

  //add this time in and see how close total is to system
  elapsed += ct.getElapsedNSecs();

  ct.resume();//Rendering
  for(pr = p.nextprime(pr); i <= N; pr = p.nextprime(pr), i++)
    i%9 == 0 ? printf("%llu\n", pr) : printf("%llu ", pr);
  ct.stop();

  cout << "\nTime to Render\n" << ct << "TOTAL: "
       << "elapsed + ct.getDurationNSecs()\n"
       << elapsed << " + " << ct.getDurationNSecs() << '\n'
       << elapsed + ct.getDurationNSecs() << "ns";

  return 0;
}

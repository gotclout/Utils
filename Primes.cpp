#ifndef _PRIMES_
#define _PRIMES_

#include <vector>
#include <cmath>
#include <climits>

using namespace std;

typedef unsigned long long tulong;

class Primes
{
  private:

    /** State Variables **/
    tulong idx,
           jdx,
           max,
           next,
           sq,
           count;

    bool retVal;

    /** Prime vector indexed beginning at 0 **/
    vector<bool> primeV;

  public:

    /**
     * The number of primes currently contained in primeV
     */
    inline const tulong& nump() const { return count; };

    /**
     * The value of the largest prime currently contained in primeV
     */
    inline const tulong& maxp() const { return max; };

    /**
     * Retrieves the size of primeV
     */
    inline size_t size() const { return primeV.size(); };

    /**
     * Initialize
     */
    explicit Primes() : primeV(ULLONG_MAX, false)
    {
      primeV[2] = true;
    };

    /**
     * Compute primes uing sieve up to init
     */
    explicit Primes(const tulong & init) : primeV(init, true)
    {
      max = 0;
      //genprimes(init);
      tulong  j, i;
      idx = 2;
      i = count = init - 1;
      primeV[0] = primeV[1] = false;

      //if prime mark off the factors of the prime
      for(; (sq = idx*idx) <= init; idx++)
      {
        if(primeV[idx])
        {
          for(jdx = sq, j = 0; jdx < init; j++, jdx = (sq) + (j*idx))
          {
            if(primeV[jdx])
            {
              primeV[jdx] = false;
              --count;
            }
          }
        }
      }

      //find the max prime and set it
      while(i > idx && !max)
      {
        if(primeV[i--])
          max = 1 + i;
      }

      count--;
    };

    /**
     * TODO: Use sieve for updating primeV in isprime()
     */
    inline void genprimes(const tulong & init)
    {
      if(max == 0)
      {
        tulong  j, i;
        idx = 2;
        i = count = init - 1;
        primeV[0] = primeV[1] = false;

        //if prime mark off the factors of the prime
        for(; (sq = idx*idx) <= init; idx++)
        {
          if(primeV[idx])
          {
            for(jdx = sq, j = 0; jdx < init; j++, jdx = (sq) + (j*idx))
            {
              if(primeV[jdx])
              {
                primeV[jdx] = false;
                --count;
              }
            }
          }
        }

        //find the max prime and set it
        while(i > idx && !max)
        {
          if(primeV[i--])
            max = 1 + i;
        }

        count--;
      }
    };

    /**
     * Locate the next prime greater than the specified value
     */
    inline tulong nextprime(const tulong & i)
    {
      tulong rval = i;

      if(++rval > max)
        while(!isprime(rval)) rval++;
      else
        while(!primeV[rval]) rval++;

      return rval;
    };

    /**
     * Locate the next prime greater than the current prime index
     */
    inline tulong nextprime()
    {
      while(!primeV[++idx]) {;}

      return idx;
    };

    /**
     * Determines if the specified value p is a prime by referencing
     * primeV index if p <= max otherwise the long way should just update
     * the list via primesieve
     */
    inline bool isprime(const tulong & p)
    {
      retVal = true;

      if(p > max)
      {
        next = idx = 2;
        do
        {
          if(p % next == 0)
          {
            primeV.push_back(false);
            retVal = false;
          }
          else
            next = nextprime();
        }while(retVal && next < sqrt(p) + 1);

        if(retVal)
        {
          primeV.push_back(true);
          max = p;
          count++;
        }
      }
      else
        retVal = primeV[p];

      return retVal;
    };
};

#endif //_PRIMES_

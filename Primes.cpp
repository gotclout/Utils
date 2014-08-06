#ifndef _PRIMES_
#define _PRIMES_

#include <vector>
#include <cmath>
#include <climits>
#include <ostream>

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
    explicit Primes() : primeV(ULONG_MAX, false)
    {
      max = 2;
      primeV[max] = true;
    };

    /**
     * Compute primes using sieve up to init
     */
    explicit Primes(const tulong & init) : primeV(init, true)
    {
      tulong  j, i = init;
      max = 0;
      idx = 2;
      count = init - 2;
      primeV[0] = primeV[1] = false;

      //if prime mark off the factors of the prime
      for(; (sq = idx*idx) <= init; ++idx)
      {
        if(primeV[idx])
        {
          for(jdx = sq, j = 0; jdx < init; ++j, jdx = (sq) + (j*idx))
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
      while(--i > idx && !max) if(primeV[i]) max = i;
    };

    /**
     * Use sieve for updating primeV in isprime()
     */
    inline void genprimes(const tulong & init)
    {
      tulong  j, i = init - primeV.size() + 1;
      max = 0;
      idx = 2;
      primeV.insert(primeV.end(), i, true);
      i = primeV.size();
      count = i - count - 2;

      //if prime mark off the factors of the prime
      for(; (sq = idx*idx) <= size(); ++idx)
      {
        if(primeV[idx])
        {
          for(jdx = sq, j = 0; jdx < size() ; ++j, jdx = (sq) + (j*idx))
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
      while(--i > idx && !max) if(primeV[i]) idx = max = i;
    };

    /**
     * Locate the next prime greater than the specified value
     */
    inline tulong nextprime(const tulong & cur)
    {
      tulong rval = cur, next = max;
      if(++rval > max) while(next == max) genprimes(++rval);
      while(!primeV[rval]) ++rval;

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
      return (primeV.size() - 1 < p) ? nextprime(p) : primeV[p];
    };

    /**
     *
     */
    friend ostream& operator<<(ostream & o, Primes* & p)
    {
      return operator<<(o, *p);
    };

    /**
     *
     */
    friend ostream& operator<<(ostream & o, Primes & p)
    {
      o << "Rendering Primes...\n";

      size_t sz = p.size();
      o << "SIZE:   " << sz << "\nPRIMES: " << p.nump()
        << "\nMAX:    " << p.maxp() << '\n';

      for(size_t c = 0; c < sz; ++c)
      {
        o << "v[" << c << "] : ";
        if(p.primeV[c]) o << '1';
        else o << '0';
        o << endl;
      }

      return o;
    };
};

#endif //_PRIMES_

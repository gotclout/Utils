#ifndef _PRIMES_
#define _PRIMES_

#include <vector>
#include <cmath>
#include <climits>
#include <string>
#include <sstream>
#include <ostream>
#include <iomanip>

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
           count,
           msz;

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
    inline size_t size() { return msz = primeV.size(); };

    /**
     * Initialize
     */
    explicit Primes() : primeV(3, false)
    {
      primeV[max = 2] = true;
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

      while(--i > idx && !max) if(primeV[i]) max = i;
    };

    /**
     * Use sieve for updating primeV in isprime()
     */
    inline void genprimes(const tulong & init)
    {
      tulong  j, i = init - size() + 1;
      bool sc = true;
      if(init < msz) {i = msz - init + 1; count += 2; sc = false;}
      max = 0;
      idx = 2;
      primeV.insert(primeV.end(), i, true);
      i = size();
      if(sc)
      count = i - count - 2;

      //if prime mark off the factors of the prime
      for(; (sq = idx*idx) <= msz; ++idx)
      {
        if(primeV[idx])
        {
          for(jdx = sq, j = 0; jdx < msz; ++j, jdx = (sq) + (j*idx))
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
    inline const tulong& nextprime()
    {
      while(!primeV[++idx]) {;}
      return (idx <= msz) ? idx : idx = nextprime(max);
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
     * Outputstream operator overload for pointer types
     */
    friend ostream& operator<<(ostream & o, Primes* & p)
    {
      return operator<<(o, *p);
    };

    /**
     * Render the prime vector string
     */
    string getPrimeVString()
    {
      stringstream ss, s;
      string tmp;
      ss << "\nPrime Vector:\n\n";
      ss.str("");
      ss.clear();
      size_t sz = primeV.size();
      for( size_t c = 0; c < sz; ++c)
      {
        s << "v[" << c << "]:" << primeV[c];
        string tmp = s.str();
        size_t z = tmp.length();
        if(!((1+c) % 7))
          ss << tmp << endl;
        else
        {
          ss << left << setw(z + (12 - z)) << setfill(' ') << tmp;
          ss << " ";
        }
        s.str("");
        s.clear();
      }
      return ss.str();
    }

    /**
     * Outputstream operator overload
     */
    friend ostream& operator<<(ostream & o, Primes & p)
    {
      stringstream ss;
      o << "Rendering Primes...\n";

      size_t sz = p.size(), c = 0;
      o << "\nSIZE:   " << sz << "\nPRIMES: " << p.nump()
        << "\nMAX:    " << p.maxp() << '\n';

      o << "\nPrime Values:\n\n";

      c = 1;
      for(p.idx = p.nextprime(1); p.idx <= p.max; p.idx = p.nextprime(), ++c)
      {
        ss << p.idx;
        string tmp = ss.str();
        size_t z = tmp.length();
        if(!((1+c)%10))
          o << tmp << endl;
        else
        {
          o << left << setw(z + (7 - z)) << setfill(' ') << tmp;
          o << " ";
        }
        ss.str("");
        ss.clear();
      }
      return o;
    };
};

#endif //_PRIMES_


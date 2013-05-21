#ifndef _TIMER_
#define _TIMER_

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

typedef unsigned long long tulong;

#ifdef _POSIX_VERSION

/**
 * A realtime pecision timer for POSIX complient systems
 * Use -lrt for gcc compiling
 */
class Timer
{
  /** The timespecs for this timer instances **/
  timespec startTime, endTime, curTime;

  /** The offset time for start stop and resume **/
  tulong offset;

  /** Indicates whether or not the timer is stopped or running **/
  bool stopped;

  /**
   * Set the time for a timespec in ns
   */
  inline tulong getTime(timespec & ts)
  {
    return !clock_gettime(CLOCK_REALTIME, &ts) ? getTimeNSecs(ts) : 0L;
  };

  /**
   * get time ns
   */
  inline tulong getTimeNSecs(timespec & ts)
  {
    return (ts.tv_sec * 1e9L) + ts.tv_nsec;
  };

  /**
   * get time micro sec
   */
  inline double getTimeUSecs(timespec & ts) { return getTimeNSecs(ts) * 0.0010; };

  /**
   * get time milisec
   */
  inline double getTimeMSecs(timespec & ts) { return getTimeUSecs(ts) * 0.0010; };

  /**
   * get time centisecs
   */
  inline double getTimeCSecs(timespec & ts) { return getTimeMSecs(ts) * 0.1000; };

  /**
   * get time decisecs
   */
  inline double getTimeDSecs(timespec & ts) { return getTimeCSecs(ts) * 0.1000; };

  /**
   * Retrieves the value of the timespec sec
   */
  inline double getTimeSecs(timespec & ts) { return getTimeDSecs(ts) * 0.1000; };

  /**
   * get time offset (while paused until resume) nsec
   */
  inline tulong getOffsetNSecs() { return offset; };

  /**
   * get time offset (while paused until resume) USecs
   */
  inline double getOffsetUSecs() { return getOffsetNSecs() * 0.0010; };

  /**
   * get time offset (while paused until resume) milisecs
   */
  inline double getOffsetMSecs() { return getOffsetUSecs() * 0.0010; };

  /**
   * get time offset (while paused until resume) centisecs
   */
  inline double getOffsetCSecs() { return getOffsetMSecs() * 0.1000; };

  /**
   * get time offset (while paused until resume) decisec
   */
  inline double getOffsetDSecs() { return getOffsetCSecs() * 0.1000; };

  /**
   * get time offset (while paused until resume) sec
   */
  inline double getOffsetSecs()  { return getOffsetDSecs() * 0.1000; };

  public:

  /**
   * Default Constructor
   */
  Timer()
  {
    offset = 0L; stopped = 1L;
  };

  /**
   * Ouput operator overload
   */
  friend ostream& operator << (ostream & o, Timer & t)
  {
    o << "Rendering Timer - "
      << "Start Time: " << t.getTimeNSecs(t.startTime) << " ns "
      << "End Time: "   << t.getTimeNSecs(t.endTime) << " ns "
      << "Cur Time: "   << t.getTimeNSecs(t.curTime) << " ns "
      << "Offset: "     << t.offset << " ns "
      << "Elapsed: "    << t.elapsed() << " ns "
      << "Duration: "   << t.getTimeNSecs(t.endTime) -
                           t.getTimeNSecs(t.startTime) -
                           t.offset << " ns : "
      << t.getTimeSecs(t.endTime) -
         t.getTimeSecs(t.startTime) -
         t.getOffsetSecs() << " s\n";

    return o;
  };

  /**
   * Starts the timer
   */
  tulong start()
  {
    tulong st = -1;

    if(stopped)
    {
      stopped = 0;
      st = getTime(startTime);
      curTime = startTime;
    }
    else
    {
      cerr << "ERROR: The timer has already been started. Use Timer::stop to "
           << "stop the timer or Timer::reset to reset it.\n";
    }

    return st;
  };

  /**
   * Stops the timer
   */
  tulong stop()
  {
    tulong d = -1;

    if(!stopped)
    {
      stopped = 1;
      d = getTime(endTime);
      curTime = endTime;
    }
    else
    {
      cerr << "ERROR: Timer has not been started before stop."
           << " Use Timer::start() to start the timer.\n";
    }

    return d;
  };

  /**
   * Resumes a stopped timer
   */
  tulong resume()
  {
    if(stopped)
    {
      offset =  (getTime(curTime) - getTimeNSecs(endTime)) - offset;
      stopped = 0;
    }
    else
    {
      cerr << "ERROR: Timer has not been stopped before resume."
           << " Use Timer::stop() to stop the timer.\n";
    }

    return offset;
  };

  /**
   * Compute the amount of time elapsed since the timer was last stopped
   * or checked
   */
  tulong elapsed()
  {
    tulong d;
    timespec now;
    d = getTime(now) - getTimeNSecs(curTime);
    curTime = now;
    return d;
  };

  /**
   *  Converts the current time to HH:MM:SS formatted string
   */
  string getTimeStr()
  {
    stringstream ss;
    time_t cur = time(0);
    struct tm* now = gmtime(&cur);

    if(now->tm_hour < 10) ss << 0;
    ss << now->tm_hour << ":";
    if(now->tm_min < 10) ss << "0";
    ss << now->tm_min << ":";
    if(now->tm_sec < 10) ss << 0;
    ss << now->tm_sec;

    return ss.str();
  }

  /**
   * Compute the duration of the interval from start to stop
   */
  tulong durationNSecs()
  {
    if(!stopped) stop();

    return getTimeNSecs(endTime) - getTimeNSecs(startTime) - offset;
  }

  /**
   * Compute duration usec
   */
  inline double durationUSecs() { return durationNSecs() * 0.0010; };

  /**
   * Compute duration msec
   */
  inline double durationMSecs() { return durationUSecs() * 0.0010; };

  /**
   * Compute duration csec
   */
  inline double durationCSecs() { return durationMSecs() * 0.1000; };

  /**
   * Compute duration dsec
   */
  inline double durationDSecs() { return durationCSecs() * 0.1000; };

  /**
   * Compute duration sec
   */
  inline double durationSecs() { return durationDSecs() * 0.1000; };

  /**
   * Reset the timer
   */
  void reset()
  {
    offset = 0;
    stopped = 1;
  };

  /**
   * Destructor ;)
   */
  ~Timer()
  {
    if(!stopped) stop();
    cerr << *this;
  }
};

#else

#warning "POSIX CLOCK_REALTIME Unavailable, using system clock()"

/**
 * The less percise time operates on system time
 */
class Timer
{
  clock_t startTime, endTime, curTime;

  tulong offset;

  bool stopped;

  public:

  Timer()          { offset = 0; stopped = 1; };
  void   start()   { startTime = clock(); curTime = startTime; stopped = 0; };
  void   stop()    { endTime = clock(); curTime = endTime; stopped = 1; };
  tulong resume()  { offset = endTime - clock(); stopped = 0; return offset; };
  double elapsed() { return (clock() - startTime) / (double) CLOCKS_PER_SEC; };
  tulong ticks ()  { return (endTime - startTime - offset); };
  double duration(){ return ticks() / (double) CLOCKS_PER_SEC; };
};

#endif//_POSIX_VERSION
#endif//_TIMER_


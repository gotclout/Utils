#ifndef __SEMAPHORE__
#define __SEMAPHORE__

#include <string.h>
#include <limits.h>   //NAME_MAX
#include <fcntl.h>    //O_* constants
#include <pthread.h>  //-ltr
#include <sys/stat.h> //S_* constants
#include <sys/time.h> //timespec - sem_timedwait()
#include <semaphore.h>

/**
 * @version 0.1
 * @since March 9 2015
 * @class Semaphore
 * @brief This class is a semaphore wrapper
 */
class Semaphore
{
  private:

  /** pointer to the allocated semaphore **/
  sem_t* semaphore;

  /** unnamed semaphore **/
  sem_t  unsemaphore;

  /** The initialization success value **/
  bool initialized;

  /** The name for named semaphores **/
  char* name;

  /** The semaphore count value **/
  int val;

  protected:

  public:

  /**
   * Construct unnamed semaphore
   *
   * @param int pShr, is the shared value
   * @param int pVal, is the initial count value
   */
  Semaphore(int pShr = 0, int pVal = 1)
  {
    initialized = !((bool) sem_init(&unsemaphore, pShr, pVal));
    semaphore   = &unsemaphore;
    name        = 0;
  }

  /**
   * Construct named semaphore
   *
   * @param const char* pName is the semaphore name
   * @param int pFlag is the creation falg
   * @param mode_t pMode is the mode flag
   * @param int pVal is the initial count value
   */
  Semaphore(const char* pName, int pFlag = 0, mode_t pMode = 0, int pVal = 1)
  {
    name = new char[NAME_MAX - 4];
    if(pName) strncpy(name, pName, NAME_MAX - 4);
    else
    {
      memset(name, 0, NAME_MAX - 4);
      strncpy(name, "/default", 8);
    }

    if(pFlag == 0) pFlag = O_CREAT | O_EXCL;  //create if not exists
    if(pMode == 0) pMode = S_IRUSR | S_IWUSR; //read and write modes (0600)

    val         = pVal;
    semaphore   = sem_open(name, pFlag, pMode, val);
    initialized = semaphore != SEM_FAILED;
  };

  /**
   * Decrement the semaphore count
   */
  int wait() { return sem_wait(semaphore); };

  /**
   * Decrement the semaphore count
   */
  int trywait() { return sem_trywait(semaphore); };

  /**
   * Decrement the semaphore count
   *
   * @param timespec* pTimeOut is the wait duration
   */
  int timedwait(const struct timespec* pTimeOut)
  {
    return pTimeOut ? sem_timedwait(semaphore, pTimeOut) : -1;
  };

  /**
   * Increment semaphore count
   */
  int post() { return sem_post(semaphore); };

  /**
   * Retrieve the semaphore count
   */
  int value() { sem_getvalue(semaphore, &val); return val; };

  /**
   * Deallocate semaphore resources and unlink if required
   */
  ~Semaphore()
  {
    if(initialized)
    {
     if(name)
     {
       sem_unlink(name);
       sem_close(semaphore);
     }
     else sem_destroy(semaphore);
    }
  };
};

#endif//__Semaphore__

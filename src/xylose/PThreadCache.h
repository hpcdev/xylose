/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2006-2009 Spencer E. Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/** \file
 * Implementation of a generally useful PThreads cache and associated task
 * manager.
 *
 * Copyright 2006-2009 Spencer Olson
 *
 * Note that this is not a gneric portable thread cache manager (yet).  This
 * will only work on systems with posix-threads available.  
 *
 * */

#ifndef xylose_PThreadCache_h
#define xylose_PThreadCache_h

#include <xylose/logger.h>
#include <xylose/strutil.h>

#include <pthread.h>
#include <sched.h>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <stdexcept>

#include <cstdio>

namespace xylose {

  /** The basic pthread task structure.  Inheriting classes must implement the
   * exec() function. */
  class PThreadTask {
    public:
      PThreadTask() {}
      virtual ~PThreadTask() {}
      virtual void exec() = 0;
  };

  /** A set of pointers to tasks. */
  typedef std::set<PThreadTask *> PThreadTaskSet;

  /** A PThreads threads cache and associated tasks manager. */
  class PThreadCache {
    /* MEMBER STORAGE */
  private:
    int max_threads;    /**< protected by max_threads_spinlock*/
    int total_threads;  /**< protected by total_threads_spinlock*/
    int active_threads; /**< protected by max_threads_spinlock (yes, same one)*/
    const bool create_threads_on_demand; /**< whether to create threads on
                                              demand ( up to max_threads ). */

    pthread_attr_t  pthread_attr; /**< protected by max_threads_spinlock */
    pthread_mutex_t task_queue_mutex; /* mutex to protect the queue items. */
    pthread_cond_t  task_ready_cond;  /* condition for task queue additions. */
    pthread_cond_t  task_finished_cond;
    pthread_mutex_t task_finished_mutex;/* mutex to protect the finished set. */
    pthread_spinlock_t max_threads_spinlock;/* mutex to protect max_threads. */
    pthread_spinlock_t total_threads_spinlock;/* mutex to protect total_threads. */

    pthread_t ** threads; /* thread id of each live thread */


    std::queue<PThreadTask *> task_queue; /* a linked list of tasks to do. */
    PThreadTaskSet finished_tasks; /* a linked list of finished tasks. */

    bool slavesQuit;



    /* MEMBER FUNCTIONS */
  private:
    inline void signalSlavesQuit() {
      /* We'll use the task_queue_mutex, just because each slave will exit the
       * cond_wait() holding this mutex.  So, if we already hold it,
       * they can't read the quit variable. */
      pthread_mutex_lock(&task_queue_mutex);

      slavesQuit = true;
      /* now send the message to all slaves. */
      pthread_cond_broadcast(&task_ready_cond);

      pthread_mutex_unlock(&task_queue_mutex);
    }

    inline void resetSlavesQuit() {
      pthread_mutex_lock(&task_queue_mutex);
      slavesQuit = false;
      pthread_mutex_unlock(&task_queue_mutex);
    }


    inline PThreadTask * getTask() {
      PThreadTask * task = NULL;
      pthread_mutex_lock(&task_queue_mutex);

      while (!slavesQuit) {
        task = task_queue.empty() ? NULL : task_queue.front();
        if (task == NULL){
          pthread_cond_wait(&task_ready_cond, &task_queue_mutex);
        } else {
          task_queue.pop();
          break;
        }
      }

      pthread_mutex_unlock(&task_queue_mutex);
      return task;
    }

    /** Add this task to the finished tasks vector and signal the waiting
     * task owners that a task has finished. */
    inline void signalTaskFinished(PThreadTask * task) {
      /* add the task to the finished tasks. */
      pthread_mutex_lock(&task_finished_mutex);

      finished_tasks.insert(task);
      pthread_cond_broadcast(&task_finished_cond);

      pthread_mutex_unlock(&task_finished_mutex);
    }

    /** Executes all tasks placed in the task queue. */
    static void taskSlave(PThreadCache * cache) {
      cache->incrementTotalThreads();    /* inc total     */

      /* check queue, if we get NULL back, that means that we were
       * requested to terminate. */
      PThreadTask * task;
      while ((task = cache->getTask()) != NULL) {
        cache->incrementActiveThreads(); /* inc active    */
        task->exec();                    /* execute task. */
        cache->decrementActiveThreads(); /* dec active    */
        cache->signalTaskFinished(task); /* signal finish */
      }

      cache->decrementTotalThreads();    /* dec total     */
    }

    /** Increment the active threads counter--this should only ever be called by
     * the taskSlave function. */
    void incrementActiveThreads() {
      pthread_spin_lock(&max_threads_spinlock);
      ++active_threads;
      pthread_spin_unlock(&max_threads_spinlock);
    }

    /** Decrement the active threads counter--this should only ever be called by
     * the taskSlave function. */
    void decrementActiveThreads() {
      pthread_spin_lock(&max_threads_spinlock);
      --active_threads;
      pthread_spin_unlock(&max_threads_spinlock);
    }

    /** Decrement the active threads counter--this should only ever be called by
     * the taskSlave function. */
    void decrementTotalThreads() {
      pthread_spin_lock(&total_threads_spinlock);
      --total_threads;
      pthread_spin_unlock(&total_threads_spinlock);
    }

    /** Increment the active threads counter--this should only ever be called by
     * the taskSlave function. */
    void incrementTotalThreads() {
      pthread_spin_lock(&total_threads_spinlock);
      ++total_threads;
      pthread_spin_unlock(&total_threads_spinlock);
    }

    static bool getOnDemandOption() {
      const char * ondemand_cstr = getenv("ONDEMAND_PTHREADS");
      if ( ondemand_cstr ) {
        const std::string s =  tolower( ondemand_cstr );

        if ( s == "f" || s == "false" || s == "0" || s == "no" )
          return false;
      }

      return true;
    }

    static void start_thread( pthread_t ** id,
                              pthread_attr_t & attr,
                              PThreadCache * c ) {
      logger::log_fine( "STARTING THREAD!!!!!" );
      if ( (*id) == NULL )
        (*id) = new pthread_t;

      if ( pthread_create( *id, &attr, (void*(*)(void*))taskSlave, c ) != 0 )
        throw std::runtime_error("PThreadCache:  could not start thread!");
    }

    void waitForStartedThread( const int & tot ) {
      bool again = true;
      while ( again ) {
        /* we want to make sure that the thread has at least started before we
         * continue. */
        pthread_spin_lock(&total_threads_spinlock);
          if ( tot <= total_threads )
            again = false;
        pthread_spin_unlock(&total_threads_spinlock);

        if ( again )
          sched_yield();
      }
    }



  public:
    /** Constructor.
     * Sets the initial value of max_threads from the environment variable
     * NUM_PTHREADS.  If this variable is not set, then the default will be to
     * NOT create any threads for executing tasks given this this cache
     * manager.
     */
    PThreadCache() : max_threads(0),
                     total_threads(0),
                     active_threads(0),
                     create_threads_on_demand( getOnDemandOption() ),
                     threads(NULL),
                     task_queue(),
                     finished_tasks() {
      pthread_attr_init(&pthread_attr);
      pthread_mutex_init(&task_queue_mutex, NULL);
      pthread_mutex_init(&task_finished_mutex,NULL);
      pthread_spin_init(&max_threads_spinlock,0);
      pthread_spin_init(&total_threads_spinlock,0);
      pthread_cond_init(&task_ready_cond, NULL);
      pthread_cond_init(&task_finished_cond, NULL);

      slavesQuit = false;

      char * max_thread_str = getenv("NUM_PTHREADS");
      int mxth = 1;

      if ( max_thread_str == NULL ||
         sscanf(max_thread_str, "%d", &mxth) != 1 ) {

        /* we'll default to one thread only */
        mxth = 1;
      }

      /* call this function to set up each of the threads. */
      set_max_threads(mxth);
    }

    /** Destructor. */
    ~PThreadCache() {
      /* make sure that all threads are dead. */
      set_max_threads(0);
      pthread_attr_destroy(&pthread_attr);
      pthread_mutex_destroy(&task_queue_mutex);
      pthread_mutex_destroy(&task_finished_mutex);
      pthread_spin_destroy(&max_threads_spinlock);
      pthread_spin_destroy(&total_threads_spinlock);
      pthread_cond_destroy(&task_ready_cond);
      pthread_cond_destroy(&task_finished_cond);
    }

    /** Add a task to the thread cache task queue.
     * @param task
     *   The task to add the the task queue.
     * @param self_if_none_avail
     *   Whether to perform the work by self if no threads are currently
     *   available [Default false].
     */
    void addTask( PThreadTask * task, bool self_if_none_avail = false ) {
      pthread_spin_lock(&total_threads_spinlock);
      pthread_spin_lock(&max_threads_spinlock);
        bool serial = max_threads <= 1 ||
                      ( self_if_none_avail && 
                        ( max_threads - active_threads ) == 0 );

        logger::log_finer(
          "addTask:   max_threads:%d"
                   ", active_threads:%d"
                   ", total_threads:%d"
                   ", serial:%s"
                   ", task_queue.size():%lu",
                  max_threads,
                  active_threads,
                  total_threads,
                  (serial ? "true":"false"),
                  task_queue.size() );
        bool thread_start = false;
        int new_total = -1;

        if ( !serial ) {
          pthread_mutex_lock(&task_queue_mutex);      /* locked queue */

          thread_start = create_threads_on_demand &&
                         total_threads < max_threads &&
                         (active_threads == total_threads ||
                          total_threads <= static_cast<int>(task_queue.size()));

          if ( thread_start ) {
            new_total = total_threads + 1;

            /* no live thread is available for performing a task.
             * Start by finding an empty thread id holder. */
            int i = 0;
            for ( ; i < max_threads; ++i ) {
              if ( threads[i] == NULL )
                break;
            }

            if ( i >= max_threads )
              throw std::runtime_error(
                "PThreadCache:  expected to find empty thread slot!"
              );

            /* start the thread. */
            start_thread( &threads[i], pthread_attr, this );
          }

          task_queue.push(task);
          /* now we should probably signal some thread that there is a task
           * ready for execution. */
          pthread_cond_signal(&task_ready_cond);

          pthread_mutex_unlock(&task_queue_mutex);    /* unlocked queue */
        }
      pthread_spin_unlock(&max_threads_spinlock);
      pthread_spin_unlock(&total_threads_spinlock);

      if ( thread_start )
        waitForStartedThread(new_total);

      if ( serial ) {
        task->exec();
        signalTaskFinished(task);
      }
    }

    /** This function waits for some tasks to finish, if there are any
     * executing, and then returns the particular task(s) that finished.
     *
     * NOTE:  If the tasks in callers_tasks are not actually executing or
     * queued to execute, then this function could cause indefinite deadlock.
     */ 
    inline PThreadTaskSet waitForTasks(const PThreadTaskSet & callers_tasks) {
      /* Only return once one of the callers tasks have actually completed. */
      PThreadTaskSet retval;

      pthread_mutex_lock(&task_finished_mutex);

      /* The order of the following is to avoid race condtions with the
       * pthread_cond_broadcast/wait commands. */
      do {
        /* Find all finished tasks that the caller owns. */
        std::set_intersection(callers_tasks.begin(), callers_tasks.end(),
                              finished_tasks.begin(), finished_tasks.end(),
                              inserter(retval, retval.begin()));

        if (retval.size() != 0) break;

        pthread_cond_wait(&task_finished_cond, &task_finished_mutex);
      } while (true);

      /* We have successfully finished one of the callers tasks.  
       * Now make sure that we remove the finished tasks from the
       * finished_tasks set.  We'll use the set operations to do this. */
      PThreadTaskSet tmp;
      std::set_difference(finished_tasks.begin(), finished_tasks.end(),
                          callers_tasks.begin(), callers_tasks.end(),
                          inserter(tmp, tmp.begin()));
      finished_tasks.swap(tmp);

      pthread_mutex_unlock(&task_finished_mutex);

      return retval;
    }

    /** Change/Set the number of threads used to execute tasks. */
    inline int set_max_threads(int mx) {
      pthread_spin_lock(&max_threads_spinlock);

      if( max_threads != mx ) {
        if(max_threads > 1) {
          /* we previously set up threads, so lets tell them to stop. */
          signalSlavesQuit();

          /* join each of the threads. */
          for ( int i = 0; i < max_threads; ++i ) {
            if ( threads[i] ) {
              pthread_join(*threads[i],NULL);
              delete threads[i];
              threads[i] = NULL;
            }
          }

          resetSlavesQuit();

          /* free up the old list of thread ids. */
          delete[]threads;
          threads = NULL;
        }/* if we have to join some threads */

        max_threads = mx > 1 ? mx : 1;

        /* only create new threads IF more than one are requested. 
         * If there is only one thread, the tasks will be executed serially at
         * the point of addTask(). */
        if(max_threads > 1) {
          /* we are instructed to prepare for threaded processing. */
          threads = new pthread_t * [max_threads];

          if ( create_threads_on_demand ) {
            std::fill( threads, threads + max_threads, (pthread_t*)(NULL) );
          } else {
            for ( int i = 0; i < max_threads; ++i ) {
              start_thread( &threads[i], pthread_attr, this );
            }
          }
        }/* if more than one thread requested */
      }/* if the request is a change */

      mx = max_threads;
      pthread_spin_unlock(&max_threads_spinlock);

      if ( mx > 1 && !create_threads_on_demand )
        waitForStartedThread(mx);

      return mx;
    }

    /** Get the maximum number of threads that will be used to execute tasks.
     * */
    inline int get_max_threads() {
      pthread_spin_lock(&max_threads_spinlock);
      int mx = max_threads;
      pthread_spin_unlock(&max_threads_spinlock);
      return mx;
    }

    /** Get both maximum number of threads that will be used to execute tasks as
     * well as the number of currently active threads.
     * */
    inline std::pair<int,int> get_active_threads() {
      pthread_spin_lock(&max_threads_spinlock);
      std::pair<int,int> retval( max_threads, active_threads );
      pthread_spin_unlock(&max_threads_spinlock);
      return retval;
    }

    /** Get the current number of threads that are alive (created).  This is
     * necessarily less than get_max_threads().
     * */
    inline int get_live_threads() {
      pthread_spin_lock(&total_threads_spinlock);
      int tot = total_threads;
      pthread_spin_unlock(&total_threads_spinlock);
      return tot;
    }
  };

  /** The default thread cache. */
  static PThreadCache pthreadCache;

  /** A function to satisfy OpenMP/PThread mixed code. */
  static inline int get_max_threads() {
      return pthreadCache.get_max_threads();
  }

}/* namespace xylose */

#endif // xylose_PThreadCache_h

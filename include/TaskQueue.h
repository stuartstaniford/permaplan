// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "Lockable.h"
#include <list>


// =======================================================================================
// Forward declarations

class TaskQueue;
class TaskQueueFarm;
class HttpLoadBalancer;


// =======================================================================================
/// @brief Specify a chuck of work that can be placed on a TaskQueue.

class Task
{
 public:
  
  // public methods
  Task(void (*work)(void*, TaskQueue*), void* arg);
  
  // member variables
  void (*doWork)(void*, TaskQueue*);
  void* theArg;  
};


// =======================================================================================
/// @brief Task queue class for use in multi-threaded version.
///
/// This is used to manage a worker thread which has a queue of tasks to perform.  Other
/// threads can add things to the queue, and the worker will get to them.  Uses pthreads.

class TaskQueue: public Lockable, public std::list<Task*> 
{
  friend TaskQueueFarm;
  friend HttpLoadBalancer;
  
 public:
  
  // Instance variables - public
  
  // Member functions - public
  TaskQueue(unsigned index);
  ~TaskQueue(void);
  void workLoop(void);
  inline unsigned queueSize(void) {return tasksQueued;}
  void addTask(void (*work)(void*, TaskQueue*), void* arg);
  inline void die(void){timeToDie = true;}
  
protected:
  pthread_cond_t           taskWait;
  unsigned                 tasksQueued;
  unsigned                 tasksInProgress; // should only ever be 0 or 1
  unsigned                 queueIndex;
  bool                     timeToDie;
  pthread_t                workerThread;
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  TaskQueue(const TaskQueue&);                 // Prevent copy-construction
  TaskQueue& operator=(const TaskQueue&);      // Prevent assignment

};


// =======================================================================================

#endif





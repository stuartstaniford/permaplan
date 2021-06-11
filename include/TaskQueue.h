// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Task queue class for use in multi-threaded version.

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "Lockable.h"
#include <list>

// =======================================================================================
// Class variable initialization

class TaskQueue;

class TaskQueueFarm;

class Task
{
 public:
  
  // public methods
  Task(void (*work)(void*, TaskQueue*), void* arg);
  
  // member variables
  void (*doWork)(void*, TaskQueue*);
  void* theArg;  
};


class TaskQueue: public std::list<Task*>, public Lockable
{
  friend TaskQueueFarm;

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

#endif





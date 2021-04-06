// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Task queue class for use in multi-threaded version.

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "Lockable.h"
#include <list>

// =======================================================================================
// Class variable initialization

class Task
{
 public:
  
  // public methods
  Task(void (*work)(void*), void* arg);
  
  // member variables
  void (*doWork)(void*);
  void* theArg;  
};


class TaskQueue: public std::list<Task*>, public Lockable
{
  
 public:
  
  // Instance variables - public
  
  // Member functions - public
  TaskQueue(unsigned index);
  ~TaskQueue(void);
  void workLoop(void);
  inline unsigned queueSize(void) {return tasksQueued;}
  void addTask(void (*work)(void*), void* arg);
  inline void die(void){timeToDie = true;}
  
private:
  pthread_cond_t           taskWait;
  unsigned                 tasksQueued;
  unsigned                 queueIndex;
  bool                     timeToDie;
  pthread_t                workerThread;
  
  // Instance variables - private
  
  // Member functions - private
  TaskQueue(const TaskQueue&);                 // Prevent copy-construction
  TaskQueue& operator=(const TaskQueue&);      // Prevent assignment

};

#endif





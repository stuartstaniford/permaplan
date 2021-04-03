// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Task queue class for use in multi-threaded version.

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include "Lockable.h"
#include <forward_list>

// =======================================================================================
// Class variable initialization

class TaskQueue: public std::forward_list<void*>, public Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  TaskQueue(void);
  ~TaskQueue(void);
  void workLoop(void);
  void addTask(void);
  inline void die(void){timeToDie = true;}
  
private:
  pthread_cond_t           taskWait;
  unsigned                 tasksQueued;
  bool                     timeToDie;
  
  // Instance variables - private
  
  // Member functions - private
  TaskQueue(const TaskQueue&);                 // Prevent copy-construction
  TaskQueue& operator=(const TaskQueue&);      // Prevent assignment

};

#endif





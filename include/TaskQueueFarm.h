// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class manages a group of task queues, and provides some convenient methods
// for initializing and working with them.

#ifndef TASK_QUEUE_FARM_H
#define TASK_QUEUE_FARM_H

#include "TaskQueue.h"
#include "HttpDebug.h"

// =======================================================================================
// Class variable initialization

class TaskQueueFarm: public Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  TaskQueueFarm(unsigned nQueues, const char* lName);
  TaskQueueFarm(unsigned nQueues, TaskQueue** tQ, const char* lName);
  ~TaskQueueFarm(void);
  void addTask(unsigned i, void (*work)(void*, TaskQueue*), void* arg);
  void loadBalanceTask(void (*work)(void*, TaskQueue*), void* arg);
  void notifyTaskDone(void);
  void waitOnEmptyFarm(void);
  bool diagnosticHTML(HttpDebug* serv);

private:
  
  // Instance variables - private
  pthread_cond_t    tasksUnfinished;
  unsigned          nQ;
  unsigned          tasksOutstanding;
  TaskQueue**       taskQueues;
  const char*       logName;

  // Member functions - private
  unsigned findBestQueue(void);
  TaskQueueFarm(const TaskQueueFarm&);                 // Prevent copy-construction
  TaskQueueFarm& operator=(const TaskQueueFarm&);      // Prevent assignment
};

#endif





// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Task queue class for use in multi-threaded version.

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

// =======================================================================================
// Class variable initialization

class TaskQueue
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  TaskQueue(void);
  ~TaskQueue(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  TaskQueue(const TaskQueue&);                 // Prevent copy-construction
  TaskQueue& operator=(const TaskQueue&);      // Prevent assignment

};

#endif





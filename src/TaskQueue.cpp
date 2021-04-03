// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Task queue class for use in multi-threaded version.

#include "TaskQueue.h"
#include <err.h>

// =======================================================================================
// Constructor

TaskQueue::TaskQueue(void):
                    tasksQueued(0u),
                    timeToDie(false)
{
  if(pthread_cond_init(&taskWait, NULL))
    err(-1, "Couldn't initialize taskWait in TaskQueue::TaskQueue.");
}


// =======================================================================================
// Destructor

TaskQueue::~TaskQueue(void)
{
  if(pthread_cond_destroy(&taskWait))
    err(-1, "Couldn't destroy taskWait in TaskQueue::~TaskQueue.");
}


// =======================================================================================
// This is the main loop of the thread servicing this TaskQueue.  When there's no work,
// it waits on a condition variable.  When there are tasks in the queue, it does them one
// by one.

void TaskQueue::workLoop(void)
{
  while(!timeToDie)
   {
    lock();
    while (!tasksQueued)
      pthread_cond_wait(&taskWait, &mutex);

    // remove the task from the queue
    tasksQueued--;
    unlock();
    
    // perform the task
   }
}


// =======================================================================================
// Function called to add a task to this particular queue.

void TaskQueue::addTask(void)
{
  lock();
  
  // add a task to the queue
  
  tasksQueued++;
  pthread_cond_signal(&taskWait);
  unlock();

}


// =======================================================================================

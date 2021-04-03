// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Task queue class for use in multi-threaded version.

#include "TaskQueue.h"
#include <err.h>


// =======================================================================================
// Constructor for individual task

Task::Task(void (*work)(void*), void* arg):
                            doWork(work),
                            theArg(arg)
{
  
}


// =======================================================================================
// C wrapper for pthread_create

void* startWorkThread(void* arg)
{
  TaskQueue* queue = (TaskQueue*)arg;
  queue->workLoop();
  return NULL;
}


// =======================================================================================
// Constructor for entire task queue

TaskQueue::TaskQueue(void):
                    tasksQueued(0u),
                    timeToDie(false)
{
  if(pthread_cond_init(&taskWait, NULL))
    err(-1, "Couldn't initialize taskWait in TaskQueue::TaskQueue.");

  // Start the worker thread  
  int pthreadErr;

  if((pthreadErr = pthread_create(&workerThread, NULL, startWorkThread, (void*)this)) != 0)
    err(-1, "Couldn't spawn worker thread for TaskQueue.\n");
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

    // remove a task from the queue
    Task* task = back();
    pop_back();
    tasksQueued--;
    unlock();
    
    // perform the task (having released the taskqueue lock)
    task->doWork(task->theArg);
    delete task;
   }
}


// =======================================================================================
// Function called to add a task to this particular queue.

void TaskQueue::addTask(Task* task)
{
  lock();
  
  // add a task to the queue
  push_front(task);  
  tasksQueued++;
  
  pthread_cond_signal(&taskWait);
  unlock();
}


// =======================================================================================

// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class manages a group of task queues, and provides some convenient methods
// for initializing and working with them.

#include "TaskQueueFarm.h"
#include "Logging.h"

// =======================================================================================
// First Constructor (out of two!!).  This one initializes nQueues task queues ourselves.

TaskQueueFarm::TaskQueueFarm(unsigned nQueues, const char* lName):
                                        nQ(nQueues), 
                                        tasksOutstanding(0u),
                                        logName(lName)
{
  if(pthread_cond_init(&tasksUnfinished, NULL))
    err(-1, "Couldn't initialize tasksUnfinished in TaskQueueFarm::TaskQueueFarm.");
  taskQueues = new TaskQueue*[nQ];
  assert(taskQueues);
  
  for(unsigned s=0; s<nQ; s++)
    taskQueues[s] = new TaskQueue(s);
  LogTaskQueueFarmOps("Task farm %s has initialized %u task queues.\n", logName, nQ);
}


// =======================================================================================
// Second Constructor (of two).  This one takes a pointer to a set of taskqueue pointers
// initialized by someone else.  The typical use case for this is the objects are really
// some subclass of TaskQueue with extra state/behavior that we don't need to know about.

TaskQueueFarm::TaskQueueFarm(unsigned nQueues, TaskQueue** tQ, const char* lName):
                                        nQ(nQueues), 
                                        tasksOutstanding(0u),
                                        taskQueues(tQ),
                                        logName(lName)
{
  if(pthread_cond_init(&tasksUnfinished, NULL))
    err(-1, "Couldn't initialize tasksUnfinished in TaskQueueFarm::TaskQueueFarm.");
  LogTaskQueueFarmOps("Task farm %s started with %u supplied task queues.\n", logName, nQ);
}


// =======================================================================================
// Destructor

TaskQueueFarm::~TaskQueueFarm(void)
{
  for(int s=0; s<nQ; s++)
    taskQueues[s]->die();
  if(pthread_cond_destroy(&tasksUnfinished))
    err(-1, "Couldn't destroy tasksUnfinished in Scene::~Scene.");
  LogTaskQueueFarmOps("Task farm %s shut down.\n", logName);

  /* XX
  Need some synchronization here to know when it's safe to delete the threads.
  In practice there are only a very small number of farms that last the full life of
   the process so it's not a big deal
  for(int s=0; s<config.nSimThreads; s++)
    delete taskQueues[s];
  delete[] taskQueues;  
  */
}


// =======================================================================================
// Function to add work to one of the queues

void TaskQueueFarm::addTask(unsigned i, void (*work)(void*, TaskQueue*), void* arg)
{
  lock();
  tasksOutstanding++;
  taskQueues[i%nQ]->addTask(work, arg);
  unlock();
}

// =======================================================================================
// Function to implement load balancing.  Note this is called with the lock already held.

unsigned TaskQueueFarm::findBestQueue(void)
{
  unsigned lowestCount = 0xffffffff;
  int bestI = -1;
  
  for(int i=0; i< nQ; i++)
   {
    taskQueues[i]->lock();
    if(taskQueues[i]->tasksQueued + taskQueues[i]->tasksInProgress < lowestCount)
     {
      lowestCount = taskQueues[i]->tasksQueued + taskQueues[i]->tasksInProgress;
      bestI = i;
     }
    taskQueues[i]->unlock();
   } 
  
  if(lowestCount == 0xffffffff || bestI < 0)
    err(-1, "Craziness in TaskQueueFarm::findBestQueue");
  LogTaskQueueFarmOps("Task farm %s assigning task to queue %d.\n", logName, bestI);
 
  return bestI;
}


// =======================================================================================
// Function to called when we want to loadBalance a task, and don't have an index to 
// use as in addTask

void TaskQueueFarm::loadBalanceTask(void (*work)(void*, TaskQueue*), void* arg)
{
  lock();
  tasksOutstanding++;
  taskQueues[findBestQueue()]->addTask(work, arg);
  unlock();
}


// =======================================================================================
// Function to let us know that a piece of work is finished (generally called from 
// within the work function at the end).

void TaskQueueFarm::notifyTaskDone(void)
{
  lock();
  tasksOutstanding--;
  pthread_cond_signal(&tasksUnfinished);
  unlock();
}


// =======================================================================================
// Function to wait until the farm has performed all current work

void TaskQueueFarm::waitOnEmptyFarm(void)
{
  lock();
  while(tasksOutstanding > 0)
    pthread_cond_wait(&tasksUnfinished, &mutex);
  unlock();
}


// =======================================================================================
// Provide a diagnostic page with a table about all the task queues

bool TaskQueueFarm::diagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Task Queues");
  
  httPrintf("<center>\n");
  lock();
  httPrintf("<b>Overall tasks:</b> %u\n", tasksOutstanding);
  unlock();
  
  serv->startTable();
  httPrintf("<tr><th>Index</th><th>Task Count</th></tr>\n");
  
  for(int i=0; i < nQ; i++)
   {
    taskQueues[i]->lock();
    httPrintf("<tr><td>%d</td><td>%u</td></tr>\n", i, taskQueues[i]->queueSize());
    taskQueues[i]->unlock();
   }
  
  httPrintf("</table></center><hr>\n");
  if(!serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================

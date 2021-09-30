// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class manages a group of task queues, and provides some convenient methods
// for initializing and working with them.

#include "TaskQueueFarm.h"
#include "Logging.h"


// =======================================================================================
/// @brief First Constructor (out of two!!).  
/// 
/// This constructor makes a new TaskFarm and also initializes nQueues new instances of
/// TaskQueue to be managed by the new farm.
/// @param nQueues The number of new TaskQueue instances to create.
/// @param lName A const C-string name to use for logging about this particular 
/// TaskQueueFarm.

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
/// @brief Second Constructor (of two).  
/// 
/// This constructor takes a pointer to a set of taskqueue pointers initialized by 
/// someone else.  The typical use case for this is the objects are really some 
/// subclass of TaskQueue with extra state/behavior that we don't need to know about.
/// @param nQueues The number of pointers to TaskQueue being supplied to us.
/// @param tQ A pointer to an array of pointers to TaskQueue (or a subclass thereof).  We
/// will be assigning tasks to these queues going forward.
/// @param lName A const C-string name to use for logging about this particular 
/// TaskQueueFarm.

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
/// @brief Destructor

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
/// @brief Function to add work to one of the queues
/// @param i An index for which queue to use (will have modulus taken to fit in right
/// range).
/// @param work A C-style function pointer for the task function to be run.
/// @param arg A void* pointer to the data to be supplied to the task function

void TaskQueueFarm::addTask(unsigned i, void (*work)(void*, TaskQueue*), void* arg)
{
  lock();
  tasksOutstanding++;
  taskQueues[i%nQ]->addTask(work, arg);
  unlock();
}


// =======================================================================================
/// @brief Function to implement load balancing.  
/// 
/// Currently this works by finding the queue with the least number of waiting items in.
/// Note this is called with the lock already held.
/// @returns An unsigned with our estimate of the best queue to use.

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
/// @brief loadBalance a task without an index to use as in addTask
/// @param work A C-style function pointer for the task function to be run.
/// @param arg A void* pointer to the data to be supplied to the task function

void TaskQueueFarm::loadBalanceTask(void (*work)(void*, TaskQueue*), void* arg)
{
  lock();
  tasksOutstanding++;
  taskQueues[findBestQueue()]->addTask(work, arg);
  unlock();
}


// =======================================================================================
/// @brief Function to let us know that a piece of work is finished.
/// 
/// This should generally called from within the work function at the end).

void TaskQueueFarm::notifyTaskDone(void)
{
  lock();
  tasksOutstanding--;
  pthread_cond_signal(&tasksUnfinished);
  unlock();
}


// =======================================================================================
/// @brief Wait until the farm has performed all current work

void TaskQueueFarm::waitOnEmptyFarm(void)
{
  lock();
  while(tasksOutstanding > 0)
    pthread_cond_wait(&tasksUnfinished, &mutex);
  unlock();
}


// =======================================================================================
/// @brief Provide a diagnostic page with a table about all the task queues
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

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

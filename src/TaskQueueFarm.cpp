// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class ..... is a template for real classes

#include "TaskQueueFarm.h"

// =======================================================================================
// Constructor

TaskQueueFarm::TaskQueueFarm(unsigned nQueues):
                                        nQ(nQueues), 
                                        tasksOutstanding(0u)
{
  if(pthread_cond_init(&tasksUnfinished, NULL))
    err(-1, "Couldn't initialize tasksUnfinished in TaskQueueFarm::TaskQueueFarm.");
  taskQueues = new TaskQueue*[nQ];
  assert(taskQueues);
  
  for(unsigned s=0; s<nQ; s++)
    taskQueues[s] = new TaskQueue(s);
}


// =======================================================================================
// Destructor

TaskQueueFarm::~TaskQueueFarm(void)
{
  for(int s=0; s<nQ; s++)
    taskQueues[s]->die();
  if(pthread_cond_destroy(&tasksUnfinished))
    err(-1, "Couldn't destroy tasksUnfinished in Scene::~Scene.");
  
  /*
  Need some synchronization here to know when it's safe to delete the threads
  for(int s=0; s<config.nSimThreads; s++)
    delete taskQueues[s];
  delete[] taskQueues;  
  */
}


// =======================================================================================
// Function to add work to one of the queues

void TaskQueueFarm::addTask(unsigned i, void (*work)(void*), void* arg)
{
  lock();
  tasksOutstanding++;
  taskQueues[i%nQ]->addTask(work, arg);
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

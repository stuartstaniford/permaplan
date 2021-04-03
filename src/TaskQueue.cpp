// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Task queue class for use in multi-threaded version.

#include "TaskQueue.h"

// =======================================================================================
// Constructor

TaskQueue::TaskQueue(void)
{
}


// =======================================================================================
// Destructor

TaskQueue::~TaskQueue(void)
{
}


// =======================================================================================
// This is the main loop of the thread servicing this TaskQueue.  When there's no work,
// it waits on a condition variable.  When there are tasks in the queue, it does them one
// by one.

void TaskQueue::workLoop(void)
{
}


// =======================================================================================
// Function called to add a task to this particular queue.

void TaskQueue::addTask(void)
{
}


// =======================================================================================

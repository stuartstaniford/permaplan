// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is an abstract interface for all parts of trees.

#include "TreePart.h"
#include <err.h>


// =======================================================================================
// Constructors.

TreePart::TreePart(void)
{
}


// =======================================================================================
// Destructor

TreePart::~TreePart(void)
{
}


        
// =======================================================================================
// Tell callers our name at runtime.

const char* TreePart::objectName(void)
{
  static char* name = (char*)"Abstract TreePart";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool TreePart::diagnosticHTML(HttpDebug* serv)
{
  
  serv->addResponseData("<tr><td>Abstract TreePart</td>");
  return true;
}


// =======================================================================================
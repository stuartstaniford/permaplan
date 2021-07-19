// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for use in rendering parts of buildings - specifically it renders a
// rectangular box with a gable roof above it.  Multiple of these can be superimposed
// to create approximations of many buildings.  Support for texturing is provided.

#include "Gable.h"
#include "MenuGablePanel.h"


// =======================================================================================
// Constructor

Gable::Gable(MenuGablePanel& gablePanel):
                              VisualObject(false)
{
}


// =======================================================================================
// Destructor

Gable::~Gable(void)
{
}


// =======================================================================================
/// @brief This is where the actual geometry of the gable is defined - we render it into a 
/// buffer on request
/// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).
/// @todo Stub only at present

bool Gable::bufferGeometryOfObject(TriangleBuffer* T)
{

  return true;
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer on a call to bufferGeometryToObject
///
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.
/// @todo Stub only at present

void Gable::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
  LogTriangleBufEstimates("Box TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief Decide if a ray touches us
/// @todo Stub only at present

bool Gable::matchRayToObject(vec3& position, vec3& direction, float& lambda)
{
  return false;
}

// =======================================================================================
/// @brief Function to validate the quadtree and all the objects.

#ifdef LOG_TREE_VALIDATION

void Gable::selfValidate(unsigned l)
{
  box->selfValidate(true);
}

#endif


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* Gable::objectName(void)
{
  static char* name = (char*)"Gable";
  return name;
}


// =======================================================================================
/// @brief Provide summary of this Gable.
/// 
/// We assume we are part of a table of visual objects and we just contribute one row
/// about this particular Gable.
/// @todo Stub only at present

bool Gable::diagnosticHTMLSummary(HttpDebug* serv)
{
  return true;
}


// =======================================================================================
// Function to check the validity of some JSON in an OLDF file that purportedly represents
// a gable.

using namespace rapidjson;

bool Gable::validateOLDF(Value& gableJsonObject)
{
  return true;  
}


// =======================================================================================

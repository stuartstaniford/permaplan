// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for use in rendering parts of buildings - specifically it renders a
// rectangular box with a gable roof above it.  Multiple of these can be superimposed
// to create approximations of many buildings.  Support for texturing is provided.

#include "Gable.h"
#include "MenuGablePanel.h"


// =======================================================================================
/// @brief Constructor for GableParamData (the core data required to specify a Gable, and
/// which gets defined or edited by a ManuGablePanel
///
/// This version defines default parameters).

GableParamData::GableParamData(void):
                                height(18.0f),
                                length(40.0f),
                                width(30.0f),
                                roofAngle(15.0f),
                                overhang(2.0f)
{
}


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
/// @todo End triangles

bool Gable::bufferGeometryOfObject(TriangleBuffer* T)
{
  unless(westWall.bufferGeometryOfElement(T, position))
    return false;
  unless(eastWall.bufferGeometryOfElement(T, position))
    return false;
  unless(northWall.bufferGeometryOfElement(T, position))
    return false;
  unless(southWall.bufferGeometryOfElement(T, position))
    return false;
  unless(westRoof.bufferGeometryOfElement(T, position))
    return false;
  unless(eastRoof.bufferGeometryOfElement(T, position))
    return false;

  return true;
}


// =======================================================================================
/// @brief Update the size of our axis-aligned bounding box.
///
/// We call each of our component BuildingRects and have them update our bounding box
/// with our position as their offset.
/// @todo - how to handle our orientation?

void Gable::updateBoundingBox(void)
{
  unless(box)
    box = new BoundingBox();
  
  westWall.updateBoundingBox(box, position);
  eastWall.updateBoundingBox(box, position);
  northWall.updateBoundingBox(box, position);
  southWall.updateBoundingBox(box, position);
  westRoof.updateBoundingBox(box, position);
  eastRoof.updateBoundingBox(box, position);
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
  westWall.triangleBufferSizes(vCount, iCount);
  vCount = 6*vCount + 2;  // 6 walls plus two vertices for the end cap triangles
  iCount = 6*iCount + 6;  // 6 walls plus two end cap triangles
  LogTriangleBufEstimates("Gable TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief Decide if a ray touches us.
/// 
/// This implementation works by checking each of our component BuildingRects
/// @param pos The vec3 for a point on the ray to be matched.
/// @param dir The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @todo End triangles

bool Gable::matchRayToObject(vec3& pos, vec3& dir, float& lambda)
{
  lambda = HUGE_VAL;
  float subLambda;
  bool matched = false;
  
  if(westWall.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  if(eastWall.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  if(northWall.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  if(southWall.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  if(westRoof.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  if(eastRoof.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  
  return matched;
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

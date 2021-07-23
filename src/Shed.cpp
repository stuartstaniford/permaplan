// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for rendering sheds - either standalone buildings, or (with an open
// side) parts of larger buildings.

#include "Shed.h"
#include "MenuShedPanel.h"


// =======================================================================================
/// @brief Constructor for ShedParamData (the core data required to specify a Shed, and
/// which gets defined or edited by a ManuShedPanel
///
/// This version defines default parameters).

ShedParamData::ShedParamData(void):
                              height(8.0f),
                              length(12.0f),
                              width(6.0f),
                              roofAngle(15.0f),
                              rightOverhang(1.0f),
                              leftOverhang(1.0f),
                              frontOverhang(1.0f),
                              backOverhang(1.0f)
{
}


// =======================================================================================
/// @brief Constructor for a shed formed from an insertion via a MenuShedPanel
///
/// This takes the insertion point and parameters specified in the MenuShedPanel and
/// instantiates the private state of the shed.  Note some things (such as the directions)
/// are not specified in the shedPanel, and will be set to default values here and changed
/// later (eg by clicking on the control arrows).

Shed::Shed(MenuShedPanel& shedPanel):
          VisualObject(false)
{
}


// =======================================================================================
/// @brief Destructor
///
/// Currently is default/empty.

Shed::~Shed(void)
{
}


// =======================================================================================
/// @brief Update the size of our axis-aligned bounding box.
///
/// We call each of our component BuildingRects and have them update our bounding box
/// with our position as their offset.
/// @todo - how to handle our orientation?

void Shed::updateBoundingBox(void)
{
  unless(box)
    box = new BoundingBox();
  
  westWall.updateBoundingBox(box, position);
  eastWall.updateBoundingBox(box, position);
  northWall.updateBoundingBox(box, position);
  southWall.updateBoundingBox(box, position);
  roof.updateBoundingBox(box, position);
}


// =======================================================================================
/// @brief This is where the actual geometry of the shed is defined - we render it into a 
/// buffer on request
/// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).
/// @todo Stub only at present

bool Shed::bufferGeometryOfObject(TriangleBuffer* T)
{
  if(westWallPresent)
    unless(westWall.bufferGeometryOfElement(T, position))
      return false;
  if(eastWallPresent)
    unless(eastWall.bufferGeometryOfElement(T, position))
      return false;
  unless(northWall.bufferGeometryOfElement(T, position))
    return false;
  unless(southWall.bufferGeometryOfElement(T, position))
    return false;
  unless(roof.bufferGeometryOfElement(T, position))
    return false;

  return true;
}


// =======================================================================================
/// @brief Decide if a ray touches us.
/// 
/// This implementation works by checking each of our component BuildingRects
/// @param pos The vec3 for a point on the ray to be matched.
/// @param dir The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.

bool Shed::matchRayToObject(vec3& pos, vec3& dir, float& lambda)
{
  lambda = HUGE_VAL;
  float subLambda;
  bool matched = false;
  
  if(westWallPresent && westWall.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  if(eastWallPresent && eastWall.matchRayToElement(pos, dir, subLambda, position))
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
  if(roof.matchRayToElement(pos, dir, subLambda, position))
   {
    matched = true;
    if(subLambda < lambda)
      lambda = subLambda;
   }
  
  return matched;
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer on a call to bufferGeometryToObject
///
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.
/// @todo Stub only at present

void Shed::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  unsigned vCountRect, iCountRect;
  vCount = 0u;
  iCount = 0u;
  
  northWall.triangleBufferSizes(vCountRect, iCountRect);
  vCount = 3*vCountRect + 2;  // 2 walls + roof + two vertices for the end cap triangles
  iCount = 3*iCountRect + 6;  // 2 walls + roof + two end cap triangles
  if(westWallPresent)
   {
    vCount += vCountRect;
    iCount += iCountRect;
   }
  if(eastWallPresent)
   {
    vCount += vCountRect;
    iCount += iCountRect;
   }
  
  LogTriangleBufEstimates("Shed TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* Shed::objectName(void)
{
  static char* name = (char*)"Shed";
  return name;
}


// =======================================================================================
/// @brief Static function to check a shed in OLDF
///
/// Tests the validity of some JSON in an OLDF file that purportedly represents a shed.
///
/// @returns True if the OLDF is valid, false otherwise.
/// @param shedJsonObject The rapidjson::Value for this piece of JSON for the shed.

using namespace rapidjson;

bool Shed::validateOLDF(Value& shedJsonObject)
{
  return true;  
}


// =======================================================================================

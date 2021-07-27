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
/// @brief Constructor

Gable::Gable(MenuGablePanel& gablePanel):
                              VisualObject(false)
{
  height    = gablePanel.height;
  length    = gablePanel.length;
  width     = gablePanel.width;
  roofAngle = gablePanel.roofAngle;
  overhang  = gablePanel.overhang;
  
  rebuildRects();
}




// =======================================================================================
/// @brief Destructor

Gable::~Gable(void)
{
}


// =======================================================================================
/// @brief Function to rebuild all our component BuildingRect walls/rooves.
/// 
/// This is called in the beginning (from the constructor), or after our parameters 
/// have changed.

void Gable::rebuildRects(void)
{
  // Preliminaries
  float sinAngle = sinf(angleFromNorth);
  float cosAngle = cosf(angleFromNorth);
  
  // The west wall (when we aren't rotated).
  // sides[0] runs along the bottom of the wall, sides[1] up the SW corner of building
  glm_vec3_copy(zeroVec, westWall.relativePos);
  westWall.sides[0][0] = length*sinAngle;
  westWall.sides[0][1] = length*cosAngle;
  westWall.sides[0][2] = 0.0f;
  westWall.sides[1][0] = 0.0f;
  westWall.sides[1][1] = 0.0f;
  westWall.sides[1][2] = height;
  westWall.normForward = false;
  
  // The east wall (when we aren't rotated).
  // Almost the same as the west wall, but offset, and norm in the opposite direction.
  memcpy((BuildRectData*)&eastWall, (BuildRectData*)&westWall, sizeof(BuildRectData));
  eastWall.normForward = true;
  eastWall.relativePos[0] = width*cosAngle;
  eastWall.relativePos[1] = width*sinAngle;
  
  // The south wall (when we aren't rotated).
  // sides[0] runs along the bottom of the wall, sides[1] up the SW corner of building
  glm_vec3_copy(zeroVec, southWall.relativePos);
  glm_vec3_copy(eastWall.relativePos, southWall.sides[0]);
  glm_vec3_copy(eastWall.sides[1], southWall.sides[1]);
  southWall.normForward = true;
  
  // The north wall (when we aren't rotated).
  // Almost the same as the south wall, but offset, and norm in the opposite direction.
  memcpy((BuildRectData*)&northWall, (BuildRectData*)&southWall, sizeof(BuildRectData));
  northWall.normForward = false;
  glm_vec3_add(southWall.relativePos, westWall.sides[0], northWall.relativePos);  
 
  // Ok, walls done, now set up for the rooves
  float tanRoofAngle  = tanf(roofAngle);
  float roofDip       = tanRoofAngle*overhang;
  roofRise            = tanRoofAngle*(width/2.0f);
  
  // West facing roof - sides[0] along the west eave, sides[1] up the sloping south side
  westRoof.relativePos[0] = -overhang;
  westRoof.relativePos[1] = -overhang;
  westRoof.relativePos[2] = height - roofDip;
  westRoof.sides[0][0] = 0.0f;
  westRoof.sides[0][1] = length + 2.0f*overhang;
  westRoof.sides[0][2] = 0.0f;
  westRoof.sides[1][0] = 0.0f;
  westRoof.sides[1][1] = width/2.0f+overhang;
  westRoof.sides[1][2] = roofDip + roofRise;
  westRoof.normForward = false;

  // West facing roof - sides[0] along the east eave, sides[1] up the sloping north side
  westRoof.relativePos[0] = width + overhang;
  westRoof.relativePos[1] = -overhang;
  westRoof.relativePos[2] = height - roofDip;
  glm_vec3_copy(westRoof.sides[0], eastRoof.sides[0]);
  westRoof.sides[1][0] = 0.0f;
  westRoof.sides[1][1] = -(width/2.0f+overhang);
  westRoof.sides[1][2] = roofDip + roofRise;
  westRoof.normForward = true;

  updateBoundingBox();
}


// =======================================================================================
/// @brief This is where the actual geometry of the gable is defined - we render it into a 
/// buffer on request
/// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

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

  unless(addEndTrianglesToBuffer(T))
    return false;
  
  return true;
}


// =======================================================================================
/// @brief After buffering all the BuildingRect pieces, add the triangles at the top of
/// the north and south wall.
/// 
/// This has to do some tricky buffer math.  Only C programmers allowed in this function 
/// :-). The south triangle reuses the upper two vertices of the south wall, as well as 
/// a newly added vertex.  Similarly the north triangle.  Consult 
/// Gable::bufferGeometryOfObject, Gable::rebuildRects and 
/// BuildingRect::bufferGeometryOfElement to figure out which vertex is which.  
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

bool Gable::addEndTrianglesToBuffer(TriangleBuffer* T)
{
  Vertex*   vertices;
  unsigned* indices;
  unsigned  vOffset;

  // Get the space for the extra triangles
  unless(T->requestSpace(&vertices, &indices, vOffset, 2, 6))
   {
    LogTriangleBufferErrs("Gable::addEndTrianglesToBuffer TriangleBuffer request"
                                                    " for 2,6 failed at %u.\n", vOffset);
    return false;
   }

  // South triangle (ie the one on top of the south wall).
  // sides[0] runs along the bottom of the wall, sides[1] up the SW corner of building
  vec3 point;
  vec3 scaledVec;
  glm_vec3_add(southWall.relativePos, position, point);
  glm_vec3_scale_as(southWall.sides[0], width/2.0f, scaledVec);
  glm_vec3_add(point, scaledVec, point); // Point at middle bottom of south wall.
  glm_vec3_scale_as(southWall.sides[1], height+roofRise, scaledVec);
  glm_vec3_add(point, scaledVec, point); // Point now correctly in position.
  //Now we copy top left vertex of south wall to new vertex
  memcpy(vertices, vertices - 10, sizeof(Vertex));
  vertices->setPosition(point);
  
  // Now indices of the south triangle
  indices[0] = vOffset-10;  // top left of southwall
  indices[1] = vOffset-10;  // top right of southwall
  indices[2] = vOffset;     // newly created vertex at top of south triangle
  
  // Now north triangle (ie the one on top of the north wall).
  glm_vec3_add(point, westWall.sides[0], point); // Move point parallel to westwall.
  //Now we copy top left vertex of north wall to new vertex
  memcpy(vertices+1, vertices - 14, sizeof(Vertex));
  (vertices+1)->setPosition(point);
  
  // Now indices of the north triangle
  indices[3] = vOffset-13;  // top right of northwall
  indices[4] = vOffset-14;  // top left of northwall
  indices[5] = vOffset+1;     // newly created vertex at top of north triangle
  
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

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

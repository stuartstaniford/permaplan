// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for rendering sheds - either standalone buildings, or (with an open
// side) parts of larger buildings.

#include "Shed.h"
#include "MenuShedPanel.h"
#include "HttpDebug.h"


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
  rebuildRects();
}


// =======================================================================================
/// @brief Function to rebuild all our component BuildingRect walls/rooves.
/// 
/// This is called in the beginning (from the constructor), or after our parameters 
/// have changed.

void Shed::rebuildRects(void)
{
  // Preliminaries
  float sinAngle      = sinf(angleFromNorth);
  float cosAngle      = cosf(angleFromNorth);
  float tanRoofAngle  = tanf(roofAngle*M_PI/180.0f);
  float shortHeight   = height - tanRoofAngle*width;
  
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
  // Almost the same as the west wall, but offset, norm in the opposite direction,
  // and lower according to the roof angle).
  memcpy((BuildRectData*)&eastWall, (BuildRectData*)&westWall, sizeof(BuildRectData));
  eastWall.normForward    = true;
  eastWall.relativePos[0] = width*cosAngle;
  eastWall.relativePos[1] = width*sinAngle;
  eastWall.sides[1][2]    = shortHeight;
  
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

  // Ok, walls done, now set up for the roof
  float roofDip       = tanRoofAngle*leftOverhang;
  
  // sides[0] along the west eave, sides[1] down the sloping south side
  roof.relativePos[0] = -leftOverhang;
  roof.relativePos[1] = -frontOverhang;
  roof.relativePos[2] = height - roofDip;
  roof.sides[0][0] = 0.0f;
  roof.sides[0][1] = length + frontOverhang + backOverhang;
  roof.sides[0][2] = 0.0f;
  roof.sides[1][0] = 0.0f;
  roof.sides[1][1] = width + leftOverhang + rightOverhang;
  roof.sides[1][2] = -tanRoofAngle*roof.sides[1][1];
  roof.normForward = false;
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
/// @todo End triangles

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
/// @todo End triangles

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
  /*
  if(buildings.HasMember("sheds"))
   {
    shedsPresent = true;
    unless(buildings["sheds"].IsArray())
     {
      LogOLDFValidity("sheds is not an array in OLDF file %s\n", config.designFileName);
      return false;     
     }
    int N = buildings["sheds"].Size();
    for(int i=0; i<N; i++)
      retVal &= Shed::validateOLDF(buildings["sheds"][i]);
   }
  */
  
  return true;  
}


// =======================================================================================
/// @brief Provide an HTML table summarizing these parameters.
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool ShedParamData::httPrintShedParamTable(HttpDebug* serv)
{
  httPrintf("<center>");
  unless(serv->startTable((char*)"Gable_Params"))
    return false;

  httPrintf("<tr><th>Parameter Name</td><th>Value</th></tr>");
  httPrintf("<tr><td>Height</td><td>%.2f</td></tr>", height);
  httPrintf("<tr><td>Length</td><td>%.2f</td></tr>", length);
  httPrintf("<tr><td>Width</td><td>%.2f</td></tr>", width);
  httPrintf("<tr><td>Roof angle</td><td>%.2f</td></tr>", roofAngle);
  httPrintf("<tr><td>Right Overhang</td><td>%.2f</td></tr>", rightOverhang);
  httPrintf("<tr><td>Left Overhang</td><td>%.2f</td></tr>", leftOverhang);
  httPrintf("<tr><td>FrontOverhang</td><td>%.2f</td></tr>", frontOverhang);
  httPrintf("<tr><td>BackOverhang</td><td>%.2f</td></tr>", backOverhang);
  httPrintf("<tr><td>Angle from North</td><td>%.2f</td></tr>", angleFromNorth);
  httPrintf("<tr><td>Position</td><td>%.1f, %.1f, %.1f</td></tr>", position[0], 
                                                                position[1], position[2]);
  httPrintf("</table></center>");
  return true;
}


// =======================================================================================
/// @brief Provides access to the diagnostic HTTP server for the diagnosticHTML page of 
/// this Shed via it's objIndex.
///
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Shed::diagnosticHTML(HttpDebug* serv)
{
  // Page header
  char title[64];
  snprintf(title, 63, "Detail Page for Shed (Object %d).", objIndex);
  unless(serv->startResponsePage(title))
    return false;

  // Information about the controlling parameters of this Gable object
  unless(httPrintShedParamTable(serv))
    return false;  

  // Details of our bounding box
  unless(box->diagnosticHTML(serv))
    return false;

  // Provide details of all our individual BuildingRect components
  unless(serv->newSection("Wall and Roof Sections"))
    return false;
  unless(westWall.httPrintTableSummary(serv, (char*)"West Wall"))
    return false;
  unless(eastWall.httPrintTableSummary(serv, (char*)"East Wall"))
    return false;
  unless(southWall.httPrintTableSummary(serv, (char*)"South Wall"))
    return false;
  unless(northWall.httPrintTableSummary(serv, (char*)"North Wall"))
    return false;
  unless(roof.httPrintTableSummary(serv, (char*)"Roof"))
    return false;
  
  // Page closing
  unless(serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================

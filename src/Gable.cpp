// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for use in rendering parts of buildings - specifically it renders a
// rectangular box with a gable roof above it.  Multiple of these can be superimposed
// to create approximations of many buildings.  Support for texturing is provided.

#include "Gable.h"
#include "MenuGablePanel.h"
#include "BuildingRect.h"


// =======================================================================================
/// @brief Constructor for GableParamData (the core data required to specify a Gable, and
/// which gets defined or edited by a ManuGablePanel
///
/// This version defines default parameters).

GableParamData::GableParamData(void):
                                height(18.0f),
                                length(40.0f),
                                width(30.0f),
                                roofAngle(30.0f),
                                overhang(2.0f)
{
}


// =======================================================================================
/// @brief Constructor

Gable::Gable(MenuGablePanel& gablePanel):
                              BuildingAssembly(6, 2)
{
  height          = gablePanel.height;
  length          = gablePanel.length;
  width           = gablePanel.width;
  roofAngle       = gablePanel.roofAngle;
  overhang        = gablePanel.overhang;
  angleFromNorth  = gablePanel.angleFromNorth;
  setPosition(gablePanel.position);
  
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
  BuildingRect& westWall = rects[0];
  glm_vec3_copy(zeroVec, westWall.relativePos);
  westWall.sides[0][0] = length*sinAngle;
  westWall.sides[0][1] = length*cosAngle;
  westWall.sides[0][2] = 0.0f;
  westWall.sides[1][0] = 0.0f;
  westWall.sides[1][1] = 0.0f;
  westWall.sides[1][2] = height;
  westWall.normForward = false;
  westWall.setContainingIndex(objIndex);
  
  // The east wall (when we aren't rotated).
  // Almost the same as the west wall, but offset, and norm in the opposite direction.
  BuildingRect& eastWall = rects[1];
  memcpy((BuildRectData*)&eastWall, (BuildRectData*)&westWall, sizeof(BuildRectData));
  eastWall.normForward = true;
  eastWall.relativePos[0] = width*cosAngle;
  eastWall.relativePos[1] = width*sinAngle;
  eastWall.setContainingIndex(objIndex);
  
  // The south wall (when we aren't rotated).
  // sides[0] runs along the bottom of the wall, sides[1] up the SW corner of building
  BuildingRect& southWall = rects[2];
  glm_vec3_copy(zeroVec, southWall.relativePos);
  glm_vec3_copy(eastWall.relativePos, southWall.sides[0]);
  glm_vec3_copy(eastWall.sides[1], southWall.sides[1]);
  southWall.normForward = true;
  southWall.setContainingIndex(objIndex);

  // The south wall triangular extension  
  // Note the order of the vertices - 0 is relativePos, 1 is relativePos + sides[0],
  // 2 is relativePos + sides[1], 3 is relativePos + sides[0] + sides[1]
  RectExtension& southTriangle = exts[0];
  southTriangle.rectIndex = 2u; // for southWall
  southTriangle.vertexIndices[0] = 2; // top south west corner of walls
  southTriangle.vertexIndices[1] = 3; // top south east corner of walls
  vec3 point;
  vec3 scaledVec;
  glm_vec3_scale_as(southWall.sides[0], width/2.0f, scaledVec);
  glm_vec3_add(southWall.relativePos, scaledVec, point); // Point at middle bottom of south wall.
  glm_vec3_scale_as(southWall.sides[1], height+roofRise, scaledVec);
  glm_vec3_add(point, scaledVec, southTriangle.extensionPoint);
  
  // The north wall (when we aren't rotated).
  // Almost the same as the south wall, but offset, and norm in the opposite direction.
  BuildingRect& northWall = rects[3];
  memcpy((BuildRectData*)&northWall, (BuildRectData*)&southWall, sizeof(BuildRectData));
  northWall.normForward = false;
  glm_vec3_add(southWall.relativePos, westWall.sides[0], northWall.relativePos);  
  northWall.setContainingIndex(objIndex);
 
  // The north wall triangular extension  
  RectExtension& northTriangle = exts[1];
  northTriangle.rectIndex = 3u; // for northWall
  northTriangle.vertexIndices[0] = 3; // top north east corner of walls
  northTriangle.vertexIndices[1] = 2; // top north west corner of walls
  glm_vec3_add(southTriangle.extensionPoint, westWall.sides[0], 
                            northTriangle.extensionPoint);  // move parallel to westWall  
  
  // Ok, walls done, now set up for the rooves
  float tanRoofAngle  = tanf(roofAngle*M_PI/180.0f);
  float roofDip       = tanRoofAngle*overhang;
  roofRise            = tanRoofAngle*(width/2.0f);
  
  // West facing roof - sides[0] along the west eave, sides[1] up the sloping south side
  BuildingRect& westRoof = rects[4];
  westRoof.relativePos[0] = -overhang;
  westRoof.relativePos[1] = -overhang;
  westRoof.relativePos[2] = height - roofDip;
  westRoof.sides[0][0] = 0.0f;
  westRoof.sides[0][1] = length + 2.0f*overhang;
  westRoof.sides[0][2] = 0.0f;
  westRoof.sides[1][0] = width/2.0f+overhang;
  westRoof.sides[1][1] = 0.0;
  westRoof.sides[1][2] = roofDip + roofRise;
  westRoof.normForward = false;
  westRoof.setContainingIndex(objIndex);

  // East facing roof - sides[0] along the east eave, sides[1] up the sloping north side
  BuildingRect& eastRoof = rects[5];
  eastRoof.relativePos[0] = width + overhang;
  eastRoof.relativePos[1] = -overhang;
  eastRoof.relativePos[2] = height - roofDip;
  glm_vec3_copy(westRoof.sides[0], eastRoof.sides[0]);
  eastRoof.sides[1][0] = -(width/2.0f+overhang);
  eastRoof.sides[1][1] = 0.0f;
  eastRoof.sides[1][2] = roofDip + roofRise;
  eastRoof.normForward = true;
  eastRoof.setContainingIndex(objIndex);

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
  LogBuildingBuffer("Gable buffered: Height: %.2f; Length: %.2f; Width: %.2f, "
                    "Roof Angle: %.1f; Overhang: %.2f, Position: [%.1f, %.1f, %.1f] "
                    "Orientation: %.1f.\n", height, length, width, roofAngle, overhang,
                    position[0], position[1], position[2], angleFromNorth);

  unless(BuildingAssembly::bufferGeometryOfObject(T))
    return false;

  return true;
}


// =======================================================================================
/// @brief Get our position vector.
///
/// @returns A float* which points to the vec3 of our position.

float* Gable::getPosition(void)
{
  return position;  
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* Gable::objectName(void)
{
  static char* name = (char*)"Gable";
  return name;
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
/// @brief  Provide one row of a table of visual objects about this particular 
/// Gable.
///
/// The type of visual object is in the first column (with a link to the detail page
/// that is provided by HeightMarker::diagnosticHTML), and details are provided in the 
/// second column.  
/// @returns True if the object was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Gable::diagnosticHTMLSummary(HttpDebug* serv)
{
  httPrintf("<tr><td><a href=\"/object/%d\">Gable</a></td>", objIndex);
  unless(httPrintPositionCell(serv))
    return false;
  
  return true;
}


// =======================================================================================
/// @brief Write out one Gable's data to an OLDF JSON file.
///
/// Note we will typically be writing out into the middle of a file, the overall 
/// organization of the file being orchestrated by our call-chain.  We just know about
/// recording ourself in the proper OLDF format.
/// @param file The C-style FILE* pointer for the open file to write to.
/// @param indent The C-string used for one unit of indentation of the JSON.

void GableParamData::writeToOLDF(FILE* file, char* indent)
{
  fprintf(file, "%s {\n", indent);

  // position
  fprintf(file, "%s%s\"position\": [%f, %f, %f],\n", indent, indent, position[0], 
                                                                  position[1], position[2]);
  
  // orientation
  fprintf(file, "%s%s\"angleFromNorth\": %.1f,\n", indent, indent, angleFromNorth);

  // height
  fprintf(file, "%s%s//\"height\": %.1f,\n", indent, indent, height);

  // length
  fprintf(file, "%s%s//\"length\": %.1f,\n", indent, indent, length);

  // width
  fprintf(file, "%s%s//\"width\": %.1f,\n", indent, indent, width);

  // roofAngle
  fprintf(file, "%s%s//\"roofAngle\": %.1f,\n", indent, indent, roofAngle);

  // overhang
  fprintf(file, "%s%s//\"height\": %.1f,\n", indent, indent, overhang);

  fprintf(file, "%s }", indent);
}


// =======================================================================================
/// @brief Provide an HTML table summarizing these parameters.
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool GableParamData::httPrintGableParamTable(HttpDebug* serv)
{
  httPrintf("<center>");
  unless(serv->startTable((char*)"Gable_Params"))
    return false;

  httPrintf("<tr><th>Parameter Name</td><th>Value</th></tr>");
  httPrintf("<tr><td>Height</td><td>%.2f</td></tr>", height);
  httPrintf("<tr><td>Length</td><td>%.2f</td></tr>", length);
  httPrintf("<tr><td>Width</td><td>%.2f</td></tr>", width);
  httPrintf("<tr><td>Roof angle</td><td>%.2f</td></tr>", roofAngle);
  httPrintf("<tr><td>Overhang</td><td>%.2f</td></tr>", overhang);
  httPrintf("<tr><td>Angle from North</td><td>%.2f</td></tr>", angleFromNorth);
  httPrintf("<tr><td>Position</td><td>%.1f, %.1f, %.1f</td></tr>", position[0], 
                                                                position[1], position[2]);
  httPrintf("</table></center>");
  return true;
}


// =======================================================================================
/// @brief Provides access to the diagnostic HTTP server for the diagnosticHTML page of 
/// this Gable via it's objIndex.
///
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Gable::diagnosticHTML(HttpDebug* serv)
{
  // Page header
  char title[64];
  snprintf(title, 63, "Detail Page for Gable (Object %d).", objIndex);
  unless(serv->startResponsePage(title))
    return false;

  // Information about the controlling parameters of this Gable object
  unless(httPrintGableParamTable(serv))
    return false;  

  // Details of our bounding box
  unless(box->diagnosticHTML(serv))
    return false;

  // Provide details of all our individual BuildingRect components
  unless(serv->newSection("Wall and Roof Sections"))
    return false;
  unless(rects[0].httPrintTableSummary(serv, (char*)"West Wall"))
    return false;
  unless(rects[1].httPrintTableSummary(serv, (char*)"East Wall"))
    return false;
  unless(rects[2].httPrintTableSummary(serv, (char*)"South Wall"))
    return false;
  unless(rects[3].httPrintTableSummary(serv, (char*)"North Wall"))
    return false;
  unless(rects[4].httPrintTableSummary(serv, (char*)"West Roof"))
    return false;
  unless(rects[5].httPrintTableSummary(serv, (char*)"East Roof"))
    return false;
  
  // Page closing
  unless(serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================

// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Class which loads a series of records of surface height at various locations
// and produces a tiling of the surface, as well as being able to texture it
// with a provided map.

#include "LandSurface.h"
#include "Camera.h"
#include "PmodDesign.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// =======================================================================================
/// @brief Constructor which sets up the surface as specified in the design file.
///
/// Note, we are called from the Scene constructor before the quadtree exists.  So we 
/// do a certain amount of setup (which is required before quadtree initialization, but 
/// then some additional things are done later via a call to LandSurface::bufferGeometry
/// after the quadtree is present (but still during the Scene constructor).

LandSurface::LandSurface(void):
                            rect(NULL),
                            qtree(NULL),
                            tbuf(NULL),
                            locationCount(0u),
                            heightLocations(),
                            heightLabels(),
                            inFitMode(false),
                            initialHeightCount(0u),
                            initialHeightIndex(0u)
#ifdef VISUALIZE_FITTING
                            , fitTBuf(NULL)
#endif
{
  using namespace rapidjson;
  const PmodConfig& config = PmodConfig::getConfig();
  PmodDesign& design = PmodDesign::getDesign();

  // Note that syntax of the landSurface object has already been checked in PmodDesign,
  // so we can rely on the structure being correct OLDF here.
  Value& LsJson = design.doc["landSurface"];
  unless(LsJson.HasMember("texture") && LsJson["texture"].IsObject())
   {
    //XX need to handle case of no texture.
    err(-1, "Cannot handle no texture correctly in file %s\n", config.designFileName);
   }
  float width = LsJson["texture"]["width"].GetFloat();
  const char* url = LsJson["texture"]["url"].GetString();
  unless(strlen(url) > 6 && strncmp(url, "file:/", 6) == 0)
   {
    //XX need to handle other kinds of urls via an http client library
    err(-1, "Only file:/ urls handled in texture url %s in file %s\n", url, config.designFileName);
   }
  
  rectOffset[0] = LsJson["texture"]["offset"][0].GetFloat();
  rectOffset[1] = LsJson["texture"]["offset"][1].GetFloat();

  rect = new TexturedRect(url+6, width, 0.0f);

  if(LsJson.HasMember("altitudes"))
   {
    unless(LsJson["altitudes"].IsArray())
      err(-1, "Altitudes is not array in file %s\n", config.designFileName);
    unless((initialHeightCount = LsJson["altitudes"].Size()) > 0)
     {
      warn("Empty altitudes array in JSON file.");
     }
    altitudeArray = &LsJson["altitudes"];
   }
  if(checkGLError(stderr, "LandSurface::LandSurface"))
    exit(-1);
}


// =======================================================================================
/// @brief Late constructor stuff that can't be done until after the Quadtree is 
/// initialized.
/// @param q Pointer to the Quadtree.

void LandSurface::bufferGeometry(Quadtree* q)
{
  if(q)
    qtree = q;

  if(tbuf)
    delete tbuf;
  tbuf = new TriangleBuffer(2*qtree->landVBOSize/3, qtree->landVBOSize, (char*)"land tBuf");
  if(!tbuf)
    err(-1, "Can't allocate memory in __func__\n");
  qtree->bufferLandSurface(tbuf);
  tbuf->sendToGPU(GL_STATIC_DRAW);
}


// =======================================================================================
/// @brief Destructor

LandSurface::~LandSurface(void)
{
  if(rect)
    delete rect;
  if(tbuf)
    delete tbuf;
}


// =======================================================================================
/// @brief Extract further height locations from the oldf design file. 
///  
/// Note that OLDF file syntax is checked in PmodDesign, so we can safely assume it's 
/// correct here.
/// @param location A vec3 which will be filled out with the height/position location.
/// The x,y are in spaceUnits relative to the referencePoint.  The z is absolute height
/// above MSL in spaceUnits.
/// @param label.  A reference for a C-string pointer which will be set to the annotation
/// label of this particular altitude input.

bool LandSurface::nextInitialHeightLocation(vec3 location, const char*& label)
{
  using namespace rapidjson;

  if(initialHeightIndex >= initialHeightCount)
    return false;

  Value& row = (*altitudeArray)[initialHeightIndex];
  for(int i=0; i<3; i++)
    location[i] = row[i].GetFloat();
  label = row[3].GetString();
  initialHeightIndex++;
  return true;
}


// =======================================================================================
/// @brief Highlight one qtree node.  
/// 
/// If color is NULL, unhighlights.  Assumes the correct VAO/VBO are already bound
/// @param targetNode The quadtree node to illuminate
/// @param color A vec4 representing the color to use
/// @param accent ??? Think this is dated now.
/// @todo This function is disabled pending reimplementation in the TriangleBuffer case.
/// Seems like it would still be useful though when we have to debug something quadtree
/// related.

void LandSurface::highlightNode(Quadtree* targetNode, vec4& color, float accent)
{
/*
#ifdef USE_TRIANGLE_BUFFER
  
#else
  //printf("Size: %u\tOffset: %u\n", targetNode->landVBOSize, targetNode->bufferOffset);
  Vertex* buf = (Vertex*)glMapBufferRange(GL_ARRAY_BUFFER,
                        (targetNode->bufferOffset)*sizeof(Vertex),
                        (targetNode->landVBOSize)*sizeof(Vertex), GL_MAP_WRITE_BIT);
  if(checkGLError(stderr, "LandSurface::highlightNode:glMapBufferRange") || !buf)
    exit(-1);
  Vertex* b;
  int i;
  if(accent > 0.0f)
    shader.setUniform("accentColor", color);
  for(b = buf, i=0; i<targetNode->landVBOSize;i++,b++)
      b->accent = accent;

  glUnmapBuffer(GL_ARRAY_BUFFER);
  if(checkGLError(stderr, "LandSurface::highlightNode"))
    exit(-1);
#endif
*/
}


// =======================================================================================
/// @brief Reconfigure the land surface when we are told about a new observation that 
/// is to be added to our state.
/// This is the most important function, which co-ordinates which kind of model to use
/// depending on how much height data we have to work with.
/// @param hM The new heightMarker.
/// @todo We don't currently detect if a new heightmarker is unacceptable - eg second 
/// one is exactly the same as the first.  Need to do more error checking, and have a 
/// way to report that the heightMarker should be rejected and deleted.
/// @todo We can't handle large numbers of observations, which should cause us to split
/// the BezierPatch.
extern vec3 zAxis;

void LandSurface::newLandHeight(HeightMarker* hM)
{
  const PmodConfig& config = PmodConfig::getConfig();
  heightLocations.push_back(hM->position);
  heightLabels.push_back(hM->label);
  locationCount++;
  vec3 plane;
  
  LogLandHeights("New height recorded at %.1f, %.1f, %.1f\n",
                      hM->location[0], hM->location[1], hM->location[2]);
  
  if(locationCount == 1)
   {
    // level surface at the new height
    plane[0] = 0.0f; // co-efficient of x
    plane[1] = 0.0f; // co-efficient of y
    plane[2] = hM->position[2]; // intersection with the z-axis
   }
  else if(config.levelPlane)
   {
    return;
   }
  else if(locationCount == 2)
   {
    // includes both points, sloping up along the line between them.
    //XXX not checking that the two points are identical
    if(heightLocations[1][0] == heightLocations[0][0])
      plane[0] = 0.0f;
    else
      plane[0] = (heightLocations[1][2] - heightLocations[0][2])/
                    (heightLocations[1][0] - heightLocations[0][0]);
    if(heightLocations[1][1] == heightLocations[0][1])
      plane[1] = 0.0f;
    else
      plane[1] = (heightLocations[1][2] - heightLocations[0][2])/
                    (heightLocations[1][1] - heightLocations[0][1]);
    plane[2] = heightLocations[0][2] - plane[0]*heightLocations[0][0]
                    - plane[1]*heightLocations[0][1];
   }
  else if(locationCount == 3)
   {
    // plane through all three points
    //XXX not checking that the triangle might be degenerate
    vec3 diff1, diff2, norm;
    glm_vec3_sub(heightLocations[0], heightLocations[1], diff1);
    glm_vec3_sub(heightLocations[0], heightLocations[2], diff2);
    glm_vec3_cross(diff1, diff2, norm);
    // The equation of a plane is ax+by+cz = d, where [a,b,c]
    // is the normal vector to the plane.
    float d = glm_vec3_dot(heightLocations[0], norm);
    plane[0] = -norm[0]/norm[2];
    plane[1] = -norm[1]/norm[2];
    plane[2] = d/norm[2];
   }
  
  if(locationCount <= 3) // done already
   {
    qtree->redoLandPlanar(plane);
    bufferGeometry(NULL);
    return;
   }

  if(locationCount == 4)
   {
    // Toss the plane, and fire up a single Bezier patch at the root of the quadtree
    qtree->stripSurface();
    BezierPatch* bez = new BezierPatch(qtree, 10); //XX - hardcoded gridpoints
    qtree->surface = bez;
    //bez->randomFit(heightLocations);
    bez->assertCopyVer();
    if(config.bezReadFileName) // XX note this should probably be folded into the OLDF file
     {
      // Note this is very expensive in a frame, but it only happens very near startup.
      FILE* readFile = fopen(config.bezReadFileName, "r");
      if(!readFile)
        err(-1, "Couldn't open file %s\n", config.bezReadFileName);
      bez->readControlPointsFromFile(readFile);
      fclose(readFile);
     }
    else
      bez->levelFit(heightLocations);

    redoBezierLandSurface(bez);
    inFitMode = true;
    return;
   }

  if(locationCount <= 30)
   {
    // Iterate on our quadtree root Bezier patch
    inFitMode = true;
    return;
   }
  else
   {
    // Kriging

   }
  
}


// =======================================================================================
/// @brief Redo the triangle buffer holding the BezierPatch landsurface.
/// 
/// This is done after improving the fit.  
/// @param bez Pointer to the BezierPatch.
/// @todo Note the approach here of tossing the whole thing and start over seems a bit 
/// costly and a more efficient approach might need to be developed if this is a 
/// bottleneck

void LandSurface::redoBezierLandSurface(BezierPatch* bez)
{
  unsigned vCount, iCount;

  bez->assertCopyVer();

  bez->triangleBufferSizes(vCount, iCount);
  recycleTriangleBuffer(tbuf, vCount, iCount, (char*)"bez tbuf");
  bez->bufferGeometryOfObject(tbuf);
  tbuf->sendToGPU(GL_STATIC_DRAW);

#ifdef VISUALIZE_FITTING
  DisplayList* D = bez->newUVLocationList();
  bez->addControlPointsToDisplayList(D);
  bez->addControlGradientsToDisplayList(D);
  vCount = 0u;
  iCount = 0u;
  D->triangleBufferSizes(vCount, iCount);
  recycleTriangleBuffer(fitTBuf, vCount, iCount, (char*)"fit tbuf");
  D->bufferGeometry(fitTBuf);
  fitTBuf->sendToGPU(GL_STATIC_DRAW);
  delete D;
#endif
  bez->assertCopyVer();
  qtree->adjustAltitudes(bez);
}


// =======================================================================================
/// @brief Render our part of the scene.
/// 
/// This is called from Scene::draw().
/// @param camera - a reference to the Camera object.

void LandSurface::draw(Camera& camera)
{
  // Highlight where the camera points at
/*  vec3 pos, dir;
 vec4 yellowAccentColor = {0.9f, 0.9f, 0.0f, 1.0f};
  float lambda;
  camera.copyDirection(pos, dir);
  Quadtree* targetNode = qtree->matchRay(pos, dir, lambda);
  if(targetNode)
    highlightNode(targetNode, yellowAccentColor, 0.3f);
*/
  
  // Draw the main surface
  if(inFitMode)
   {
    BezierPatch* bez = (BezierPatch*)qtree->surface;
    if(bez->improveFit(heightLocations))
      redoBezierLandSurface(bez);
    else
      inFitMode = false;
   }
  rect->texture.bind(0, "earthTexture");
  tbuf->draw(Lighted, NULL);
#ifdef VISUALIZE_FITTING
  if(fitTBuf)
    fitTBuf->draw(NoTexColor, NULL);
#endif

  if(checkGLError(stderr, "LandSurface::draw"))
    exit(-1);
}


// =======================================================================================
/// @brief Write out the LandSurface data to a file in OLDF JSON format.
/// @param file C FILE* pointer to the open file to write too.
/// @param indent Character sequence for use in indenting the JSON.

void LandSurface::writeOLDFSection(FILE* file, char* indent)
{
  // Open the object
  fprintf(file, "%s\"landSurface\":\n", indent);
  fprintf(file, "%s {\n", indent);

  // Texture
  fprintf(file, "%s%s\"texture\":\n%s%s {\n", indent, indent, indent, indent);
  fprintf(file, "%s%s%s\"url\": \"file:/%s\",\n", indent, indent, indent,
                                                            rect->texture.textureFileName);
  fprintf(file, "%s%s%s\"width\": %f,\n", indent, indent, indent, rect->width);
  fprintf(file, "%s%s%s\"offset\": [%f,%f]\n", indent, indent, indent,
                                                            rectOffset[0], rectOffset[1]);
  fprintf(file, "%s%s },\n", indent, indent);

  // altitudes
  fprintf(file, "%s%s\"altitudes\":\n%s%s [\n", indent, indent, indent, indent);
  for(int i=0; i < locationCount; i++)
   {
    fprintf(file, "%s%s%s[%f, %f, %f, \"%s\"]", indent, indent, indent,
                heightLocations[i][0], heightLocations[i][1], heightLocations[i][2],
                heightLabels[i]);
    if(i<locationCount-1)
      fprintf(file, ",\n");
    else
      fprintf(file, "\n");
   }   
  fprintf(file, "%s%s ]\n", indent, indent);

  // Close out the landSurface object
  fprintf(file, "%s },\n", indent);
}


// =======================================================================================
/// @brief Provide a diagnostic page about the land surface
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool LandSurface::diagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Land");
  
  httPrintf("<center>\n");
  serv->startTable();
  httPrintf("<tr><th>Heightmarker index</th><th>X</th>");
  httPrintf("<th>Y</th><th>Z</th></tr>\n");
  
  for(int i=0; i< locationCount; i++)
   {
    httPrintf("<tr><th>%d</td>", i);
    for(int j=0; j<3; j++)
      httPrintf("<td>%.1f</td>", heightLocations[i][j]);
    httPrintf("</tr>\n");
   }
  
  httPrintf("</table></center><hr>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================

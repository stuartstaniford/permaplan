// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Class which loads a series of records of surface height at various locations
// and produces a tiling of the surface, as well as being able to texture it
// with a provided map.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "LandSurface.h"


// =======================================================================================
// Constructor which sets up the surface as specified in the design file

LandSurface::LandSurface(Shader& S, PmodDesign& D):
                            rect(NULL),
                            qtree(NULL),
                            shader(S),
                            design(D),
                            tbuf(NULL),
                            locationCount(0u),
                            heightLocations(),
                            inFitMode(false)
#ifdef VISUALIZE_FITTING
                            , fitTBuf(NULL)
#endif
{
  using namespace rapidjson;

  if(!(design.doc.HasMember("landSurface") && design.doc["landSurface"].IsObject()))
    err(-1, "No land surface available\n");
  
  Value& LsJson = design.doc["landSurface"];
  float width;
  if(LsJson.HasMember("width") && LsJson["width"].IsNumber())
    width = LsJson["width"].GetFloat();
  else
    err(-1, "Bad landSurface width in file %s\n", design.config.designFileName);
  if(LsJson.HasMember("textureFile") && LsJson["textureFile"].IsString())
    rect = new TexturedRect(shader, LsJson["textureFile"].GetString(), width, 0.0f);
  else
    err(-1, "Bad landSurface texturefile in file %s\n", design.config.designFileName);

  if(checkGLError(stderr, "LandSurface::LandSurface"))
    exit(-1);
}


// =======================================================================================
// Late constructor stuff that can't be done until after the Qtree is initialized

void LandSurface::bufferGeometry(Quadtree* q)
{
  if(q)
    qtree = q;
  
  if(tbuf)
    delete tbuf;
  //XXX note sizing here is inefficient, needs to be replaced with more dynamic
  // approach
  tbuf = new TriangleBuffer(qtree->landVBOSize, qtree->landVBOSize);
  if(!tbuf)
    err(-1, "Can't allocate memory in __func__\n");
  qtree->bufferLandSurface(tbuf);
  tbuf->sendToGPU(GL_STATIC_DRAW);
}


// =======================================================================================
// Destructor

LandSurface::~LandSurface(void)
{
  if(rect)
    delete rect;
  if(tbuf)
    delete tbuf;
}


// =======================================================================================
// Highlight one qtree node.  If color is NULL, unhighlights.  Assumes the correct
// VAO/VBO are already bound

void LandSurface::highlightNode(Quadtree* targetNode, vec4& color, float accent)
{
/*
#ifdef USE_TRIANGLE_BUFFER
  
#else
  //printf("Size: %u\tOffset: %u\n", targetNode->landVBOSize, targetNode->bufferOffset);
  VertexBufElement* buf = (VertexBufElement*)glMapBufferRange(GL_ARRAY_BUFFER,
                        (targetNode->bufferOffset)*sizeof(VertexBufElement),
                        (targetNode->landVBOSize)*sizeof(VertexBufElement), GL_MAP_WRITE_BIT);
  if(checkGLError(stderr, "LandSurface::highlightNode:glMapBufferRange") || !buf)
    exit(-1);
  VertexBufElement* b;
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
// Reconfigure the land surface when we are told about a new observation that is to be
// added to our state

//XXX We don't currently detect if a new heightmarker is unacceptable - eg second one
// is exactly the same as first.  Need to do more error checking, and have a way to
// report that the heightMarker should be rejected and deleted.

extern vec3 zAxis;

void LandSurface::newLandHeight(HeightMarker* hM)
{
  heightLocations.push_back(hM->location);
  locationCount++;
  vec3 plane;
  
  if(locationCount == 1)
   {
    // level surface at the new height
    plane[0] = 0.0f; // co-efficient of x
    plane[1] = 0.0f; // co-efficient of y
    plane[2] = hM->location[2]; // intersection with the z-axis
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
    bez->randomFit(heightLocations);
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
// Redo the triangle buffer (after improving the fit).  XX Note the approach here of
// tossing the whole thing and start over seems a bit costly and a more efficient
// approach might need to be developed if this is a bottleneck

void LandSurface::redoBezierLandSurface(BezierPatch* bez)
{
  unsigned vCount, iCount;
  bez->triangleBufferSizes(vCount, iCount);
  recycleTriangleBuffer(tbuf, vCount, iCount);
  bez->bufferGeometry(tbuf);
  tbuf->sendToGPU(GL_STATIC_DRAW);

#ifdef VISUALIZE_FITTING
  DisplayList* D = bez->newUVLocationList();
  bez->addControlPointsToDisplayList(D);
  bez->addControlGradientsToDisplayList(D);
  D->triangleBufferSizes(vCount, iCount);
  recycleTriangleBuffer(fitTBuf, vCount, iCount);
  D->bufferGeometry(fitTBuf);
  fitTBuf->sendToGPU(GL_STATIC_DRAW);
#endif
}


// =======================================================================================
// Render our part of the scene

vec4 yellowAccentColor = {0.9f, 0.9f, 0.0f, 1.0f};

void LandSurface::draw(Camera& camera)
{
  // Highlight where the camera points at
/*  vec3 pos, dir;
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
  rect->texture.bind(shader, 0, "earthTexture");
  tbuf->draw();
#ifdef VISUALIZE_FITTING
  if(fitTBuf)
   {
    shader.setUniform("noTexColor", true);
    fitTBuf->draw();
    shader.setUniform("noTexColor", false);
   }
#endif

  if(checkGLError(stderr, "LandSurface::draw"))
    exit(-1);
}


// =======================================================================================
// Provide a diagnostic page about the land surface

bool LandSurface::diagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Land");
  
  serv->addResponseData("<center>\n");
  serv->startTable();
  serv->addResponseData("<tr><th>Heightmarker index</th><th>X</th>");
  serv->addResponseData("<th>Y</th><th>Z</th></tr>\n");
  
  for(int i=0; i< locationCount; i++)
   {
    serv->respPtr += sprintf(serv->respPtr, "<tr><th>%d</td>", i);
    for(int j=0; j<3; j++)
      serv->respPtr += sprintf(serv->respPtr, "<td>%.1f</td>", heightLocations[i][j]);
    serv->addResponseData("</tr>\n");
   }
  
  serv->addResponseData("</table></center><hr>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================

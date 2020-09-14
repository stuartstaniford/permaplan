
// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// This manages the quadtree used for efficient organization of visual rendering

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Quadtree.h"
#include "Scene.h"


/* Arrangement of kids is as follows:
 ---------------
 |      |      |
 | k[2] | k[3] |
 |      |      |
 ---------------
 |      |      |
 | k[0] | k[1] |
 |      |      |
 ---------------      */

#define forAllKids(i)  for(int i=0; i<4; i++) if(kids[i])

// =======================================================================================
// Constructor for a quadtree node

vec3 xyPlane = {0.0f, 0.0f, 0.0f};

Quadtree::Quadtree(Shader& S, float x, float y, unsigned width, unsigned height,
                    float s, float t, float sWidth, float tHeight, unsigned& minSize,
                    unsigned offset):
                        landVBOSize(0u),
                        bufferOffset(offset),
                        vertexTBufSize(0u),
                        indexTBufSize(0u),
                        bbox(x, y, 0.0f, x + (float)width, y+ (float)height, 0.1f),
                        shader(S),
                        surface(NULL),
                        vObjects()
{
  topLeftZ      = 0.05f;
  bottomRightZ  = 0.05f;

  setVec2(textureBL, s, t);
  setVec2(textureTR, s + sWidth, t + tHeight);

  unsigned h1, h2, w1, w2;
  float    th1, th2, sw1, sw2;

  //printf("Creating Quadtree at [%.1f, %.1f] with dims [%u, %u]\n", x, y, width, height);
  if(height > minSize)
   {
    h1  = height/2;
    h2 = height - h1;
    th1 = (tHeight*h1)/height;
    th2 = (tHeight*h2)/height;
   }
  else
   {
    h1  = height;
    th1 = tHeight;
   }
  
  if(width > minSize)
   {
    w1 = width/2;
    w2 = width - w1;
    sw1 = (sWidth*w1)/width;
    sw2 = (sWidth*w2)/width;
    kids[0] = new Quadtree(shader, x, y, w1, h1, s, t, sw1, th1, minSize, offset);
    offset += kids[0]->landVBOSize;
    kids[1] = new Quadtree(shader, x+w1, y, w2, h1, s + sw1, t, sw2, th1, minSize, offset);
    offset += kids[1]->landVBOSize;
    landVBOSize += kids[0]->landVBOSize + kids[1]->landVBOSize;
    if(height > minSize)
     {
     // Splitting on x and y
      kids[2] = new Quadtree(shader, x, y+h1, w1, h2, s, t+th1, sw1, th2, minSize, offset);
      offset += kids[2]->landVBOSize;
      kids[3] = new Quadtree(shader, x+w1, y+h1, w2, h2, s+sw1, t+th1, sw2, th2, minSize, offset);
      landVBOSize += kids[2]->landVBOSize + kids[3]->landVBOSize;
     }
    else
     {
      // We are splitting on x, but not on y
      kids[2] = kids[3] = NULL;
     }
   }
  else
   {
    w1  = width;
    sw1 = sWidth;
    kids[1] = kids[3] = NULL;
    if(height <= minSize)
     {
      // leaves are actually the most common case
      kids[0] = kids[2] = NULL;
      landVBOSize = 6;   //XX should this be hard-coded here?
      surface = new LandSurfaceRegionPlanar(x, y, width, height, s, t,
                                            sWidth, tHeight, xyPlane);
      // Do other leafy stuff here
     }
    else
     {
      // We are splitting on y, but not on x
      kids[0] = new Quadtree(shader, x, y, w1, h1, s, t, sw1, th1, minSize, offset);
      offset += kids[0]->landVBOSize;
      kids[2] = new Quadtree(shader, x, y+h1, w1, h2, s, t+th1, sw1, th2, minSize, offset);
      landVBOSize += kids[0]->landVBOSize + kids[2]->landVBOSize;
     }
   }
  
  if(checkGLError(stderr, "Quadtree::Quadtree"))
    exit(-1);
}


// =======================================================================================
// Destructor

Quadtree::~Quadtree(void)
{
  forAllKids(i)
    delete kids[i];
}


// =======================================================================================
// Put a single quadtree node into a gpu buffer.  Generally used on leaves, but would
// work on internal nodes if ever used for that.

void Quadtree::bufferGeometryLeaf(VertexBufElement* buf)
{
  // First triangle
  buf[0].set(bbox.lower[0], bbox.lower[1], bbox.lower[2],   //lower left
                                                    textureBL[0], textureBL[1]);
  buf[1].set(bbox.upper[0], bbox.lower[1], bottomRightZ,          //lower right
                                                    textureTR[0], textureBL[1]);
  buf[2].set(bbox.lower[0], bbox.upper[1], topLeftZ,   //upper left
                                                    textureBL[0], textureTR[1]);
  
  // Second triangle
  buf[3].copy(buf+2);//upper left
  buf[4].copy(buf+1);//lower right
  buf[5].set(bbox.upper[0], bbox.upper[1], bbox.upper[2],  //upper right
                                                    textureTR[0], textureTR[1]);
}


// =======================================================================================
// This function stores a visual object in the quadtree, which can later be accessed,
// displayed etc.  If the object's bounding box entirely fits in one of our kids'
// in an x-y sense, we will have the kid store it (and on down recursively).  However
// if it crosses over our kids, we will store it.

void Quadtree::storeVisualObject(VisualObject* obj)
{
  unsigned vSize, iSize;
  obj->triangleBufferSizes(vSize, iSize);
  vertexTBufSize += vSize;
  indexTBufSize += iSize;

  bbox.extendZ(*(obj->box));
  forAllKids(i)
    if(kids[i]->bbox.xyContains(*(obj->box)))
     {
      kids[i]->storeVisualObject(obj);
      return;
     }
  vObjects.push_back(obj);
}


// =======================================================================================
// Put all of the quadtree visual objects into a buffer in depth first order.

void Quadtree::bufferVisualObjects(TriangleBuffer* tbuf)
{
  // Handle visual objects stored at our level
  vObjects.bufferGeometry(tbuf);
  
  // Deal with kids
  if(landVBOSize > 6)
    forAllKids(i)
      kids[i]->bufferVisualObjects(tbuf);
}


// =======================================================================================
// Put all of the quadtree visual objects into a buffer in depth first order.

void Quadtree::bufferLandSurface(TriangleBuffer* tbuf)
{
  if(landVBOSize > 6)
   {
    // Deal with kids
    forAllKids(i)
      kids[i]->bufferLandSurface(tbuf);
   }
  else
    surface->bufferGeometry(tbuf); // we are a leaf, buffer our surface object
}


// =======================================================================================
// Recompute the bounding box from scratch after changes

void Quadtree::recomputeBoundingBox(void)
{
  bbox.unsetZs();
  if(landVBOSize > 6)
   {
    forAllKids(i)
      bbox.extendZ(kids[i]->bbox);
   }
  else
    bbox.extendZ(*(surface->box));
  
  //XXX need to recompute the effect of visual objects
}


// =======================================================================================
// Redo the landsurface in the quadtree with a plane specified by the parameters
// (The plane goes through position, with normal being perpendicular to it).
// Note this function makes the assumption that our surface is already an
// existing LandSurfaceRegionPlanar.  This should generally be true.

void Quadtree::redoLandPlanar(vec3 plane)
{
  if(landVBOSize > 6)
   {
    forAllKids(i)
      kids[i]->redoLandPlanar(plane);
   }
  else
   {
    LandSurfaceRegionPlanar* planarSurface = (LandSurfaceRegionPlanar*)surface;
    planarSurface->resetPlane(plane);
   }
  recomputeBoundingBox();
}


// =======================================================================================
// Remove all surface links, preparatory to rebuilding some other way.  Note that
// bounding boxes could be invalid after this call, and will need to be rebuilt.

void Quadtree::stripSurface(void)
{
  if(landVBOSize > 6)
   {
    forAllKids(i)
      kids[i]->stripSurface();
   }
  else
   {
    delete surface;
    surface = NULL;
   }
}

// =======================================================================================
// Put all of the quadtree geometry into a buffer in depth first order.  Caller must
// give us a large enough buffer (presumably by checking landVBOSize).

void Quadtree::bufferGeometry(VertexBufElement* buf)
{
  if(6==landVBOSize)
   {
    // We are a leaf
    bufferGeometryLeaf(buf);
   }
  else
   {
    VertexBufElement* b = buf;
    forAllKids(i)
     {
      kids[i]->bufferGeometry(b);
      b += kids[i]->landVBOSize;
     }
   }
}


// =======================================================================================
// Render our part of the scene

void Quadtree::draw(void)
{
  forAllKids(i)
    kids[i]->draw();
  
  if(checkGLError(stderr, "Quadtree::draw"))
    exit(-1);
}


// =======================================================================================
// Used by matchRay, in the situation in which we know that we have matched, and we want
// to know if there's a more specific match to our child.

Quadtree* Quadtree::matchChild(vec3& position, vec3& direction, float& lambda)
{
  float     kidLambda;
  float     bestLambda      = HUGE_VALF;
  Quadtree* descendant;
  Quadtree* bestDescendant  = NULL;
  
  for(int i=0; i<4; i++)
   {
    if(!kids[i])
      continue;
    if(!(descendant = kids[i]->matchRay(position, direction, kidLambda)))
      continue;
    if(kidLambda < bestLambda)
     {
      bestLambda      = kidLambda;
      bestDescendant  = descendant;
     }
   }
  if(bestDescendant)
   {
    lambda = bestLambda;
    return bestDescendant;
   }
  return this;
}


// =======================================================================================
// Give a ray specified by a position vector and a direction vector, find the first
// quadtree leaf node that intersects the ray.  Returns NULL if nothing matches.  Note
// matches at negative multiples of the direction vector will not be counted - we seek
// the smallest positive multiple of direction (here store in the reference arg lambda).

Quadtree* Quadtree::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(bbox.matchRay(position, direction, lambda))
    return matchChild(position, direction, lambda);
  else
    return NULL;
}


// =======================================================================================
// Provide a diagnostic page about this node in the tree (or possibly refer to one of our kids)

bool Quadtree::diagnosticHTML(HttpDebug* serv, char* path)
{
  if(path[0] != '\0')
   {
    // We are being asked about one of our children (or else there's an error).
    // the only acceptable next part of the url is 0/ or 1/ or 2/ or 3/
    if(strncmp(path, "0/", 2) == 0 && kids[0])
      return kids[0]->diagnosticHTML(serv, path+2);
    else if(strncmp(path, "1/", 2) == 0 && kids[1])
      return kids[1]->diagnosticHTML(serv, path+2);
    else if(strncmp(path, "2/", 2) == 0 && kids[2])
      return kids[2]->diagnosticHTML(serv, path+2);
    else if(strncmp(path, "3/", 2) == 0 && kids[3])
      return kids[3]->diagnosticHTML(serv, path+2);
    else
      return serv->errorPage("Bad child quadtree node");
   }
  else
   {
    // We are to provide information about ourself
    serv->startResponsePage("Quadtree Node");

    // Bounding box
    bbox.diagnosticHTML(serv);
 
    serv->newSection("Texture Bounds");
    serv->startTable();
    serv->addResponseData("<tr><th></th><th>Bottom Left</th><th>Top Right</th></tr>\n");

    // S co-ords
    serv->addResponseData("<tr><td>S</td><td>");
    serv->respPtr += sprintf(serv->respPtr, "%.4f</td><td>%.4f</td></tr>\n",
                             textureBL[0], textureTR[0]);
    // T co-ords
    serv->addResponseData("<tr><td>T</td><td>");
    serv->respPtr += sprintf(serv->respPtr, "%.4f</td><td>%.4f</td></tr>\n",
                             textureBL[1], textureTR[1]);
    serv->addResponseData("</table></center><hr>\n");

    // Kids
    serv->newSection("Child Nodes");
    serv->startTable();
    serv->addResponseData("<tr><th>Index</th><th>X Bounds</th><th>Y Bounds</th>\n");
    for(int i = 0; i < 4; i++)
     {
      if(kids[i])
       {
        serv->respPtr += sprintf(serv->respPtr,
                          "<tr><td><a href=\"%d/\">%d</a></td>",i, i);
        serv->respPtr += sprintf(serv->respPtr, "<td>(%.1f, %.1f)</td>",
                            kids[i]->bbox.lower[0], kids[i]->bbox.upper[0]);
        serv->respPtr += sprintf(serv->respPtr, "<td>(%.1f, %.1f)</td>",
                            kids[i]->bbox.lower[1], kids[i]->bbox.upper[1]);
       }
      else
        serv->respPtr += sprintf(serv->respPtr, "<tr><td>%d</td><td>None",i);
      serv->addResponseData("</td></tr>\n");
     }
    serv->addResponseData("</table></center>\n");
    
    // Visual objects
    serv->newSection("Objects Stored at this Quadtree Node");
    serv->startTable();
    if(surface)
      surface->diagnosticHTML(serv);
    vObjects.diagnosticHTML(serv);
    serv->addResponseData("</table></center>\n");

    // Vertex Data
    serv->newSection("Vertex Data");
    serv->addResponseData("</center>\n");
    serv->respPtr += sprintf(serv->respPtr, "<b>Vertex Count:</b> %u<br>\n", landVBOSize);
    serv->respPtr += sprintf(serv->respPtr, "<b>Vertex Offset:</b> %u<br>\n", bufferOffset);

    
    // Other data
    serv->newSection("Other Data");
    serv->addResponseData("</center>\n");

    // Camera intersection
    float lambda;
    vec3 pos, dir;
    serv->scene.camera.copyDirection(pos, dir);
    if(matchRay(pos, dir, lambda))
      serv->respPtr += sprintf(serv->respPtr, "<b>Camera intersects, Lambda:</b> %.3f<br>\n",
                               lambda);
    else
      serv->addResponseData("Camera doesn't intersect.<br>\n");

    serv->endResponsePage();
   }
  return true;
}


// =======================================================================================

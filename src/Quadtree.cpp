
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

#define forAllKids(i)  if(!isLeaf) for(int i=0; i<4; i++) if(kids[i])

// =======================================================================================
// Constructor for a quadtree node

vec3 xyPlane = {0.0f, 0.0f, 0.0f};

Quadtree::Quadtree(float x, float y, unsigned width, unsigned height,
                    float s, float t, float sWidth, float tHeight, unsigned& minSize,
                    unsigned offset, unsigned lev, Quadtree* prt):
                        landVBOSize(0u),
                        bufferOffset(offset),
                        vertexTBufSize(0u),
                        indexTBufSize(0u),
                        bbox(x, y, HUGE_VALF, x + (float)width, y+ (float)height, -HUGE_VALF),
                        parent(prt),
                        surface(NULL),
                        vObjects(),
                        level(lev),
                        isLeaf(true)
{
  topLeftZ      = 0.05f;
  bottomRightZ  = 0.05f;

  setVec2(textureBL, s, t);
  setVec2(textureTR, s + sWidth, t + tHeight);

#ifdef MULTI_THREADED_SIMULATION
  pthread_mutex_init(&mutex, NULL);
#endif

  unsigned h1, h2, w1, w2;
  float    th1, th2, sw1, sw2;

  LogQuadtreeCreation("Creating Quadtree[level %u] at [%.1f, %.1f] with dims [%u, %u]\n",
                                                            level, x, y, width, height);
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
    kids[0] = new Quadtree(x, y, w1, h1, s, t, sw1, th1, minSize, offset, level+1, this);
    offset += kids[0]->landVBOSize;
    kids[1] = new Quadtree(x+w1, y, w2, h1, s + sw1, t, sw2, th1, minSize, offset,
                                                                          level+1, this);
    offset += kids[1]->landVBOSize;
    landVBOSize += kids[0]->landVBOSize + kids[1]->landVBOSize;
    isLeaf = false;
    if(height > minSize)
     {
     // Splitting on x and y
      kids[2] = new Quadtree(x, y+h1, w1, h2, s, t+th1, sw1, th2, minSize, offset,
                                                                            level+1, this);
      offset += kids[2]->landVBOSize;
      kids[3] = new Quadtree(x+w1, y+h1, w2, h2, s+sw1, t+th1, sw2, th2, minSize,
                                                                      offset, level+1, this);
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
      // leaves are actually the most common case - leave isLeaf as true
      kids[0] = kids[2] = NULL;
      landVBOSize = 6;   //XX should this be hard-coded here?
      surface = new LandSurfaceRegionPlanar(x, y, width, height, s, t,
                                            sWidth, tHeight, xyPlane);
      // Do other leafy stuff here
     }
    else
     {
      // We are splitting on y, but not on x
      isLeaf = false;
      kids[0] = new Quadtree(x, y, w1, h1, s, t, sw1, th1, minSize, offset, level+1, this);
      offset += kids[0]->landVBOSize;
      kids[2] = new Quadtree(x, y+h1, w1, h2, s, t+th1, sw1, th2, minSize, offset,
                                                                              level+1, this);
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
#ifdef MULTI_THREADED_SIMULATION
  pthread_mutex_destroy(&mutex);
#endif

  forAllKids(i)
    delete kids[i];
  if(surface != parent->surface)
    delete surface;
}


// =======================================================================================
// Put a single quadtree node into a gpu buffer.  Generally used on leaves, but would
// work on internal nodes if ever used for that.

void Quadtree::bufferGeometryLeaf(Vertex* buf)
{
  // First triangle
  buf[0].setPosition(bbox.lower[0], bbox.lower[1], bbox.lower[2]);   //lower left
  buf[0].setTexCoords(textureBL[0], textureBL[1]);
  buf[1].setPosition(bbox.upper[0], bbox.lower[1], bottomRightZ);          //lower right
  buf[1].setTexCoords(textureTR[0], textureBL[1]);
  buf[2].setPosition(bbox.lower[0], bbox.upper[1], topLeftZ);   //upper left
  buf[2].setTexCoords(textureBL[0], textureTR[1]);
  
  // Second triangle
  buf[3].copy(buf+2);//upper left
  buf[4].copy(buf+1);//lower right
  buf[5].setPosition(bbox.upper[0], bbox.upper[1], bbox.upper[2]);  //upper right
  buf[5].setTexCoords(textureTR[0], textureTR[1]);
}


// =======================================================================================
// This function stores a visual object in the quadtree, which can later be accessed,
// displayed etc.  If the object's bounding box entirely fits in one of our kids'
// in an x-y sense, we will have the kid store it (and on down recursively).  However
// if it crosses over our kids, we will store it.

// Note outsiders should only call this function on the root of the quadtree, or there
// will be problems with bounding box book-keeping.

void Quadtree::storeVisualObject(VisualObject* obj)
{
  unsigned vSize, iSize;
  obj->triangleBufferSizes(vSize, iSize);
  vertexTBufSize += vSize;
  indexTBufSize += iSize;
  LogQuadtreeObjSizes("Quadtree node obj size estimates increasing by [%u, %u] "
                        "to [%u, %u]\n", vSize, iSize, vertexTBufSize, indexTBufSize);
#ifdef LOG_QUADTREE_INSERTIONS
  float x, y;
  obj->getGroundContact(x,y);
#endif

  bbox.extendZ(*(obj->box));
  forAllKids(i)
    if(kids[i]->bbox.xyContains(*(obj->box)))
     {
      LogQuadtreeInsertions("Passing %s object at %.1f, %.1f to child %d, level %u.\n",
                            obj->objectName(), x, y, i, level);
      kids[i]->storeVisualObject(obj);
      return;
     }
  LogQuadtreeInsertions("Final storage of %s object at %.1f, %.1f, level %u.\n",
                          obj->objectName(), x, y, level);
  vObjects.emplace(obj);
  obj->qTreeNode = this;
}


// =======================================================================================
// When a visual object has changed it's dimensions (eg a tree grew bigger, or a box
// rotated and increased the size of it's bounding box) it calls this function to notify
// us.  We decide if the object still fits in us, or needs to be transferred up to our
// parent.

// XX don't currently handle the case of an object that shrinks and should be passed down
// to a child (which is mainly an efficiency issue).

// Note that after this is called, there must be a call to rebuildTBufSizes, as we don't
// keep track of the effects of our changes on vertexTBufSize and indexTBufSize

void Quadtree::notifyObjectBoxChange(VisualObject* obj)
{
  LogQuadtreeBoundBox("notifyObjectBoxChange at level %d needs to do something.", level);

  Quadtree* q;
  
  // Fix Quadtree bounding boxes if necesssary
  for(q = this; q; q = q->parent)
    unless(q->bbox.extendZ(*(obj->box)))
      break;
      
  if(!parent)
    return; // we are the root so we are stuck with this thing, regardless

  if(bbox.xyContains(*(obj->box)))
    return; // nothing to do here
    
  vObjects.erase(obj);
  parent->newObjectFromChild(obj);
}


// =======================================================================================
// A child is ejecting an object that has grown too big for it and wants us to handle it.

void Quadtree::newObjectFromChild(VisualObject* obj)
{
  LogQuadtreeBoundBox("newObjectFromChild at level %d needs to do something.", level);

  if(!parent || bbox.xyContains(*(obj->box)))
   {
    vObjects.emplace(obj);  // don't fix boundingBox as notifyObjectBoxChange did it.
    obj->qTreeNode = this;
   }
  else
    parent->newObjectFromChild(obj);
}


// =======================================================================================
// Go through the quadtree and everything in it and double check that everything is
// self consistent.  Called per frame if compiled in.  Should not be turned on in
// production releases

#ifdef LOG_TREE_VALIDATION

void Quadtree::selfValidate(unsigned l)
{
  // Checks at our own level
  if(l==0) // special cases at the root
   {
    assert(level == 0);
    assert(parent == NULL);
   }
  else
    assert(level == l);
  
  // Check our VisualObjects
  for(VisualObject* v: vObjects)
   {
    assert(v->qTreeNode == this);
    if(v->box->isDefined())
     {
      if(level>0)
        assert(*(v->box) <= bbox);
      else
        assert(bbox.zContains(*(v->box))); // objects at root are allowed to overlap the edge
     }
    v->selfValidate(0u);
   }
  
  // Check bounding box x-y constraints of kids.
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

  if(kids[0])
   {
    assert(kids[0]->bbox.lower[0] == bbox.lower[0]);
    assert(kids[0]->bbox.lower[1] == bbox.lower[1]);
   }
  if(kids[1])
   {
    assert(kids[1]->bbox.upper[0] == bbox.upper[0]);
    assert(kids[1]->bbox.lower[1] == bbox.lower[1]);
   }
  if(kids[2])
   {
    assert(kids[2]->bbox.lower[0] == bbox.lower[0]);
    assert(kids[2]->bbox.upper[1] == bbox.upper[1]);
   }
  if(kids[3])
   {
    assert(kids[3]->bbox.upper[0] == bbox.upper[0]);
    assert(kids[3]->bbox.upper[1] == bbox.upper[1]);
   }

  // Recursively check our kids
  int kidCount = 0;
  forAllKids(i)
   {
    assert(kids[i]->parent == this);
    assert(kids[i]->bbox.lower[2] >= bbox.lower[2]);
    assert(kids[i]->bbox.upper[2] <= bbox.upper[2]);
    kids[i]->selfValidate(l+1);
    kidCount++;
   }
  assert(kidCount == 0 || kidCount == 2 || kidCount == 4);
}
#endif


// =======================================================================================
// Put all of the quadtree visual objects into a buffer in depth first order.

void Quadtree::bufferVisualObjects(TriangleBuffer* tbuf)
{
  // Handle visual objects stored at our level
  vObjects.bufferGeometry(tbuf);
  
  // Deal with kids
  forAllKids(i)
    kids[i]->bufferVisualObjects(tbuf);
}


// =======================================================================================
// This gets called whenever the shape of the land has changed, and tells all the visual
// objects in the quadtree to update their idea of their altitude.

void Quadtree::adjustAltitudes(LandSurfaceRegion* landsurface)
{
  // Handle visual objects stored at our level
  if(!surface)
    surface = landsurface;
  vObjects.adjustAltitudes(surface);
  
  // Deal with kids
  forAllKids(i)
    kids[i]->adjustAltitudes(surface);
}


// =======================================================================================
// Put all of the quadtree visual objects into a buffer in depth first order.

void Quadtree::bufferLandSurface(TriangleBuffer* tbuf)
{
  if(isLeaf)
    surface->bufferGeometry(tbuf); // buffer our surface object
  else
   {
    // Deal with kids
    forAllKids(i)
      kids[i]->bufferLandSurface(tbuf);
   }
}


// =======================================================================================
// Recompute the bounding box from scratch after changes

void Quadtree::recomputeBoundingBox(void)
{
  bbox.unsetZs();
  if(isLeaf)
    bbox.extendZ(*(surface->box));
  else
    forAllKids(i)
      bbox.extendZ(kids[i]->bbox);
  
  for(VisualObject* v: vObjects)
    if(v->box->isDefined())
      bbox.extendZ(*(v->box));
}


// =======================================================================================
// Redo our idea of the size we need to render ourselves in a Tbuf.  This is called
// for example after a simulation step in which objects may change their size/form.

void Quadtree::rebuildTBufSizes(void)
{
  vObjects.triangleBufferSizes(vertexTBufSize, indexTBufSize);
  forAllKids(i)
   {
    kids[i]->rebuildTBufSizes();
    vertexTBufSize  += kids[i]->vertexTBufSize;
    indexTBufSize   += kids[i]->indexTBufSize;
   }

#ifdef LOG_QUADTREE_OBJ_SIZES
  if(!level)
   {
    LogQuadtreeObjSizes("Quadtree obj sizes at level %u recomputed to [%u, %u].\n",
                        level, vertexTBufSize, indexTBufSize);
   }
#endif
}


// =======================================================================================
// Redo the landsurface in the quadtree with a plane specified by the parameters
// (The plane goes through position, with normal being perpendicular to it).
// Note this function makes the assumption that our surface is already an
// existing LandSurfaceRegionPlanar.  This should generally be true.

void Quadtree::redoLandPlanar(vec3 plane)
{
  if(isLeaf)
   {
    LandSurfaceRegionPlanar* planarSurface = (LandSurfaceRegionPlanar*)surface;
    planarSurface->resetPlane(plane);
   }
  else
    forAllKids(i)
      kids[i]->redoLandPlanar(plane);

  recomputeBoundingBox();
}


// =======================================================================================
// Remove all surface links, preparatory to rebuilding some other way.  Note that
// bounding boxes could be invalid after this call, and will need to be rebuilt.

void Quadtree::stripSurface(void)
{
  if(isLeaf)
   {
    delete surface;
    surface = NULL;
   }
  else
    forAllKids(i)
      kids[i]->stripSurface();
}

// =======================================================================================
// Put all of the quadtree geometry into a buffer in depth first order.  Caller must
// give us a large enough buffer (presumably by checking landVBOSize).

void Quadtree::bufferGeometry(Vertex* buf)
{
  if(isLeaf)
   {
    // We are a leaf
    bufferGeometryLeaf(buf);
   }
  else
   {
    Vertex* b = buf;
    forAllKids(i)
     {
      kids[i]->bufferGeometry(b);
      b += kids[i]->landVBOSize;
     }
   }
}


// =======================================================================================
// Used by matchRay, in the situation in which we know that we have matched, and we want
// to know if there's a more specific match to our child.

VisualObject* Quadtree::matchChild(vec3& position, vec3& direction, float& lambda)
{
  float         kidLambda;
  float         bestLambda      = HUGE_VALF;
  VisualObject* descObject;
  VisualObject* bestDescObject  = NULL;
  
  forAllKids(i)
   {
    if(!(descObject = kids[i]->matchRay(position, direction, kidLambda)))
      continue;
    if(kidLambda < bestLambda)
     {
      bestLambda      = kidLambda;
      bestDescObject  = descObject;
     }
   }
  if(bestDescObject)
   {
    lambda = bestLambda;
    return bestDescObject;
   }
  return NULL;
}


// =======================================================================================
// Give a ray specified by a position vector and a direction vector, find the first
// object leaf node that intersects the ray.  Returns NULL if nothing matches.  Note
// matches at negative multiples of the direction vector will not be counted - we seek
// the smallest positive multiple of direction (here store in the reference arg lambda).

VisualObject* Quadtree::matchRay(vec3& position, vec3& direction, float& lambda)
{
  VisualObject* returnObject = NULL;
  
  unless(bbox.matchRay(position, direction, lambda))
    return NULL;
  
  float         objLambda, bestLambda = HUGE_VALF;
  for(VisualObject* v: vObjects)
    if(v->matchRay(position, direction, objLambda))
      if(objLambda < bestLambda)
       {
        bestLambda    = objLambda;
        returnObject  = v;
       }
  
#ifdef LOG_QUADTREE_MATCH_RAY
  if(returnObject)
    LogQuadtreeMatchRay("Quadtree matchRay: best object at level %d has lambda %.2f and "
                        "type %s\n", level, bestLambda, returnObject->objectName());
#endif
  
  if(VisualObject* bestKidObject = matchChild(position, direction, objLambda))
   {
    if(objLambda < bestLambda)
     {
      bestLambda    = objLambda;
      returnObject  = bestKidObject;
      LogQuadtreeMatchRay("Quadtree matchRay: best object at level %d superseded: "
                          "lambda %.2f and type %s\n", level, bestLambda,
                          returnObject->objectName());
     }
   }
  
  if(returnObject)
   {
    lambda = bestLambda;
    return returnObject;
   }
    
  if(surface && surface->matchRay(position, direction, lambda))
   {
    LogQuadtreeMatchRay("Quadtree matchRay: returning surface at level %d", level);
    return surface;
   }
  
  return NULL;
}


// =======================================================================================
// Store the landsurface configuation in a file

void Quadtree::saveSurfaceState(char* fileName)
{
  if(surface && strcmp(surface->objectName(), (char*)"Bezier Patch") == 0)
   {
    FILE* bezFile = fopen(fileName, "w");
    ((BezierPatch*)surface)->printControlPoints(bezFile);
    fclose(bezFile);
   }
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
    unless(serv->startResponsePage("Quadtree Node"))
      return false;

    // Bounding box
    unless(bbox.diagnosticHTML(serv))
     {
      LogResponseErrors("Couldn't output bbox HTTP at quadtree level %d\n", level);
      return false;
     }
    
    unless(serv->newSection("Texture Bounds"))
      return false;
    unless(serv->startTable())
      return false;
    httPrintf("<tr><th></th><th>Bottom Left</th><th>Top Right</th></tr>\n");

    // S co-ords
    httPrintf("<tr><td>S</td><td>");
    httPrintf("%.4f</td><td>%.4f</td></tr>\n", textureBL[0], textureTR[0]);
    
    // T co-ords
    httPrintf("<tr><td>T</td><td>");
    httPrintf("%.4f</td><td>%.4f</td></tr>\n", textureBL[1], textureTR[1]);
    httPrintf("</table></center><hr>\n");

    // Kids
    unless(serv->newSection("Child Nodes"))
      return false;
    unless(serv->startTable())
      return false;
    httPrintf("<tr><th>Index</th><th>X Bounds</th><th>Y Bounds</th>\n");
    
    forAllKids(i)
     {
      httPrintf("<tr><td><a href=\"%d/\">%d</a></td>", i, i);
      httPrintf("<td>(%.1f, %.1f)</td>", kids[i]->bbox.lower[0], kids[i]->bbox.upper[0]);
      httPrintf("<td>(%.1f, %.1f)</td>", kids[i]->bbox.lower[1], kids[i]->bbox.upper[1]);
      httPrintf("</td></tr>\n");
     }
    httPrintf("</table></center>\n");
    
    // Visual objects
    unless(serv->newSection("Objects Stored at this Quadtree Node"))
      return false;
    unless(serv->startTable())
      return false;
    if(surface)
      unless(surface->diagnosticHTML(serv))
       {
        LogResponseErrors("Couldn't output surface HTTP at quadtree level %d\n", level);
        return false;
       }
    unless(vObjects.diagnosticHTML(serv))
     {
      LogResponseErrors("Couldn't output vObjects HTTP at quadtree level %d\n", level);
      return false;
     }
    httPrintf("</table></center>\n");

    // Vertex Data
    unless(serv->newSection("Vertex Data"))
      return false;
    httPrintf("</center>\n");
    httPrintf("<b>Vertex Count:</b> %u<br>\n", landVBOSize);
    httPrintf("<b>Vertex Offset:</b> %u<br>\n", bufferOffset);
    
    // Other data
    unless(serv->newSection("Other Data"))
      return false;
    httPrintf("</center>\n");
    httPrintf("<b>Level:</b> %u<br>\n", level);

    // Camera intersection
    float lambda;
    vec3 pos, dir;
    serv->scene.camera.copyDirection(pos, dir);
    if(matchRay(pos, dir, lambda))
     {
      httPrintf("<b>Camera intersects, Lambda:</b> %.3f<br>\n", lambda);
     }
    else
     {
      httPrintf("Camera doesn't intersect.<br>\n");
     }
    
    unless(serv->endResponsePage())
      return false;
   }
              
  return true;
}


// =======================================================================================

// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class for storing and rendering the rectangular boxes (eg straw bales, durisol blocks,
// timber beams, etc).

#include <err.h>
#include "Box.h"


// =======================================================================================
// Constructors.

Box::Box(mat4 transform)
{
  glm_mat4_copy(transform, trans);
  updateBoundingBox();
}


// =======================================================================================
// Destructor

Box::~Box(void)
{
}


// =======================================================================================
// Iterator over vertices.  // return is ptr to a vec3

bool Box::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  if(resetToFirst)
   {
    iterI = iterJ = iterK = '\0';
   }
  else
   {
    iterK++;
    if(iterK > 1)
     {
      iterK = '\0';
      iterJ++;
     }
    if(iterJ > 1)
     {
      iterJ = '\0';
      iterI++;
     }
    if(iterI > 1)
      return false;
   }
  
  vec4 vertex4, out;

  vertex4[0] = iterI;
  vertex4[1] = iterJ;
  vertex4[2] = iterK;
  vertex4[3] = 1.0f;
  glm_mat4_mulv(trans, vertex4, out);
  glm_vec3(out, (float*)v->pos);
  return true;
}


// =======================================================================================
//XX Stub definition needs to be implemented
bool Box::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
// Iterator over indices.  Recall the underlying unit cube pre transformation is on axes
// from [0,0,0] to [1,1,1].  Refer to getNextVertex above for which vertex is which.  This
// function defines the triangles.  Recall counterclockwise winding order is front facing.
// https://learnopengl.com/Advanced-OpenGL/Face-culling

static int indexArray[36] = {
                          // Underside (z=0), close to origin
                          0, 2, 4,
                          // Underside (z=0), away from origin
                          2, 6, 4,
                          // Topside (z=1), close to origin
                          1, 5, 3,
                          // Topside (z=1), away from origin
                          3, 5, 7,
                          // Westside (x=0), close to origin
                          0, 1, 2,
                          // Westside (x=0), away from origin
                          2, 1, 3,
                          // Eastside (x=1), close to origin
                          5, 4, 6,
                          // Eastside (x=1), away from origin
                          5, 6, 7,
                          // Southside (y=0), close to origin
                          0, 4, 1,
                          // Southside (y=0), away from origin
                          1, 4, 5,
                          // Northside (y=1), close to origin
                          2, 3, 6,
                          // Northside (y=1), away from origin
                          3, 7, 6
                        };

int Box::getNextIndex(bool resetToFirst)
{
  if(resetToFirst)
    return indexArray[(index = 0)];
  else if(index >= 36)
    return -1;
  else
    return indexArray[++index];
}


// =======================================================================================
// This is where the actual geometry is defined - we render it into a buffer
// on request

bool Box::bufferGeometry(TriangleBuffer* T)
{
  Vertex* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, 8u, 36u))
    return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  int i;
  bool result;
  for(i=0, result=getNextUniqueVertex(true, vertices+i, PositionOnly); result;
                          i++, result = getNextUniqueVertex(false, vertices+i, PositionOnly))
   {
    if(useNoTexColor)
      vertices[i].setNoTexColor(noTexColor);
   }

  int I;
  for(i = 0, I = getNextIndex(true); I >= 0; i++, I = getNextIndex(false))
    indices[i] = vOffset + I;

  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Box::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 8u;
  iCount = 36u;
}


// =======================================================================================
// Stub definition

void Box::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool Box::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Compute the bounding box.
                                                                
void Box::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox();
  else
    box->hugeValify();
  
  Vertex v3;
  bool result;
  for(result = getNextUniqueVertex(true, &v3, PositionOnly); result;
                                  result = getNextUniqueVertex(false, &v3, PositionOnly))
   {
    for(int m=0; m<3; m++)
     {
      if(v3.pos[m] < box->lower[m])
        box->lower[m] = v3.pos[m];
      if(v3.pos[m] > box->upper[m])
        box->upper[m] = v3.pos[m];
     }
   }
  
  return;
}
                                                                

// =======================================================================================
// Tell callers our name at runtime.

const char* Box::objectName(void)
{
  static char* name = (char*)"Box";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool Box::diagnosticHTML(HttpDebug* serv)
{
  
  serv->addResponseData("<tr><td>Box</td>");
  /*serv->respPtr += sprintf(serv->respPtr,
                           "<td><b>Location:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                           location[0], location[1], location[2]);*/
  return true;
}


// =======================================================================================

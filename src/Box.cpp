// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class for storing and rendering the rectangular boxes (eg straw bales, durisol blocks,
// timber beams, etc).

#include <err.h>
#include "Box.h"

#define forAllBoxVertices(i,j,k) for(int i=0; i<2; i++) \
                                   for(int j=0; j<2; j++) \
                                     for(int k=0; k<2; k++)

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
// This is where the actual octahedron geometry is defined - we render it into a buffer
// on request

bool Box::bufferGeometry(TriangleBuffer* T)
{
  VertexBufElement* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, 8u, 36u))
    return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
 // vertices[0].set(location[0], location[1],
 //                 location[2]);  //bottom vertex
 
//  if(useNoTexColor)
//    for(int i=0; i<6; i++)
//      vertices[i].setNoTexColor(noTexColor);
  
  // Lower facing south
  //indices[0] = vOffset;
  //indices[1] = vOffset + 2u;
  //indices[2] = vOffset + 1u;


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
  if(!box->matchRay(position, direction, lambda))
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
  
  vec4 vertex4, out;
  vec3 vertex3;
  
  forAllBoxVertices(i,j,k)
   {
    vertex4[0] = i;
    vertex4[1] = j;
    vertex4[2] = k;
    vertex4[3] = 1.0f;
    glm_mat4_mulv(trans, vertex4, out);
    glm_vec3(out, vertex3);
    for(int m=0; m<3; m++)
     {
      if(vertex3[m] < box->lower[m])
        box->lower[m] = vertex3[m];
      if(vertex3[m] > box->upper[m])
        box->upper[m] = vertex3[m];
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

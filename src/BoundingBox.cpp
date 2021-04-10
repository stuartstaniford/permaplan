
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This manages the quadtree used for efficient organization of visual rendering

#include "BoundingBox.h"
#include "Shader.h"
#include <assert.h>
#include <math.h>
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// =======================================================================================
// Constructors

//NB!! Three constructors

BoundingBox::BoundingBox(vec3& bottomCorner, vec3& topCorner)
{
  glm_vec3_copy(bottomCorner, lower);
  glm_vec3_copy(topCorner, upper);
}


BoundingBox::BoundingBox(float x_l, float y_l, float z_l, float x_u, float y_u, float z_u)
{
  lower[0] = x_l;
  lower[1] = y_l;
  lower[2] = z_l;
  upper[0] = x_u;
  upper[1] = y_u;
  upper[2] = z_u;
}

// This last one is useful when we are about to search a bunch of vertices to find
// lower/upper bounds

BoundingBox::BoundingBox(void)
{
  lower[0] = HUGE_VALF;
  lower[1] = HUGE_VALF;
  lower[2] = HUGE_VALF;
  upper[0] = -HUGE_VALF;
  upper[1] = -HUGE_VALF;
  upper[2] = -HUGE_VALF;
}


// =======================================================================================
// Destructor

BoundingBox::~BoundingBox(void)
{
}


// =======================================================================================
// Give new values to an existing box.

void BoundingBox::reset(float x_l, float y_l, float z_l, float x_u, float y_u, float z_u)
{
  lower[0] = x_l;
  lower[1] = y_l;
  lower[2] = z_l;
  upper[0] = x_u;
  upper[1] = y_u;
  upper[2] = z_u;
}


// =======================================================================================
// Set up an existing box for vertex search on update

void BoundingBox::hugeValify(void)
{
  lower[0] = HUGE_VALF;
  lower[1] = HUGE_VALF;
  lower[2] = HUGE_VALF;
  upper[0] = -HUGE_VALF;
  upper[1] = -HUGE_VALF;
  upper[2] = -HUGE_VALF;
}


// =======================================================================================
// Make sure a bounding box has sensible values

bool BoundingBox::isDefined(void)
{
  if(lower[0] == HUGE_VALF)
    return false;
  return true;
}


// =======================================================================================
// Double check that all box values make sense as part of Quadtree::selfValidate

void BoundingBox::selfValidate(void)
{
  for(int m=0; m<3; m++)
   {
    assert(lower[0] > -HUGE_VALF);
    assert(upper[0] < HUGE_VALF);
    assert(lower[0] < upper[0]);
   }
}


// =======================================================================================
// Set Zs to nonsense values (to be overriden by assembling values from other boxes

void BoundingBox::unsetZs(void)
{
  lower[2] = HUGE_VALF;
  upper[2] = -HUGE_VALF;
}


// =======================================================================================
// Print short summary of bounding box (mostly for logging).

void BoundingBox::sprint(char* buf)
{
  sprintf(buf, "[[%.2f, %.2f, %.2f], [%.2f, %.2f, %.2f]]",
                      lower[0], lower[1], lower[2], upper[0], upper[1], upper[2]);
}


// =======================================================================================
// Put a single bounding box into a gpu buffer.  This is generally done with fixed color
// we don't handle texturing the bounding box.  This is mostly for debugging/diagnostic
// purposes.  Intended for use with GL_TRIANGLE_STRIP.  Requires 14 vertices

void BoundingBox::bufferGeometry(Vertex* buf)
{
  buf[0].setPosition(lower[0], upper[1], upper[2]); // Front-top-left
  buf[1].setPosition(upper[0], upper[1], upper[2]); // Front-top-right
  buf[2].setPosition(lower[0], lower[1], upper[2]); // Front-bottom-left
  
  buf[3].setPosition(upper[0], lower[1], upper[2]);     // Front-bottom-right
  buf[4].setPosition(upper[0], lower[1], lower[2]);    // Back-bottom-right
  buf[5].setPosition(upper[0], upper[1], upper[2]);      // Front-top-right
  
  buf[6].setPosition(upper[0], upper[1], lower[2]);     // Back-top-right
  buf[7].setPosition(lower[0], upper[1], upper[2]);     // Front-top-left
  buf[8].setPosition(lower[0], upper[1], lower[2]);    // Back-top-left
  
  buf[9].setPosition(lower[0], lower[1], upper[2]);    // Front-bottom-left
  buf[10].setPosition(lower[0], lower[1], lower[2]);   // Back-bottom-left
  buf[11].setPosition(upper[0], lower[1], lower[2]);    // Back-bottom-right
  
  buf[12].setPosition(lower[0], upper[1], lower[2]);    // Back-top-left
  buf[13].setPosition(upper[0], upper[1], lower[2]);      // Back-top-right
}


// =======================================================================================
// Put a single bounding box into a gpu buffer.  We manage our own buffer, call
// bufferGeometry(Vertex), and return a newly allocated VertexBufferCombo
// which can be used by the caller to draw.

VertexBufferCombo* BoundingBox::bufferGeometry(void)
{
  Vertex* buf = new Vertex[14];
  bufferGeometry(buf);
  VertexBufferCombo* combo = new VertexBufferCombo(14, buf);
  delete[] buf; buf = NULL;
  return combo;
}


// =======================================================================================
// Following a call to bufferGeometry(void), this will draw the bounding box.

void BoundingBox::draw(Shader& shader, VertexBufferCombo* combo, vec4 color)
{
  shader.setUniform("fixedColor", true);
  shader.setUniform("theColor", color);
  combo->bind();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
  shader.setUniform("fixedColor", false);
}


// =======================================================================================
// Give a ray specified by a position vector and a direction vector, determine if the
// ray intersects us.  Returns true or false.  Note, matches at negative multiples of
// the direction vector will not be counted - we seek the smallest positive multiple of
// direction (here stored in the reference arg lambda).

bool BoundingBox::matchRay(vec3& position, vec3& direction, float& lambda)
{
  // first deal with a bunch of quick cases where we are clearly pointed away from the box
  for(int i=0; i<3; i++)
   {
    if(position[i] < lower[i] && direction[i] < EPSILON)
      return false;
    if(position[i] > upper[i] && direction[i] > -EPSILON)
      return false;
   }

  // Now test each pair of faces orthogonal to the i'th axis
  for(int i=0; i<3; i++)
   {
    // We'll refer to the other two directions as a and b
    int a = (i+1)%3;
    int b = (i+2)%3;
    if(fabsf(direction[i]) < EPSILON)
      continue;
    if( (lambda = (lower[i] - position[i])/direction[i]) > 0.0f)
     {
      // Test each side of the square for this plane
      if(     (position[a] + lambda*direction[a] >= lower[a])
          &&  (position[a] + lambda*direction[a] <= upper[a])
          &&  (position[b] + lambda*direction[b] >= lower[b])
          &&  (position[b] + lambda*direction[b] <= upper[b]) )
        // We matched the lower face in the i'th direction
        return true;
     }
    if( (lambda = (upper[i] - position[i])/direction[i]) > 0.0f)
     {
      if(     (position[a] + lambda*direction[a] >= lower[a])
         &&  (position[a] + lambda*direction[a] <= upper[a])
         &&  (position[b] + lambda*direction[b] >= lower[b])
         &&  (position[b] + lambda*direction[b] <= upper[b]) )
        // We matched the upper face in the i'th direction
        return true;
     }
   }
  return false;
}


// =======================================================================================
// Find out if otherBox fits inside us (but only considering the x-y dimensions,
// not the z dimension.

bool BoundingBox::xyContains(const BoundingBox& otherBox)
{
  if(otherBox.lower[0] < lower[0])
    return false;
  if(otherBox.lower[1] < lower[1])
    return false;
  if(otherBox.upper[0] > upper[0])
    return false;
  if(otherBox.upper[1] > upper[1])
    return false;
  return true;
}


// =======================================================================================
// Find out if otherBox fits inside us (but only considering the z dimensions,
// not the x-y dimension.

bool BoundingBox::zContains(const BoundingBox& otherBox)
{
  if(otherBox.lower[2] < lower[2])
    return false;
  if(otherBox.upper[2] > upper[2])
    return false;
  return true;
}


// =======================================================================================
// Whether one box is entirely contained in another

bool BoundingBox::operator<=(const BoundingBox& B)
{
  if(lower[0] < B.lower[0] || upper[0] > B.upper[0])
    return false;
  if(lower[1] < B.lower[1] || upper[1] > B.upper[1])
    return false;
  if(lower[2] < B.lower[2] || upper[2] > B.upper[2])
    return false;
  
  return true;
}


// =======================================================================================
// Extend a box to include another

BoundingBox& BoundingBox::operator+=(const BoundingBox& B)
{
  for(int m=0; m<3; m++)
   {
    if(B.lower[0] < lower[0])
      lower[0] = B.lower[0];
    if(B.upper[0] > upper[0])
      upper[0] = B.upper[0];
   }
  return *this;
}


// =======================================================================================
// Extend our Z dimensions to include those of otherBox

bool BoundingBox::extendZ(const BoundingBox& otherBox)
{
  bool retVal = false;
  if(otherBox.lower[2] < lower[2])
   {
    retVal    = true;
    lower[2]  = otherBox.lower[2];
   }
  if(otherBox.upper[2] > upper[2])
   {
    retVal    = true;
    upper[2]  = otherBox.upper[2];
   }
  return retVal;
}


// =======================================================================================
// Extend any or all of our dimensions to include a supplied point.  Returns whether or
// not any extension occurred.

bool BoundingBox::extends(const vec3 point)
{
  bool retVal = false;
  for(int i=0; i<3; i++)
   {
    if(point[i] < lower[i])
     {
      lower[i] = point[i];
      retVal = true;
     }
    else if(point[i] > upper[i])
     {
      upper[i] = point[i];
      retVal = true;
     }
   }
  return retVal;
}


// =======================================================================================
// Provide a section of a diagnostic page about our parameters

bool BoundingBox::diagnosticHTML(HttpDebug* serv)
{
  // Start the section/table
  httPrintf("<center><h3>Bounds</h3>\n");
  serv->startTable();
  httPrintf("<tr><th></th><th>Bottom Coords</th><th>Top Coords</th>\n");
  
  // X co-ords
  httPrintf("<tr><td>X</td><td>");
  httPrintf("%.1f</td><td>%.1f</td></tr>\n", lower[0], upper[0]);
  
  // Y co-ords
  httPrintf("<tr><td>Y</td><td>");
  httPrintf("%.1f</td><td>%.1f</td></tr>\n", lower[1], upper[1]);
  
  // Z co-ords
  httPrintf("<tr><td>Z</td><td>");
  httPrintf("%.1f</td><td>%.1f</td></tr>\n", lower[2], upper[2]);
  
  // Conclude
  httPrintf("</table></center><hr>\n");
  return true;
}


// =======================================================================================

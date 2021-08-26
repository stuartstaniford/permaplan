// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is used for anything that has a axis-aligned 3D bounding box around it.
// It's used very heavily throughout the code (eg all VisualObjects have a bounding box)
// for a variety of cases of doing a first quick approximation (eg in first cut 
// intersection detection of rays, in deciding whether tree might shade each other, etc
// etc).

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
/// @brief Constructor from two corners.  
/// 
/// Note this is #1 of 3 constructors.
/// @param bottomCorner A reference to a vec3 of the lowest corner in all three x,y,z
/// dimensions.
/// @param topCorner A reference to a vec3 of the highest corner in all three dimensions.

BoundingBox::BoundingBox(vec3& bottomCorner, vec3& topCorner)
{
  glm_vec3_copy(bottomCorner, lower);
  glm_vec3_copy(topCorner, upper);
}


// =======================================================================================
/// @brief Constructor from explicit x,y,z bounds.  
/// 
/// Note this is #2 of 3 constructors.
/// @param x_l The lower x coordinate of the new bounding box.
/// @param x_u The upper x coordinate of the new bounding box.
/// @param y_l The lower y coordinate of the new bounding box.
/// @param y_u The upper y coordinate of the new bounding box.
/// @param z_l The lower z coordinate of the new bounding box.
/// @param z_u The upper z coordinate of the new bounding box.

BoundingBox::BoundingBox(float x_l, float y_l, float z_l, float x_u, float y_u, float z_u)
{
  lower[0] = x_l;
  lower[1] = y_l;
  lower[2] = z_l;
  upper[0] = x_u;
  upper[1] = y_u;
  upper[2] = z_u;
}


// =======================================================================================
/// @brief Constructor.  
/// 
/// Note this is #3 of 3 constructors.  This last one is useful when we are about to 
/// search a bunch of vertices to find lower/upper bounds.  It initializes all the lower
/// bounds to infinity and the upper bounds to minus infinity, so any points compared to
/// the initial bounding box are sure to extend it.  However, as directly constructed it
/// will be unusable until something has been added into it.

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
/// @brief Destructor currently doesn't need to do anything.

BoundingBox::~BoundingBox(void)
{
}


// =======================================================================================
/// @brief Give new values to an existing box.
/// @param x_l The lower x coordinate of the new bounding box.
/// @param x_u The upper x coordinate of the new bounding box.
/// @param y_l The lower y coordinate of the new bounding box.
/// @param y_u The upper y coordinate of the new bounding box.
/// @param z_l The lower z coordinate of the new bounding box.
/// @param z_u The upper z coordinate of the new bounding box.

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
/// @brief Set up an existing box for vertex search on update.
/// 
/// It initializes all the lower bounds to infinity and the upper bounds to minus 
/// infinity, so any points compared to the initial bounding box are sure to extend it.  

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
/// @brief Set an existing box to the unit cube centered on some position.
/// @param pos The position where the box should now be centered.

void BoundingBox::unitCube(vec3 pos)
{
  for(int m=0; m<3; m++)
   {
    lower[m] = pos[m] - 0.5f;
    upper[m] = pos[m] + 0.5f;
   }
}


// =======================================================================================
/// @brief Make sure a bounding box is not in the HUGE_VALF condition.
/// @returns True if the box has non-infinited values (only actually checks the lower[0]
/// dimension.  False otherwise.

bool BoundingBox::isDefined(void)
{
  if(lower[0] == HUGE_VALF)
    return false;
  return true;
}


// =======================================================================================
/// @brief Double check that all box values make sense as part of Quadtree::selfValidate.
///
/// This does assert() macros to test that all values are finite (and thus will bail
/// out of the program if they are not.
/// @param checkFlatBox.  If this is true, will also check that the box has non-zero extent
/// in each direction.  If false, just checks that lower is at least less than or equal
/// to upper.

void BoundingBox::selfValidate(bool checkFlatBox)
{
  for(int m=0; m<3; m++)
   {
    assert(lower[m] > -HUGE_VALF); // XX we should figure out some tighter bounds on these
    assert(upper[m] < HUGE_VALF);
    if(checkFlatBox)
      assert(lower[m] < upper[m]);
    else
      assert(lower[m] <= upper[m]);      
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
// Find the centroid of this bounding box

void BoundingBox::getCentroid(vec3 centroid)
{
  for(int m=0; m<3; m++)
    centroid[m] = (lower[m] + upper[m])/2.0f;
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
    if(B.lower[m] < lower[m])
      lower[m] = B.lower[m];
    if(B.upper[m] > upper[m])
      upper[m] = B.upper[m];
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
/// @brief Extend any or all of our dimensions to just include a supplied point.  
/// @returns whether or not any extension occurred.
/// @param point The vec3 of the position which our bounding box should now include.

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
    if(point[i] > upper[i])
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
  serv->startTable((char*)"BoundingBox");
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

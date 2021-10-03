// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -

#include "Box.h"
#include "HttpDebug.h"
#include <err.h>

#define BOX_ICOUNT 36u
#define BOX_VCOUNT 36u


// =======================================================================================
/// @brief Constructor.
///
/// The constructor initializes VisualObject with absolute heights set to on (ie boxes
/// don't adjust to the land surface - not clear on why this is so. 
/// @param transform - matrix transformation to be applied to the box.  If the identity
/// matrix is supplied, the box will be a unit cube at the origin.

Box::Box(mat4 transform):
                    VisualObject(true)
{
  glm_mat4_copy(transform, trans);
  updateBoundingBox();
  incrementBoxMemory(sizeof(Box)+sizeof(BoundingBox));
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Destructor
///
/// Doesn't do much at present, but track BoxMemory.

Box::~Box(void)
{
  incrementBoxMemory(-sizeof(Box)-sizeof(BoundingBox));
}


// =======================================================================================
/// @brief Interface for getting the vertices of the object one at a time.
///
/// This function that will cycle through all the vertices in the object, touching each 
/// vertex only once (thus normals and texture coordinates are poorly defined.

/// @param resetToFirst A boolean that if true says restart the vertex list from the beginning.
/// @param v A point to the vertex to be filled out.
/// @param detail A VertexDetail enum (see Vertex.h) as to whether to include texture and 
/// normal coordinates in addition to the position.

bool Box::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  if(detail > PositionOnly)
    err(-1,"Textures etc not implemented in Box::getNextUniqueVertex.\n");
  
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
// Iterator over indices.  Recall the underlying unit cube pre transformation is on axes
// from [0,0,0] to [1,1,1].  Refer to getNextVertex above for which vertex is which.  This
// function defines the triangles.  Recall counterclockwise winding order is front facing.
// https://learnopengl.com/Advanced-OpenGL/Face-culling

static int indexArray[BOX_ICOUNT] = {
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
  else if(index >= BOX_ICOUNT)
    return -1;
  else
    return indexArray[++index];
}


// =======================================================================================
//Generate all the vertices in order, with textures, normals, etc.

static Vertex vertexArray[BOX_VCOUNT] = {

  // Each line is
  // pos[0], pos[1], pos[2], color, tex[0], tex[1], norm[0], norm[1], norm[2]
  
  // Underside (z=0), close to origin
  {{0.0f, 0.0f, 0.0f}, 0u, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},  // 0
  {{0.0f, 1.0f, 0.0f}, 0u, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},  // 2
  {{1.0f, 0.0f, 0.0f}, 0u, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},  // 4

  // Underside (z=0), away from origin
  {{0.0f, 1.0f, 0.0f}, 0u, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},  // 2
  {{1.0f, 1.0f, 0.0f}, 0u, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},  // 6
  {{1.0f, 0.0f, 0.0f}, 0u, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},  // 4

  // Topside (z=1), close to origin
  {{0.0f, 0.0f, 1.0f}, 0u, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},   // 1
  {{1.0f, 0.0f, 1.0f}, 0u, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},   // 5
  {{0.0f, 1.0f, 1.0f}, 0u, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},   // 3

  // Topside (z=1), away from origin
  {{0.0f, 1.0f, 1.0f}, 0u, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},   // 3
  {{1.0f, 0.0f, 1.0f}, 0u, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},   // 5
  {{1.0f, 1.0f, 1.0f}, 0u, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},   // 7

  // Westside (x=0), close to origin
  {{0.0f, 0.0f, 0.0f}, 0u, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},  // 0
  {{0.0f, 0.0f, 1.0f}, 0u, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},  // 1
  {{0.0f, 1.0f, 0.0f}, 0u, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},  // 2

  // Westside (x=0), away from origin
  {{0.0f, 1.0f, 0.0f}, 0u, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},  // 2
  {{0.0f, 0.0f, 1.0f}, 0u, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},  // 1
  {{0.0f, 1.0f, 1.0f}, 0u, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},  // 3

  // Eastside (x=1), close to origin
  {{1.0f, 0.0f, 1.0f}, 0u, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},   // 5
  {{1.0f, 0.0f, 0.0f}, 0u, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},   // 4
  {{1.0f, 1.0f, 0.0f}, 0u, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},   // 6

  // Eastside (x=1), away from origin
  {{1.0f, 0.0f, 1.0f}, 0u, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},   // 5
  {{1.0f, 1.0f, 0.0f}, 0u, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},   // 6
  {{1.0f, 1.0f, 1.0f}, 0u, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},   // 7

  // Southside (y=0), close to origin
  {{0.0f, 0.0f, 0.0f}, 0u, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},  // 0
  {{1.0f, 0.0f, 0.0f}, 0u, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},  // 4
  {{0.0f, 0.0f, 1.0f}, 0u, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},  // 1

  // Southside (y=0), away from origin
  {{0.0f, 0.0f, 1.0f}, 0u, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},  // 1
  {{1.0f, 0.0f, 0.0f}, 0u, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},  // 4
  {{1.0f, 0.0f, 1.0f}, 0u, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},  // 5

  // Northside (y=1), close to origin
  {{0.0f, 1.0f, 0.0f}, 0u, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},   // 2
  {{0.0f, 1.0f, 1.0f}, 0u, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},   // 3
  {{1.0f, 1.0f, 0.0f}, 0u, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},   // 6

  // Northside (y=1), away from origin
  {{0.0f, 1.0f, 1.0f}, 0u, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},   // 3
  {{1.0f, 1.0f, 1.0f}, 0u, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},   // 7
  {{1.0f, 1.0f, 0.0f}, 0u, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},   // 6
};


// =======================================================================================
/// @brief Interface for getting the vertices of the object one at a time.
///
/// This interface is for a function that will cycle through all the vertices of all 
/// the triangles of the object.  

/// @param resetToFirst A boolean that if true says restart the vertex list from the beginning.
/// @param v A point to the vertex to be filled out.
/// @param detail A VertexDetail enum (see Vertex.h) as to whether to include texture and 
/// normal coordinates in addition to the position.

bool Box::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  if(resetToFirst)
    index = 0;
  else
    index++;
  if(index >= BOX_VCOUNT)
    return false;
  
  vec4 workVec4, out;
  
  glm_vec3_copy(vertexArray[index].pos, workVec4);
  workVec4[3] = 1.0f;
  
  glm_mat4_mulv(trans, workVec4, out);
  glm_vec3(out, (float*)v->pos);

  if(detail >= IncludeTexture)
    glm_vec2_copy(vertexArray[index].tex, v->tex);

  if(detail >= IncludeNormal)
   {
    vec3 normTemp;
    for(int j=0; j<3; j++)
      normTemp[j] = vertexArray[index].normal[j];
    glm_vec3_copy(normTemp, workVec4);
    workVec4[3] = 1.0f;
    glm_mat4_mulv(trans, workVec4, out);
    glm_vec3(out, normTemp);
    for(int j=0; j<3; j++)
      v->normal[j] = (__fp16)normTemp[j];
   }
  return true;
}


// =======================================================================================
/// @brief This is where the actual geometry of the box is defined - we render it into a 
/// buffer on request
/// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

bool Box::bufferGeometryOfObject(TriangleBuffer* T)
{
  Vertex* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, BOX_VCOUNT, BOX_ICOUNT))
   {
    LogTriangleBufferErrs("Box TriangleBuffer request for %u,%u failed at %u.\n",
                          BOX_VCOUNT, BOX_ICOUNT, vOffset);
    return false;
   }
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  int i;
  bool result;
  for(i=0, result=getNextVertex(true, vertices+i, IncludeNormal); result;
                          i++, result = getNextVertex(false, vertices+i, IncludeNormal))
   {
    if(useNoTexColor)
      vertices[i].setColor(noTexColor);
    vertices[i].setObjectId(objIndex);

    indices[i] = vOffset + i;
   }

  return true;
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer on a call to bufferGeometryToObject
///
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.

void Box::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = BOX_VCOUNT;
  iCount = BOX_ICOUNT;
  LogTriangleBufEstimates("Box TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
// Function to validate the quadtree and all the objects.

#ifdef LOG_TREE_VALIDATION

void Box::selfValidate(unsigned l)
{
  box->selfValidate(true);
}

#endif


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* Box::objectName(void)
{
  static char* name = (char*)"Box";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool Box::diagnosticHTMLSummary(HttpDebug* serv)
{
  httPrintf("<tr><td><a href=\"/object/%d\">Box</a></td>", objIndex);
  httPrintf("<td><b>Location:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                           box->lower[0], box->lower[1], box->lower[2]);
  
  return true;
}


// =======================================================================================
// Function to check the validity of some JSON in an OLDF file that purportedly represents
// a box.

using namespace rapidjson;

bool Box::validateOLDF(Value& boxJsonObject)
{
  /*  if(buildings.HasMember("blocks"))
     {
      blocksPresent = true;
      unless(buildings["blocks"].IsArray())
       {
        LogOLDFValidity("blocks is not an array in OLDF file %s\n", config.designFileName);
        return false;     
       }
      int N = buildings["blocks"].Size();
      for(int i=0; i<N; i++)
        retVal &= Box::validateOLDF(buildings["blocks"][i]);
     }
  */

  return true;  
}


// =======================================================================================

// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -
// Class for storing and rendering trees.

#include "Tree.h"
#include "PmodDesign.h"
#include <err.h>

unsigned short Tree::treeCount = 0u;
Tree** Tree::treePtrArray = new Tree*[TREE_ARRAY_SIZE];

using namespace rapidjson;

// =======================================================================================
// Constructors.  NB MORE THAN ONE CONSTRUCTOR!!!

/*
Tree::Tree(mat4 transform)
{
  glm_mat4_copy(transform, trans);
  updateBoundingBox();
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
}
*/

// Constructor used when starting a seedling from the interface
Tree::Tree(Species* S, vec3 loc):
                  species(S)
{
  glm_vec3_copy(loc, location);
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
}


// This is the constructor generally used when reading from OLDF.
Tree::Tree(Value& plantObject)
{
  char speciesPath[MAX_SPECIES_PATH];
  sprintf(speciesPath, "%s/%s", plantObject["genus"].GetString(),
                                            plantObject["species"].GetString());
  species = Species::getSpeciesByPath(speciesPath);
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
}


// =======================================================================================
// Destructor

Tree::~Tree(void)
{
}


// =======================================================================================
// Function that is applied to grow the tree by a certain number of years (possibly
// fractional).

void Tree::growStep(float years)
{
}


// =======================================================================================
// Static function that just grows all the trees known to treePtrArray.

void Tree::growAllTrees(float years)
{
  for(int i=0; i<treeCount; i++)
    treePtrArray[i]->growStep(years);
}


// =======================================================================================
// Iterator over vertices with each unique.  Doesn't do textures, normals, as they aren't
// very well defined for this purpose.

bool Tree::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
// Iterator over indices.  Recall the underlying unit cube pre transformation is on axes
// from [0,0,0] to [1,1,1].  Refer to getNextVertex above for which vertex is which.  This
// function defines the triangles.  Recall counterclockwise winding order is front facing.
// https://learnopengl.com/Advanced-OpenGL/Face-culling

int Tree::getNextIndex(bool resetToFirst)
{
  return 0;
}


// =======================================================================================
//Generate all the vertices in order, with textures, normals, etc.


bool Tree::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
// This is where the actual geometry is defined - we render it into a buffer
// on request

bool Tree::bufferGeometry(TriangleBuffer* T)
{
  Vertex* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, 36u, 36u))
    return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  int i;
  bool result;
  for(i=0, result=getNextVertex(true, vertices+i, IncludeNormal); result;
                          i++, result = getNextVertex(false, vertices+i, IncludeNormal))
   {
    if(useNoTexColor)
      vertices[i].setNoTexColor(noTexColor);
    indices[i] = vOffset + i;
   }

  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Tree::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
}


// =======================================================================================
// Stub definition

void Tree::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool Tree::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Compute the bounding box.
                                                                
void Tree::updateBoundingBox(void)
{
}

 
// =======================================================================================
// Static function which reads a bunch of entries from the plants section in the pmod
// design and instantiates the trees.

void Tree::readTreesFromDesign(void)
{
  PmodDesign& design = PmodDesign::getDesign();
 
  unless(design.doc.HasMember("plants")) // plants are optional
    return;

  // The structure was checked in PmodDesign::validatePlants(), so we assume it's correct.
  Value& plants = design.doc["plants"];
  int N = plants.Size();
  Tree* tree;
  for(int i=0; i<N; i++)
    tree = new Tree(plants[i]);
}


// =======================================================================================
// Tell callers our name at runtime.

const char* Tree::objectName(void)
{
  static char* name = (char*)"Tree";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool Tree::diagnosticHTML(HttpDebug* serv)
{
  
  serv->addResponseData("<tr><td>Tree</td>");
  return true;
}


// =======================================================================================

// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -
// Class for storing and rendering trees.

#include "Tree.h"
#include "WoodySegment.h"
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
                          species(S),
                          trunk(NULL)
{
  glm_vec3_copy(loc, location);
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
}


// This is the constructor generally used when reading from OLDF.
Tree::Tree(Value& plantObject):
                          trunk(NULL)
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
  // Handle the case of a brand new tree
  if(!trunk)
   {
    trunk = new WoodySegment(*species, treePtrArrayIndex, years);
    return;
   }
  
  // Handle the case of an existing tree by recursing into the trunk
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
  trunk->triangleBufferSizesRecurse(vCount, iCount);
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
// Write out one Tree's data to a plants object in OLDF JSON file.

void Tree::writeToOLDF(FILE* file, char* indent)
{
  fprintf(file, "%s {\n", indent);

  // location
  fprintf(file, "%s%s\"location\": [%f, %f],\n", indent, indent, location[0], location[1]);
  
  // yearPlanted - XX incomplete
  fprintf(file, "%s%s\"yearPlanted\": \"\",\n", indent, indent);

  // timePlanted - XX incomplete
  fprintf(file, "%s%s\"timePlanted\": \"[0,0]\",\n", indent, indent);

  // genus
  fprintf(file, "%s%s\"genus\": \"%s\",\n", indent, indent, species->genusName);

  // species
  fprintf(file, "%s%s\"species\": \"%s\",\n", indent, indent, species->speciesName);

  // var - XX missing
  
  // commonName - XX incomplete
  fprintf(file, "%s%s\"commonName\": \"\",\n", indent, indent);

  // notes - XX missing

  // treeDiameter - XX incomplete
  // Note this is the last one and shouldn't have the trailing comma.
  fprintf(file, "%s%s\"treeDiameter\": \"\"\n", indent, indent);

  fprintf(file, "%s }", indent);
}


// =======================================================================================
// Write out all the Tree data to a plants object in OLDF JSON file.

void Tree::writeTreesToOLDF(FILE* file, char* indent)
{
  // Open the object
  fprintf(file, "%s\"plants\":\n", indent);
  fprintf(file, "%s [\n", indent);

  for(int i=0; i<treeCount-1; i++)
   {
    treePtrArray[i]->writeToOLDF(file, indent);
    fprintf(file, ",\n");
   }
  if(treeCount > 0)
   {
    treePtrArray[treeCount-1]->writeToOLDF(file, indent);
    fprintf(file, "\n");
   }

  // Close out the plants object
  fprintf(file, "%s ],\n", indent);
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
  serv->respPtr += sprintf(serv->respPtr, "<tr><td>%d</td><td>%s %s</td></tr>\n",
                           treePtrArrayIndex, species->genusName, species->speciesName);
  return true;
}


// =======================================================================================
// Provide a diagnostic page about all the trees/plants

bool Tree::allTreeDiagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Trees");
  
  serv->addResponseData("<center>\n");
  serv->startTable();
  serv->addResponseData("<tr><th>Index</th><th>Species</th></tr>\n");
  
  for(int i=0; i< treeCount; i++)
    treePtrArray[i]->diagnosticHTML(serv);
  
  serv->addResponseData("</table></center><hr>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================

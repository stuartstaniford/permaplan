// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -
// Class for storing and rendering trees.

#include "Tree.h"
#include "WoodySegment.h"
#include "PmodDesign.h"
#include <err.h>
#include "Scene.h"

unsigned short Tree::treeCount = 0u;
Tree** Tree::treePtrArray = new Tree*[TREE_ARRAY_SIZE];

using namespace rapidjson;

// =======================================================================================
// Constructors.  NB MORE THAN ONE CONSTRUCTOR!!!

// Constructor used when starting a seedling from the interface

Tree::Tree(Species* S, vec3 loc, float age, float now):
                          species(S),
                          trunk(NULL)
{
  glm_vec3_copy(loc, location);
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
  ageNow      = now;
  yearPlanted = now - age;
  growStep(age);
  updateBoundingBox();
}


// =======================================================================================
// This is the constructor generally used when reading from OLDF.

Tree::Tree(Value& plantObject):
                          trunk(NULL)
{
  char speciesPath[MAX_SPECIES_PATH];
  sprintf(speciesPath, "%s/%s", plantObject["genus"].GetString(),
                                            plantObject["species"].GetString());
  species = Species::getSpeciesByPath(speciesPath);
  
  location[0] = plantObject["location"][0].GetFloat();
  location[1] = plantObject["location"][1].GetFloat();
  if(plantObject.HasMember("timePlanted"))
   {
    Timeval T;
    T.set(plantObject["timePlanted"][0].GetInt(), plantObject["timePlanted"][1].GetInt());
    yearPlanted = T.floatYear();
   }
  else
    yearPlanted = plantObject["yearPlanted"].GetFloat();
   
  if(SIMULATION_BASE_YEAR - yearPlanted > 0.0f)
    growStep(SIMULATION_BASE_YEAR - yearPlanted);
  else
    ageNow = SIMULATION_BASE_YEAR - yearPlanted;
  
  trunkRadiusObserved = plantObject["treeGirth"][0].GetFloat()*mmPerSpaceUnit/2.0f/M_PI;
  yearTrunkMeasured =   plantObject["treeGirth"][1].GetFloat();
  
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
  updateBoundingBox();
  LogTreeReads("Tree %d (%s %s) read in from file.\n", treePtrArrayIndex,
                                            species->genusName, species->speciesName);
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
  // Update age so all our parts know how old the tree is
  ageNow += years;  //XX precision could get marginal here if the simulation step is
                    // very small and the tree is hundreds of years old.

  LogTreeSimOverview("Growing tree %d (%s %s) by %.2f years to age %.2f.\n",
                                treePtrArrayIndex, species->genusName, species->speciesName,
                                years, ageNow);

  unless(ageNow < 0.0f) // the tree is in the future
   {
    // Handle the case of a brand new tree
    unless(trunk)
     {
      LogTreeSimDetails("Tree %d getting its new trunk.\n", treePtrArrayIndex);
      trunk = new WoodySegment(*species, treePtrArrayIndex, years, location);
     }
    else
     {
      LogTreeSimDetails("Tree %d growing trunk by %.2f years.\n",
                                    treePtrArrayIndex, years);
      trunk->growStep(years);
     }
   }
}


// =======================================================================================
// Static function that just grows all the trees known to treePtrArray.

void Tree::growAllTrees(float years)
{
  LogTreeSimOverview("Growing %d trees by %.2f years.\n", treeCount, years);
  for(int i=0; i<treeCount; i++)
    treePtrArray[i]->growStep(years);
}


// =======================================================================================
// Iterator over vertices with each unique.  Doesn't do textures, normals, as they aren't
// very well defined for this purpose.
// Not implemented here as the data is with the kids

bool Tree::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
// Iterator over indices.
// Not implemented here as the data is with the kids

int Tree::getNextIndex(bool resetToFirst)
{
  return 0;
}


// =======================================================================================
// Generate all the vertices in order, with textures, normals, etc.
// Not implemented here as the data is with the kids

bool Tree::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
// This routine is generally where the actual geometry is defined - but in our case,
// Tree itself doesn't directly define any geometry, and instead it's all based on
// recursion.

bool Tree::bufferGeometry(TriangleBuffer* T)
{
  if(trunk)
   {
    LogTreeVisualization("Buffering tree %d.\n", treePtrArrayIndex);
    LogTreeVisDetails("Trying to buffer trunk\n");
    unless(trunk->bufferGeometry(T))
      return false;
   }
  else
    LogTreeVisualization("Couldn't buffer emptry tree %d.\n", treePtrArrayIndex);

  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Tree::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
  if(trunk)
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
// Static function which reads a bunch of entries from the plants section in the pmod
// design and instantiates the trees.

void Tree::readTreesFromDesign(Quadtree* qtree)
{
  PmodDesign& design = PmodDesign::getDesign();
 
  unless(design.doc.HasMember("plants")) // plants are optional
    return;

  // The structure was checked in PmodDesign::validatePlants(), so we assume it's correct.
  Value& plants = design.doc["plants"];
  int N = plants.Size();
  Tree* tree;
  for(int i=0; i<N; i++)
   {
    tree = new Tree(plants[i]);
    qtree->storeVisualObject(tree);
   }
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
// Build a page about this particular tree with all the gory details of its treeparts.

// XX right now this is the version being called from the Qtree, which is not giving
// the correct result.

bool Tree::diagnosticHTML(HttpDebug* serv)
{
  // Page header
  char title[32+MAX_SPECIES_PATH];
  snprintf(title, 32+MAX_SPECIES_PATH, "Detail Page for Tree %d: %s %s",
                              treePtrArrayIndex, species->genusName, species->speciesName);
  serv->startResponsePage(title);
  
  // Summary Data about the tree
  httPrintf("<h2>Summary Data</h2>");
  httPrintf("<b>Location</b>: [%f, %f, %f]<br>", location[0], location[1], location[2]);
  httPrintf("<b>Species</b>: <a href=\"/species/%s/%s\">%s %s</a><br>",
            species->genusName, species->speciesName, species->genusName, species->speciesName);
  httPrintf("<hr>");
  
  // Table of treeparts
  httPrintf("<h2>Table of parts of this tree</h2>");
  serv->startTable();
  httPrintf("<tr><th>Type</th><th>Location</th><th>Direction</th>"
                                                    "<th>Other Information</th></tr>\n");
  if(trunk)
    unless(trunk->diagnosticHTML(serv))
      return false;
  httPrintf("</table></center><hr>\n");

  // Page closing
  serv->endResponsePage();
  return true;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool Tree::diagnosticHTMLRow(HttpDebug* serv)
{
  httPrintf("<tr><td><a href=\"/plants/%d\">%d</a></td>", treePtrArrayIndex,
                                                                    treePtrArrayIndex)
  httPrintf("<td><a href=\"/species/%s/%s/\">%s %s</a></td>", species->genusName,
                            species->speciesName, species->genusName, species->speciesName);
  httPrintf("<td>[%.2f, %.2f]</td>\n", location[0], location[1]);
  httPrintf("<td>%.2f</td>\n", yearPlanted);

  httPrintf("<td>%.2f</td><td>%.2f</td><td>%.2f</td>\n", ageNow,
            getHeight()*mmPerSpaceUnit, getRadius()*mmPerSpaceUnit*2.0f*M_PI);
  
  httPrintf("</tr>\n");

  return true;
}


// =======================================================================================
// Get the total height of the tree (in spaceUnits)

float Tree::getHeight(void)
{
  if(trunk)
    return ((WoodySegment*)trunk)->cylinder->getLength(); //XX maybe use bounding box here?
  else
    return 0.0f;
}


// =======================================================================================
// Get the trunk radio of the tree (in spaceUnits)

float Tree::getRadius(void)
{
  if(trunk)
    return ((WoodySegment*)trunk)->cylinder->radius;
  else
    return 0.0f;
}


// =======================================================================================
// Provide a diagnostic page with a table about all the trees/plants

bool Tree::allTreeDiagnosticHTML(HttpDebug* serv)
{
  httPrintf("Trees");
  
  httPrintf("<center>\n");
  serv->startTable();
  httPrintf("<tr><th>Index</th><th>Species</th><th>Location</th>"
              "<th>Planted</th><th>Age</th><th>Height</th><th>Girth</th></tr>\n");
  
  for(int i=0; i< treeCount; i++)
    treePtrArray[i]->diagnosticHTMLRow(serv);
  
  httPrintf("</table></center><hr>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================
// Figure out which kind of tree page is required, and call the appropriate method

bool Tree::treePageGateway(HttpDebug* serv, char* path)
{
  // No more path after plants/ so return table of all trees
  if(0 == strlen(path))
    return allTreeDiagnosticHTML(serv);
  
  // if only digits after plants/ return that tree if valid
  for(char* check = path; *check; check++)
    if(!isdigit(*check))
      return false;
  unsigned T = atoi(path);
  if(T < treeCount && treePtrArray[T])
    return treePtrArray[T]->diagnosticHTML(serv);
  
  return false;
}


// =======================================================================================

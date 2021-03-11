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
                          VisualObject(false),
                          species(S),
                          yearPlanted(now - age),
                          ageNow(age),
                          trunk(NULL)
{
  glm_vec3_copy(loc, location);
  location[2] = 0.0f;
  altitude = loc[2];
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
  growStep(age);
  updateBoundingBox();
  incrementTreeMemory(sizeof(Tree));
}


// =======================================================================================
// This is the constructor generally used when reading from OLDF.  Note that validation
// was done already by PmodDesign, so here we can safely assume the JSON is all
// syntactically correct.

Tree::Tree(Value& plantObject):
                          VisualObject(false),
                          trunk(NULL)
{
  char speciesPath[MAX_SPECIES_PATH];
  if(plantObject.HasMember("var"))
    snprintf(speciesPath, MAX_SPECIES_PATH, "%s/%s/%s", plantObject["genus"].GetString(),
                plantObject["species"].GetString(), plantObject["var"].GetString());
  else
    snprintf(speciesPath, MAX_SPECIES_PATH, "%s/%s", plantObject["genus"].GetString(),
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
  incrementTreeMemory(sizeof(Tree));
}


// =======================================================================================
// Destructor

Tree::~Tree(void)
{
  incrementTreeMemory(-sizeof(Tree));
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
      vec3 direction;
      direction[0] = 0.0f;
      direction[1] = 0.0f;
      direction[2] = years*species->stemRate; //XX not quite right if tree born mid-step
      trunk = new WoodySegment(*species, treePtrArrayIndex,
                                        0x0000, location, direction);  // treeMemory
      LogTreeSimDetails("Tree %d getting its new trunk, height %.2f.\n",
                        treePtrArrayIndex, years*species->stemRate);
     }
    else
     {
      LogTreeSimDetails("Tree %d growing trunk by %.2f years.\n",
                                    treePtrArrayIndex, years);
      trunk->growStep(years);
     }
    updateBoundingBox();
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
    unless(trunk->bufferGeometry(T, altitude))
      return false;
   }
  else
    LogTreeVisualization("Couldn't buffer empty tree %d.\n", treePtrArrayIndex);

  return true;
}


// =======================================================================================
// Function to return the location at which we officially contact ground/grade level.

void Tree::getGroundContact(float& x, float& y)
{
  if(!trunk)
   {
    x = location[0];
    y = location[1];
   }
  else
   {
    x = ((WoodySegment*)trunk)->cylinder->location[0];
    y = ((WoodySegment*)trunk)->cylinder->location[1];
   }
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Tree::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
  if(trunk)
    trunk->triangleBufferSizesRecurse(vCount, iCount);
  LogTriangleBufEstimates("Tree TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
// Function to validate the tree.

#ifdef LOG_TREE_VALIDATION

void Tree::selfValidate(unsigned l)
{
  assert(species);
  if(trunk)
   {
    assert(trunk->ourTreeIndex == treePtrArrayIndex);
    trunk->selfValidate(0u);
   }
  else
    assert(ageNow <= 0.0f);
}

#endif

// =======================================================================================
// Recurse into all our parts and update our bounding box

void Tree::updateBoundingBox(void)
{
#ifdef LOG_TREE_BOUNDING_BOX
  char buf[128];
#endif

  if(!box)
   {
    box = new BoundingBox();
#ifdef LOG_TREE_BOUNDING_BOX
    box->sprint(buf);
    LogTreeBoundingBox("Creating bounding box for tree %d: %s.\n", treePtrArrayIndex, buf);
#endif
   }
  
  if(trunk && trunk->updateBoundingBox(box, altitude))
   {
#ifdef LOG_TREE_BOUNDING_BOX
    box->sprint(buf);
    LogTreeBoundingBox("Updating bounding box for tree %d: %s\n", treePtrArrayIndex, buf);
#endif
    if(qTreeNode)
      qTreeNode->notifyObjectBoxChange(this);
   }
}


// =======================================================================================
// Test if a ray intersects us or not.  For trees we follow a hybrid approach - at a
// long distance, we just rely on the bounding box.  But close up, we recurse into the
// structure (which is expensive, especially if we don't match).

bool Tree::matchRay(vec3& position, vec3& direction, float& lambda)
{
  LogTreeMatchRay("Testing tree %d (box %.1f, %.1f; %.1f, %.1f; %.1f, %.1f) for ray match.\n",                       treePtrArrayIndex, box->lower[0], box->upper[0],
                          box->lower[1], box->upper[1], box->lower[2], box->upper[1]);
  unless(box->matchRay(position, direction, lambda))
    return false;
  LogTreeMatchRay("Tree %d ray matches box.\n", treePtrArrayIndex);

  // So it touches our bounding box, have to test all the branches.
  vec3 relativePos = {position[0], position[1], position[2]-altitude};
  if(trunk->matchRay(relativePos, direction))
    return true;  // XX note lambda based on bounding box not actual branch hit
  
  return false;
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
    tree = new Tree(plants[i]); //treeMemory
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
  static char nameBuf[32+MAX_SPECIES_PATH];
  
  if(species->varName)
    snprintf(nameBuf, 32+MAX_SPECIES_PATH, "Tree (%s %s %s)", species->genusName,
                                                      species->speciesName, species->varName);
  else
    snprintf(nameBuf, 32+MAX_SPECIES_PATH, "Tree (%s %s)", species->genusName,
                                                                      species->speciesName);
  return nameBuf;
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
  unless(serv->startResponsePage(title, 4))
    return false;
  
  // Summary Data about the tree
  httPrintf("<h2>Summary Data</h2>");
  httPrintf("<b>Location</b>: [%f, %f, %f]<br>", location[0], location[1], location[2]);
  if(species->varName)
   {
    httPrintf("<b>Species</b>: <a href=\"/species/%s/%s/%s\">%s %s %s</a><br>",
                            species->genusName, species->speciesName, species->varName,
                            species->genusName, species->speciesName, species->varName);
   }
  else
    httPrintf("<b>Species</b>: <a href=\"/species/%s/%s\">%s %s</a><br>",
            species->genusName, species->speciesName, species->genusName, species->speciesName);
  httPrintf("<hr>");
  
  unless(box->diagnosticHTML(serv))
    return false;
  
  // Table of treeparts
  httPrintf("<h2>Table of parts of this tree</h2>");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Type</th><th>Location (%c)</th><th>Direction (%c)</th>"
              "<th>Other Information</th></tr>\n", spaceUnitAbbr, spaceUnitAbbr);
  if(trunk)
    unless(trunk->diagnosticHTML(serv))
      return false;
  httPrintf("</table></center><hr>\n");

  // Page closing
  unless(serv->endResponsePage())
    return false;
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
            getHeight(), getRadius()*2.0f*M_PI);
  
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
  serv->startResponsePage("Trees");
  
  httPrintf("<center>\n");
  serv->startTable();
  httPrintf("<tr><th>Index</th><th>Species</th><th>Location (%c)</th><th>Planted</th>"
              "<th>Age (yr)</th><th>Height (%c)</th><th>Girth (%c)</th></tr>\n",
              spaceUnitAbbr, spaceUnitAbbr, spaceUnitAbbr);
  
  for(int i=0; i< treeCount; i++)
    treePtrArray[i]->diagnosticHTMLRow(serv);
  
  httPrintf("</table></center><hr>\n");
  if(!serv->endResponsePage())
    return false;
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
     {
      LogRequestErrors("Bad HTTP request path %s treePageGateway\n", path);
      return false;
     }
  unsigned T = atoi(path);
  if(T < treeCount && treePtrArray[T])
    return treePtrArray[T]->diagnosticHTML(serv);
  
  LogRequestErrors("Unhandled HTTP request %s in treePageGateway\n", path);
  return false;
}


// =======================================================================================

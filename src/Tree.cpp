// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -
// Class for storing and rendering trees.

#include "Tree.h"
#include "WoodySegment.h"
#include "PmodDesign.h"
#include "Scene.h"
#include "SkySampleModel.h"
#include "TaskQueueFarm.h"
#include <err.h>


// =======================================================================================
// Static variables.

unsigned short Tree::treeCount = 0u;
Tree** Tree::treePtrArray = new Tree*[TREE_ARRAY_SIZE];

using namespace rapidjson;

// =======================================================================================
/// @brief Constructor used when starting a seedling from the UI.
///
/// NB MORE THAN ONE CONSTRUCTOR!!! - See below
/// @param S    A pointer to the Species of this kind of tree
/// @param loc  A vec3 location of where the tree is to be placed
/// @param age  A floating point age (in years) of how old the tree is to be created as
/// @param now  A floating point represenation of the current year, so that the tree's 
/// year planted can be computed.
/// @todo Tree should inherit from Positionable, not have its own location
/// field.

Tree::Tree(Species* S, vec3 loc, float age, float now):
                          VisualObject(false),
                          species(S),
                          yearPlanted(now - age),
                          ageNow(age),
                          commonName(NULL),
                          taxonomyLink(NULL),
                          trunk(NULL)
{
  glm_vec3_copy(loc, location);
  location[2] = 0.0f;
  altitude = loc[2];
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
  growStep(age);
  updateBoundingBox();
  incrementTreeMemory(sizeof(Tree));
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Constructor used when reading from OLDF.
/// 
/// Note that validation was done already by PmodDesign, so here we can safely assume 
/// the JSON is all syntactically correct.
/// @param plantObject A rapidJson::Value reference to the OLDF/JSON representation of 
/// the tree.

Tree::Tree(Value& plantObject):
                          VisualObject(false),
                          trunk(NULL)
{
  // species/genus/var
  char speciesPath[MAX_SPECIES_PATH];
  if(plantObject.HasMember("var"))
    snprintf(speciesPath, MAX_SPECIES_PATH, "%s/%s/%s", plantObject["genus"].GetString(),
                plantObject["species"].GetString(), plantObject["var"].GetString());
  else
    snprintf(speciesPath, MAX_SPECIES_PATH, "%s/%s", plantObject["genus"].GetString(),
                                            plantObject["species"].GetString());
  species = Species::getSpeciesByPath(speciesPath);
  
  // location
  location[0] = plantObject["location"][0].GetFloat();
  location[1] = plantObject["location"][1].GetFloat();
  
  // time/year Planted
  if(plantObject.HasMember("timePlanted"))
   {
    Timeval T;
    T.set(plantObject["timePlanted"][0].GetInt(), plantObject["timePlanted"][1].GetInt());
    yearPlanted = T.floatYear();
   }
  else
    yearPlanted = plantObject["yearPlanted"].GetFloat();
  
  // commonName
  if(plantObject.HasMember("commonName"))
    commonName = plantObject["commonName"].GetString();
  else
    commonName = NULL;

  // taxonomyLink
  if(plantObject.HasMember("taxonomyLink"))
    taxonomyLink = plantObject["taxonomyLink"].GetString();
  else
    taxonomyLink = NULL;

  // ageNow
  if(SIMULATION_BASE_YEAR - yearPlanted > 0.0f)
    growStep(SIMULATION_BASE_YEAR - yearPlanted);
  else
    ageNow = SIMULATION_BASE_YEAR - yearPlanted;
  
  // treeGirth
  trunkRadiusObserved = plantObject["treeGirth"][0].GetFloat()*mmPerSpaceUnit/2.0f/M_PI;
  yearTrunkMeasured =   plantObject["treeGirth"][1].GetFloat();
  
  treePtrArray[(treePtrArrayIndex = treeCount++)] = this;
  updateBoundingBox();
  LogTreeReads("Tree %d (%s %s) read in from OLDF file.\n", treePtrArrayIndex,
                                            species->genusName, species->speciesName);
  setLabel(plantObject["notes"].GetString());
  incrementTreeMemory(sizeof(Tree));
}


// =======================================================================================
/// @brief Destructor

Tree::~Tree(void)
{
  incrementTreeMemory(-sizeof(Tree));
}


// =======================================================================================
/// @brief Simulate tree growth.
/// 
/// This method is applied to grow the tree by a certain number of years (possibly
/// fractional).
/// @param years A float value for the number of additional years to simulate the growth
/// of the tree.

void Tree::growStep(float years)
{
  // Update age so all our parts know how old the tree is
  ageNow += years;  //XX precision could get marginal here if the simulation step is
                    // very small and the tree is hundreds of years old.

  LogTreeSimOverview("Growing tree %d (%s %s) by %.2f years to age %.2f.\n",
                                treePtrArrayIndex, species->genusName, species->speciesName,
                                years, ageNow);

  if(ageNow < 0.0f) // the tree is in the future
    return;
    
  unless(trunk)
   {
    // Handle the case of a brand new tree
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


// =======================================================================================
/// @brief Estimate the likilihood that a ray of light will make it through our bounding 
/// box in a direction aligned with the specified axis.
/// @param axis The direction of estimation with 0=x, 1=y, 2=z.  This is used by 
/// Tree::estimateOpacity to approximate the general case of arbitrary directions.

float Tree::estimateOpacityAxially(int axis)
{
  int iDir = (axis+1)%3;
  int jDir = (axis+2)%3;
  vec3 position, direction;
  float lambda;
  int total = 0;
  direction[axis] = 1.0f;
  direction[iDir] = direction[jDir] = 0.0f;
  float iDirExtent = (box->upper[iDir] - box->lower[iDir])/OPACITY_ESTIMATE_FACTOR;
  float jDirExtent = (box->upper[jDir] - box->lower[jDir])/OPACITY_ESTIMATE_FACTOR;

  for(int i=0; i<OPACITY_ESTIMATE_FACTOR; i++)
    for(int j=0; j<OPACITY_ESTIMATE_FACTOR; j++)
     {
      position[axis] = box->lower[axis] - 1.0f;
      position[iDir] = box->lower[iDir] + (i+0.05f)*iDirExtent;
      position[jDir] = box->lower[jDir] + (j+0.05f)*jDirExtent;
      if(matchRayToObject(position, direction, lambda))
        total++;
     }
  
  return (float)total/OPACITY_ESTIMATE_FACTOR/OPACITY_ESTIMATE_FACTOR;
}


// =======================================================================================
/// @brief Estimate the likilihood that a ray of light will make it through our bounding 
/// box in a particular direction.  
/// 
/// This is used for approximating shading calculations (eg for other nearby trees being
/// shaded by this one.  Intended to work by interpolating between the values from 
/// Tree::estimateOpacityAxially
/// @returns A float value from 0 (completely transparent) to 1.0 (completely opaque).
/// @param direction A vec3 of the direction in which the caller would like to know the 
/// opacity.

float Tree::estimateOpacity(vec3 direction)
{
  return 0.0f;
}


// =======================================================================================
// C function to pass to TaskQueue.

#ifdef MULTI_THREADED_SIMULATION

void growOneTree(void* arg, TaskQueue* T)
{
  Tree* tree = (Tree*)arg;
  tree->growStep(tree->yearsToSim);
  
  threadFarm->notifyTaskDone();   
}


// =======================================================================================
// Static function that processes all the trees into a graph of copses, and assigns
// the simulation work to different threads.

void Tree::analyzeTreeGraph(float years, Scene& scene)
{
  SkySampleModel& sky = SkySampleModel::getSkySampleModel();
  int arraySize = treeCount*(treeCount-1)/2;  // store only below the diagonal of matrix
  unsigned edges[arraySize]; 
  unsigned bases[treeCount];
  
  //XX could we perhaps use quadtree to perform fewer comparisons here?
  //XX at least if the land was large compared to the biggest tree height
  for(int i=0, base = 0; i<treeCount; base+=i, i++)
   {
    bases[i] = base;
    for(int j=0; j<i; j++)
     {
      if(treePtrArray[i]->ageNow < 0.0f || treePtrArray[j]->ageNow < 0.0f)
       {
        edges[base+j] = TREES_INVALID;
        continue;
       }
      edges[base+j] = sky.treesInteract(treePtrArray[i]->box, treePtrArray[j]->box);
#ifdef LOG_TREE_GRAPH
      if(edges[base+j] & TREEi_SHADES_TREEj)
        LogTreeGraph("Tree %d shades tree %d.\n", i, j);
      if(edges[base+j] & TREEj_SHADES_TREEi)
        LogTreeGraph("Tree %d shades tree %d.\n", j, i);
      if(edges[base+j] & TREES_CLUSTER)
        LogTreeGraph("Trees %d and %d are together as copse.\n", j, i);
      unless(edges[base+j])
        LogTreeGraph("Trees %d and %d are unrelated.\n", j, i);      
#endif
     }
   }
  
  // Now need to analyze the matrix
  for(int i=0; i<treeCount; i++)
    treePtrArray[i]->taskId = -1;
  
  unsigned nextTaskId = 0u;
  for(int j=0; j<treeCount; j++)
   {
    if(treePtrArray[j]->taskId < 0)
     {
      LogTreeGraph("Assigning task-id %u to tree %d.\n", nextTaskId, j);     
      treePtrArray[j]->taskId = nextTaskId++;
     }
    // work down the columns
    for(int i=j+1; i < treeCount; i++)
     {
      unsigned edge = edges[bases[i] + j];
      if(edge & TREES_CLUSTER || ((edge & TREEj_SHADES_TREEi) && (edge & TREEi_SHADES_TREEj)))
       {
        LogTreeGraph("Assigning task-id %d to tree %d.\n", treePtrArray[j]->taskId, i);     
        treePtrArray[i]->taskId = treePtrArray[j]->taskId;
       }
     }
   }  

  // Now apportion the work to the threads

  for(int i=0; i<treeCount;i++)  
   {
    treePtrArray[i]->yearsToSim = years;
    threadFarm->addTask(i, growOneTree, treePtrArray[i]);
   }
  
  threadFarm->waitOnEmptyFarm();
}


// =======================================================================================
// Static function that just grows all the trees known to treePtrArray.

#else  // that is, not MULTI_THREADED_SIMULATION

void Tree::growAllTrees(float years)
{
  LogTreeSimOverview("Growing %d trees by %.2f years.\n", treeCount, years);
  for(int i=0; i<treeCount; i++)
   {
    treePtrArray[i]->growStep(years);
#ifdef LOG_TREE_OPACITY
    vec3 opacity;
    for(int m=0; m<3; m++)
      opacity[m] = treePtrArray[i]->estimateOpacityAxially(m);
    LogTreeOpacity("Tree %d has opacities x:%.1f%%, y:%.1f%%, z:%.1f%%.\n", i,
                   opacity[0]*100.0f, opacity[1]*100.0f, opacity[2]*100.0f);
#endif
   }
  
}

#endif // MULTI_THREADED_SIMULATION


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

bool Tree::bufferGeometryOfObject(TriangleBuffer* T)
{
  if(trunk)
   {
    LogTreeVisualization("Buffering tree %d.\n", treePtrArrayIndex);
    LogTreeVisDetails("Trying to buffer trunk\n");
    vec3 offset = {0.0f, 0.0f, altitude};
    unless(trunk->bufferGeometry(T, offset))
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
  if(ageNow > 0.0f)
    box->selfValidate(true);
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

bool Tree::matchRayToObject(vec3& position, vec3& direction, float& lambda)
{
  LogTreeMatchRay("Testing tree %d (box %.1f, %.1f; %.1f, %.1f; %.1f, %.1f) for ray match.\n",                       treePtrArrayIndex, box->lower[0], box->upper[0],
                          box->lower[1], box->upper[1], box->lower[2], box->upper[1]);
  unless(box->matchRay(position, direction, lambda))
    return false;
  LogTreeMatchRay("Tree %d ray matches box.\n", treePtrArrayIndex);

  // So it touches our bounding box, have to test all the branches.
  vec3 offset = {0.0f, 0.0f, altitude};
  if(trunk->matchRay(position, direction, offset))
    return true;
  
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
  
  // yearPlanted
  fprintf(file, "%s%s\"yearPlanted\": %.0f,\n", indent, indent, yearPlanted);

  // timePlanted - XX incomplete
  fprintf(file, "%s%s//\"timePlanted\": \"[0,0]\",\n", indent, indent);

  // genus
  fprintf(file, "%s%s\"genus\": \"%s\",\n", indent, indent, species->genusName);

  // species
  fprintf(file, "%s%s\"species\": \"%s\",\n", indent, indent, species->speciesName);

  // var
  if(species->varName)
    fprintf(file, "%s%s\"var\": \"%s\",\n", indent, indent, species->varName);
  
  // taxonomyLink
  if(taxonomyLink)
    fprintf(file, "%s%s\"taxonomyLink\": \"%s\",\n", indent, indent, taxonomyLink);

  // commonName
  if(commonName)
    fprintf(file, "%s%s\"commonName\": \"%s\",\n", indent, indent, commonName);

  // notes
  fprintf(file, "%s%s\"notes\": \"%s\",\n", indent, indent, label);

  // treeDiameter
  // trunkRadiusObserved = plantObject["treeGirth"][0].GetFloat()*mmPerSpaceUnit/2.0f/M_PI;
  // yearTrunkMeasured =   plantObject["treeGirth"][1].GetFloat();
  // Note this is the last one and shouldn't have the trailing comma.
  fprintf(file, "%s%s\"treeGirth\": [%f, %f]\n", indent, indent, 
                          trunkRadiusObserved*2.0f*M_PI/mmPerSpaceUnit, yearTrunkMeasured);

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
  fprintf(file, "%s ]\n", indent);
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
// about this particular object.  This is the version called from Quadtree pages

bool Tree::diagnosticHTMLSummary(HttpDebug* serv)
{
  httPrintf("<tr><td><a href=\"/plants/%d\">Tree %d</a></td>", treePtrArrayIndex,
                                                                    treePtrArrayIndex)
  httPrintf("<td><a href=\"/species/%s/%s/\">%s %s</a></td>", species->genusName,
                            species->speciesName, species->genusName, species->speciesName);  
  httPrintf("</tr>\n");

  return true;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.  This is the version in the "/plants" url.

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
  httPrintf("<td>%d</td>\n", taskId);
  
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
  unless(serv->startResponsePage("Trees"))
    return false;
  
  httPrintf("<center>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Index</th><th>Species</th><th>Location (%c)</th><th>Planted</th>"
              "<th>Age (yr)</th><th>Height (%c)</th><th>Girth (%c)</th><th>Task-id</th></tr>\n",
              spaceUnitAbbr, spaceUnitAbbr, spaceUnitAbbr);
  
  for(int i=0; i < treeCount; i++)
    treePtrArray[i]->diagnosticHTMLRow(serv);
  
  httPrintf("</table></center><hr>\n");
  unless(serv->endResponsePage())
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

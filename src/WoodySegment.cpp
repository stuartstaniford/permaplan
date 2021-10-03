// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#include "WoodySegment.h"
#include "PmodDesign.h"
#include "Tree.h"
#include "JSONStructureChecker.h"
#include "TruncatedCone.h"
#include "LeafModel.h"
#include "Species.h"
#include <err.h>


// =======================================================================================
// Constructors.

WoodySegment::WoodySegment(Species& species, unsigned short treeIndex,
                                                  unsigned short lev, vec3 loc, vec3 dir):
                              TreePart(treeIndex),
                              heartRadius(0.0f),
                              sapThickness(species.initSapThickness),
                              barkThickness(species.initBarkThickness),
                              barkColor(0u),
                              level(lev)
{
  float radius = heartRadius + sapThickness + barkThickness;
  if(level < 2)
    cylinder = (AxialElement*)new TruncatedCone(loc, dir, radius/10.0f, radius, WOOD_SEG_SIDES);
  else
    cylinder = (AxialElement*)new Cylinder(loc, dir, radius, WOOD_SEG_SIDES);

  cylinder->closedTop = true;
  incrementTreeMemory(sizeof(WoodySegment)+sizeof(Cylinder));
}


// =======================================================================================
// Destructor

WoodySegment::~WoodySegment(void)
{
  delete cylinder;
  incrementTreeMemory(-sizeof(WoodySegment)-sizeof(Cylinder));
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      delete kids[i];
}


// =======================================================================================
// Update a supplied bounding box.

bool WoodySegment::updateBoundingBox(BoundingBox* box, float altitude)
{
  LogTreeBoundingBox("Updating the bounding box for tree %d, WoodySegment at level %u.\n",
                                                                          ourTreeIndex, level);
  bool retVal = false;
  vec3 offset = {0.0f, 0.0f, altitude};
  if(cylinder->updateBoundingBox(box, offset))
   {
    LogTreeBoundingBox("Bounding box changed! for tree %d, WoodySegment at level %u.\n",
                                                                            ourTreeIndex, level);
    retVal = true;
   }
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i] && kids[i]->updateBoundingBox(box, altitude))
      retVal = true;
  return retVal;
}


// =======================================================================================
// Function to validate the tree.

#ifdef LOG_TREE_VALIDATION

void WoodySegment::selfValidate(unsigned l)
{
  assert(cylinder);
  
  int N = kids.size();
  for(int i=0; i< N; i++)
   {
    assert(kids[i]);
    assert(kids[i]->getTreeIndex() == ourTreeIndex);
    kids[i]->selfValidate(l+1);
   }
}

#endif


// =======================================================================================
// Match a ray.  Subclassses need to implement this so we can tell whether the mouse
// is over the tree or not.

bool WoodySegment::matchRay(vec3& position, vec3& direction, vec3 offset)
{
  float L;
  if(cylinder->matchRayToElement(position, direction, L, offset))
   {
    LogTreeMatchRay("Tree %d ray matches WoodySegment (level %d) at "
                    "[%.1f, %.1f, %.1f] dir [%.1f, %.1f, %.1f].\n", ourTreeIndex, level,
                    cylinder->location[0], cylinder->location[1],
                    cylinder->location[2], cylinder->axisDirection[0],
                    cylinder->axisDirection[1], cylinder->axisDirection[2]);
    return true;
   }
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i]->matchRay(position, direction, offset))
      return true;
  return false;
}


// =======================================================================================
// Buffer the vertices/indices for this part.

bool WoodySegment::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  cylinder->color = barkColor;
  unless(cylinder->bufferGeometryOfElement(T, offset))
    return false;
  
  LogTreeVisDetails("Buffering WoodySegment "
                    "(Loc: [%.1f, %.1f, %.1f], Dir: [%.1f, %.1f, %.1f],"
                    "heartR: %.1f, sapT: %.1f, barkT: %.1f\n",
                    cylinder->location[0], cylinder->location[1],
                    cylinder->location[2] + altitude, cylinder->axisDirection[0],
                    cylinder->axisDirection[1], cylinder->axisDirection[2],
                    heartRadius*mmPerSpaceUnit, sapThickness*mmPerSpaceUnit,
                    barkThickness*mmPerSpaceUnit);
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
     {
      LogTreeVisDetails("Trying to buffer kid[%d]\n", i);
      unless(kids[i]->bufferGeometry(T, offset))
        return false;
     }
  return true;
}


// =======================================================================================
// Assess the number of vertices/indices required in a triangle buffer to render us
// and all our children.

void WoodySegment::triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount)
{
  // Add ourselves
  unsigned V, I;
  cylinder->triangleBufferSizes(V, I);
  vCount += V;
  iCount += I;
  
  // Add our branches, etc
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      kids[i]->triangleBufferSizesRecurse(vCount, iCount);
}


// =======================================================================================
// Function that defines how many branches we should now have, based on our length, etc.

unsigned WoodySegment::expectedKids(float len)
{
  const Species& ourSpecies = *(Tree::treePtrArray[ourTreeIndex]->species);
  
  return lround(len/ourSpecies.branchSpacing-0.5f) // never round up
                                        *ourSpecies.branchFactor;
}


// =======================================================================================
// Function that is applied to grow the tree by a certain number of years (possibly
// fractional).

void WoodySegment::growStep(float years)
{
  // Do ourselves.
  Tree&     ourTree = *(Tree::treePtrArray[ourTreeIndex]);
  Species&  ourSpecies = *(ourTree.species);
  float len;
  
  if(level == 0)  // trunk case
   {
    //XX use of ourTree.ageNow is temp hack - need to model ages of each stem/branch
    ourSpecies.logisticGrowthModel(ourTree.ageNow, cylinder->radius, len);
    barkColor = ourSpecies.getBarkColor(ourTree.ageNow);
   }
  else // branches
   {
    float treeHeight = ((WoodySegment*)ourTree.trunk)->cylinder->getLength();
    float ourHeight = cylinder->location[2] -
                                    ((WoodySegment*)ourTree.trunk)->cylinder->location[2];
    if(level == 1)
     {
      barkColor = ourSpecies.getBarkColor(ourTree.ageNow);
      len = (treeHeight-ourHeight)*ourSpecies.maxWidth/(2.0f*ourSpecies.maxHeight);
     }
    else
     {
      barkColor = ourSpecies.foliage->leafColors[displaySeason];
      len = (treeHeight-ourHeight)*ourSpecies.maxWidth/(6.0f*ourSpecies.maxHeight);
     }
    cylinder->radius = len/40.0f;   //XX braindead branch thickness

   }
  
  cylinder->setLength(len);
  sapThickness = cylinder->radius - heartRadius - barkThickness; //XX obviously braindead
    
#ifdef LOG_TREE_SIM_DETAILS
  char buf[32];
  vec3& loc = cylinder->location;
  RGBArrayFromColor(barkColor, buf);
  LogTreeSimDetails("Woody Segment at loc [%.1f, %.1f, %.1f] growing to length %.1f%c,"
                          "girth %.1f%c, color %s\n",
                          loc[0], loc[1], loc[2],
                          cylinder->getLength(), spaceUnitAbbr,
                          cylinder->radius*2.0f*M_PI, spaceUnitAbbr,
                          buf);
#endif

  // See if we need to make new kids
  int N = kids.size();
  if(level < 2) // only branching from the trunk and primary branches right now
   {
    unsigned e = expectedKids(len);
    if(e > N)
     {
      LogTreeSimDetails("WoodySegment at [%.1f, %.1f, %.1f] will create %d kids\n",
                                                            loc[0], loc[1], loc[2], e-N);
      for(int i=N; i<e; i++)
       {
        vec3 tempDir, branchLoc, branchDir;
        // Compute location of root of branch, branchDir is intermediate variable
        int branchPoint = i/ourSpecies.branchFactor;  // integer division intended
        glm_vec3_scale_as(cylinder->axisDirection,
                                      branchPoint*ourSpecies.branchSpacing, tempDir);
        glm_vec3_add(cylinder->location, tempDir, branchLoc);
        
        // now compute branchDir as the direction of the new branch
        vec3 currentDir;
        glm_vec3_copy(cylinder->axisDirection, currentDir);
  
        if(i==0) // first branch, no prior branch to work with.
         {
          //XX this approach to first branch direction needs work.
          //XX only works for trunk case.
          vec3 northVec = {0.0f, 1.0f, 0.0f};
          glm_vec3_copy(currentDir, tempDir);
          glm_vec3_rotate(tempDir, ourSpecies.branchAngle*M_PI/180.0f, northVec);
          
         }
        else // easier case, just working from the prior branch
         {
          glm_vec3_copy(((WoodySegment*)kids[i-1])->cylinder->axisDirection, tempDir);
          if(i%ourSpecies.branchFactor) // middle of a set of branches circling the tree
            glm_vec3_rotate(tempDir, 2.0f*M_PI/ourSpecies.branchFactor, currentDir);
          else // first of a new set of branches - use spiral angle
            glm_vec3_rotate(tempDir, ourSpecies.branchSpiralAngle*M_PI/180.0f, currentDir);
         }
        glm_vec3_scale_as(tempDir, years*ourSpecies.stemRate, branchDir); //XX stem born mid-step

        // Now we have everything needed to add the new child to our kids
        WoodySegment* branch = new WoodySegment(ourSpecies, ourTreeIndex,
                                                level + 1, branchLoc, branchDir);
        kids.push_back(branch);
        LogTreeSimDetails("WoodySegment at [%.1f, %.1f, %.1f] created branch %d with "
                          "loc: [%.1f, %.1f, %.1f]; dir [%.1f, %.1f, %.1f]\n",
                          loc[0], loc[1], loc[2], i,
                          branchLoc[0], branchLoc[1], branchLoc[2],
                          branchDir[0], branchDir[1], branchDir[2]);
       }
     }
   }
  
  // Recurse into our kids
  for(int i=0; i<N; i++)
    if(kids[i])
      kids[i]->growStep(years);
}

  
// =======================================================================================
// Function to print out in JSON format.

#define bufprintf(...) if((buf += snprintf(buf, end-buf,  __VA_ARGS__)) >= end) {return -1;}

int WoodySegment::printOPSF(char*& buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  // XX this isn't quite formulated right at present.  We are starting as though we are within
  // an object, yet putting the kids in an array.
  
  // Our own information
  bufprintf("\"woodySegment\": {\n");
  bufprintf("\"heartRadius\": %.1f,\n", heartRadius*mmPerSpaceUnit);
  bufprintf("\"sapThickness\": %.1f,\n", sapThickness*mmPerSpaceUnit);
  bufprintf("\"barkThickness\": %.1f,\n", barkThickness*mmPerSpaceUnit);
  
  // Cylinder info
  cylinder->printOPSF(buf, (unsigned)(end-buf));
  bufprintf(",\n");

  // Our kids
  bufprintf("kids = [\n");
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
     {
     }
  bufprintf("]\n"); // kids array is last thing inside our object, so no comma

  // Close up and go home.
  bufprintf("}"); // Note no ",\n" - caller must supply if necessary
  return (bufSize - (int)(end-buf));
}


// =======================================================================================
// Tell callers our name at runtime.

const char* WoodySegment::objectName(void)
{
  static char* name = (char*)"WoodySegment";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool WoodySegment::diagnosticHTML(HttpDebug* serv)
{
  // Print our info
  httPrintf("<tr><td>WoodySegment</td><td>[%.1f, %.1f, %.1f]</td><td>[%.1f, %.1f, %.1f]</td>",
              cylinder->location[0], cylinder->location[1], cylinder->location[2],
              cylinder->axisDirection[0], cylinder->axisDirection[1],
              cylinder->axisDirection[2]);
  httPrintf("<td>");
  httPrintf("<b>Level:</b> %u; ", level);
  httPrintf("<b>Radius:</b> %.4f%c; ", cylinder->radius, spaceUnitAbbr);
#ifdef LOG_TREE_MATCH_RAY
  httPrintf("<b>Ray dist:</b> %.2f%c; ", cylinder->lastRayMatch, spaceUnitAbbr);
  httPrintf("<b>Joinline:</b> [%.2f, %.2f, %.2f]; ", cylinder->joinLine[0],
            cylinder->joinLine[1], cylinder->joinLine[2]);
  httPrintf("<b>OrigDiff:</b> [%.2f, %.2f, %.2f]; ", cylinder->originDiff[0],
            cylinder->originDiff[1], cylinder->originDiff[2]);
#endif
  char buf[32];
  RGBArrayFromColor(barkColor, buf);
  httPrintf("<b>Color:</b> %s", buf);
  httPrintf("</td></tr>\n");

  // Recurse into our branches, leaves, etc
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      unless(kids[i]->diagnosticHTML(serv))
        return false;
         
  return true;
}


// =======================================================================================

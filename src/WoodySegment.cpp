// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#include "WoodySegment.h"
#include "PmodDesign.h"
#include "Tree.h"
#include "JSONStructureChecker.h"
#include <err.h>


// =======================================================================================
// Constructors.

WoodySegment::WoodySegment(Species& species, float years, unsigned short treeIndex,
                           unsigned short lev, vec3 location):
                              TreePart(treeIndex),
                              heartRadius(0.0f),
                              sapThickness(species.initSapThickness),
                              barkThickness(species.initBarkThickness),
                              barkColor(0u),
                              level(lev)
{
  vec3 direction;
  direction[0] = 0.0f;
  direction[1] = 0.0f;
  direction[2] = years*species.stemRate;
  cylinder = new Cylinder(location, direction,
                          heartRadius + sapThickness + barkThickness, WOOD_SEG_SIDES);
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
// Buffer the vertices/indices for this part.

bool WoodySegment::bufferGeometry(TriangleBuffer* T, float altitude)
{
  unless(cylinder->bufferGeometry(T, altitude, barkColor))
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
      unless(kids[i]->bufferGeometry(T, altitude))
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
// Function that is applied to grow the tree by a certain number of years (possibly
// fractional).

void WoodySegment::growStep(float years)
{
  // Do ourselves.
  Tree* ourTree = Tree::treePtrArray[ourTreeIndex];
  float len;
  //XX use of ourTree->ageNow is temp hack - need to model ages of each stem/branch
  ourTree->species->logisticGrowthModel(ourTree->ageNow, cylinder->radius, len);
  cylinder->setLength(len);
  sapThickness = cylinder->radius - heartRadius - barkThickness; //XX obviously braindead
  barkColor = ourTree->species->getBarkColor(ourTree->ageNow);
  
#ifdef LOG_TREE_SIM_DETAILS
  char buf[32];
  RGBArrayFromColor(barkColor, buf);
  LogTreeSimDetails("Woody Segment at loc [%.1f, %.1f, %.1f] growing to length %.0f,"
                          "girth %.0f, color %s\n",
                          cylinder->location[0], cylinder->location[1], cylinder->location[2],
                          cylinder->radius*mmPerSpaceUnit*2.0f*M_PI,
                          cylinder->getLength()*mmPerSpaceUnit, buf);
#endif

  // Recurse into our kids
  int N = kids.size();
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
  char buf[32];
  RGBArrayFromColor(barkColor, buf);
  httPrintf("<b>Color:</b> %s", buf);
  httPrintf("</td></tr>");

  // Recurse into our branches, leaves, etc
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      unless(kids[i]->diagnosticHTML(serv))
        return false;
         
  return true;
}


// =======================================================================================

// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// This is where the implementation of the logging system goes (most of it is macros
// in Logging.h)

// =======================================================================================

#define LOGGING_IMPLEMENTATION

#include <err.h>
#include <stdio.h>
#include "Logging.h"
#include "Vertex.h"


// =======================================================================================
// Variables that control real time logging when compiled in

// Logging options to do with overall control flow and speed
bool doLogFrameStarts         = true; // Log each frame as it begins
bool doLogCloseDown           = true; // Log when we exit
bool doLogStructureSizes      = true; // Log the size of structures on this platform at startup.
bool doLogOpenGLConstants     = true; // Log various openGL parameters

// Logging options to do with parsing and validating OLDF and OTDL files
bool doLogOLDFValidity        = true; // Log validity problems in the file
bool doLogOLDFDetails         = true; // Log details of valid OLDF objects
bool doLogOTDLValidity        = true; // Log validity problems in an OTDL object
bool doLogOTDLDetails         = true; // Log details of valid OTDL objects

// Logging options to do with interface controls
bool doLogMouseLocation       = true; // where the mouse is on the screen each frame
bool doLogMouseClick          = true; // Location of single mouse clicks
bool doLogDoubleClick         = true; // Location and intra-click timing of double clicks
bool doLogMouseRayPoint       = true; // Where the mouse is pointing in 3d space
bool doLogLandHeights         = true; // Log when a new land height is entered.
bool doLogObjectInsertions    = true; // Log when a new object is inserted in scene.
bool doLogSimulationControls  = true; // Log control operations on the simulation.

// Logging options to do with materials and textures
bool doLogTextureAtlas        = true; // Log texture atlas creation
bool doLogAtlasAnomalies      = true; // Log weird conditions in texture atlas creation
bool doLogAtlasPlacement      = true; // Log details of which image is where in atlas
bool doLogMaterialSelections  = true; // Log when a material is selected.
bool doLogPathMap             = true; // Log storage and use of the path map

// Logging options for Trees/Plants
bool doLogTreeSelections      = true; // Log when a tree is selected.
bool doLogTreeReads           = true; // Log when a tree is read in from a file or url
bool doLogTreeSimOverview     = true; // Log high level actions in simulating tree growth.
bool doLogTreeSimDetails      = true; // Log all the gory details of simulated tree growth.
bool doLogGrowthModel         = true; // Log details of the growth model.
bool doLogTreeMatchRay        = true; // Log matching a ray to a tree
bool doLogTreeVisualization   = true; // Log trees being rendered.
bool doLogTreeVisDetails      = true; // Log every twig being rendered.
bool doLogBarkDisplay         = true; // Log bark color and texture details.
bool doLogTreeErrors          = true; // Log clear errors in tree related operations.

// Logging options for the HTTP debug interface
bool doLogRequestErrors       = true; // Log validity problems in the HTTP request
bool doLogResponseErrors      = true; // Log problems encountered building the response
bool doLogHTTPDetails         = true; // Log normal details of HTTP operations

// Logging options to do with quadtree operations
bool doLogQuadtreeCreation    = true; // Log the initial setup of the quadtree.
bool doLogQuadtreeInsertions  = true; // Log the a new object being put in the quadtree.
bool doLogDisplayListBuffer   = true; // Log objects being buffered for display.
bool doLogQuadtreeObjSizes    = true; // Log the process of estimating/changing object sizes

// Logging options to do with other infrastructure
bool doLogTriangleBufferOps     = true; // Log the workings of the triangle buffers.
bool doLogTriangleBufEstimates  = true; // Log estimates of sizes needed.
bool doLogTriangleBufferErrs    = true; // Log actual errors the triangle buffers.

// Logging options to do with the Bezier Patch code
bool doLogBezierFit           = true; // Log the fitting of a Bezier patch to height data
bool doLogBezierMatchRay      = true; // Log the process of matching a ray to the patch.
bool doLogObjectAltitudes     = true; // Log finding the altitudes of objects above the land


// =======================================================================================
// Function to start up logging

FILE* LogFile;

void LogInit(void)
{
  char* fileName = (char*)"permaplan.log";
  
  LogFile = fopen(fileName, "w");
  if(!LogFile)
    err(-1, "Couldn't create log file %s\n", fileName);
  
  LogStructureSizes("Size of Vertex: %lu bytes.\n", sizeof(Vertex));
  LogStructureSizes("Size of unsigned: %lu bytes.\n", sizeof(unsigned));
  LogStructureSizes("Size of unsigned long: %lu bytes.\n", sizeof(unsigned long));
  LogStructureSizes("Size of unsigned long long: %lu bytes.\n", sizeof(unsigned long long));

}

// =======================================================================================

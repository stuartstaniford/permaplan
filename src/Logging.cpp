// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// This is where the implementation of the logging system goes (most of it is macros
// in Logging.h)

// =======================================================================================

#define LOGGING_IMPLEMENTATION

#include "Logging.h"
#include "Vertex.h"
#include "HttpDebug.h"
#include "Shed.h"
#include <err.h>
#include <stdio.h>


// =======================================================================================
// Variables that control real time logging when compiled in

// Logging options to do with overall control flow and speed
bool doLogFrameStarts         = true; // Log each frame as it begins
bool doLogCloseDown           = true; // Log when we exit
bool doLogStructureSizes      = true; // Log the size of structures on this platform at startup.
bool doLogEnvVars             = true; // Log the value of environment variables we care about.
bool doLogOpenGLConstants     = true; // Log various openGL parameters
bool doLogTreeValidation      = true; // Validate data structures after every frame

// Logging options to do with resource management and collection
bool doLogResourceErrors        = true; // Clear errors in the resource manager
bool doLogResourceActions       = true; // Stuff the resource manager actually does
bool doLogResourceDetails       = true; // Micro-detail of the resource manager operating
bool doLogHttpClientErrors      = true; // Log things going wrong in the HTTP Client
bool doLogPermaservClientErrors = true; // Log things going wrong in the Permaserv Client layer
bool doLogPermaservClientOps    = true; // Log normal operations in the Permaserv Client layer
bool doLogPermaservOps          = true; // Log normal operation of the Permaserv server
bool doLogPermaservOpDetails    = true; // Log detailed operation of Permaserv for debugging

// Logging options to do with parsing and validating OLDF and OTDL files
bool doLogOLDFValidity        = true; // Log validity problems in the file
bool doLogOLDFDetails         = true; // Log details of valid OLDF objects
bool doLogOTDLValidity        = true; // Log validity problems in an OTDL object
bool doLogOTDLDetails         = true; // Log details of valid OTDL objects

// Logging options to do with interface controls
bool doLogMouseLocation       = true; // where the mouse is on the screen each frame
bool doLogMouseClick          = true; // Location of single mouse clicks
bool doLogDoubleClick         = true; // Location and intra-click timing of double clicks
bool doLogClickDetails        = true; // Extra detailed recording of parts of click detection
bool doLogMouseRayPoint       = true; // Where the mouse is pointing in 3d space
bool doLogLandHeights         = true; // Log when a new land height is entered.
bool doLogObjectInsertions    = true; // Log when a new object is inserted in scene.
bool doLogDumpObjectBuffer    = true; // Dump buffer when a new object is inserted.
bool doLogSimulationControls  = true; // Log control operations on the simulation.
bool doLogWindowOperations    = true; // Log changes to the window (eg resizing).
bool doLogCameraFrames        = true; // Log camera variables every frame
bool doLogGLFWErrors          = true; // Log when the GLFW library needs to report errors

// Logging options to do with materials and textures
bool doLogTextureAtlas        = true; // Log texture atlas creation
bool doLogAtlasAnomalies      = true; // Log weird conditions in texture atlas creation
bool doLogAtlasPlacement      = true; // Log details of which image is where in atlas
bool doLogMaterialSelections  = true; // Log when a material is selected.
bool doLogPathMap             = true; // Log storage and use of the path map

// Logging options to do with sky sampling and lighting
bool doLogSkySampleInit       = true; // Log the setup of the sky sampling model
bool doLogGdalError           = true; // Log problems in accessing files via libgdal

// Logging options to do with soil stuff
bool doLogSoilDbErr           = true; // Log problems in the soil database operation
bool doLogSoilDbOps           = true; // Log normal operations on the soil database
bool doLogSoilDbDetails       = true; // Log extra details in the soil database operation
bool doLogHSWDExhaustive      = true; // Log every field read from the HSWD database
bool doLogBilFileDetails      = true; // Log details of reading from a BILFILE

// Logging options to do with climate data processing
bool doLogClimateDbErr        = true; // Log problems in the climate database operation
bool doLogClimateDbOps        = true; // Log normal operations on the climate database
bool doLogClimateOutliers     = true; // Log climate observation outlier exclusion
bool doLogClimateCompDetails  = true; // Log details of climate station comparisons
bool doLogGHCNExhaustive      = true; // Log minute details of reading the GHCN info

// Logging options for Trees/Plants
bool doLogTreeSelections      = true; // Log when a tree is selected.
bool doLogTreeReads           = true; // Log when a tree is read in from a file or url
bool doLogTreeSimOverview     = true; // Log high level actions in simulating tree growth.
bool doLogTreeSimDetails      = true; // Log all the gory details of simulated tree growth.
bool doLogGrowthModel         = true; // Log details of the growth model.
bool doLogTreeBoundingBox     = true; // Log updating the tree's bounding box
bool doLogTreeMatchRay        = true; // Log matching a ray to a tree
bool doLogTreeVisualization   = true; // Log trees being rendered.
bool doLogTreeVisDetails      = true; // Log every twig being rendered.
bool doLogBarkDisplay         = true; // Log bark color and texture details.
bool doLogTreeOpacity         = true; // Log tree bounding box opacity.
bool doLogTreeGraph           = true; // Log tree thread analysis graph.
bool doLogOTDLFileSearch      = true; // Log process of looking for OTDL files.
bool doLogTreeErrors          = true; // Log clear errors in tree related operations.

// Logging options for buildings (gables, sheds, etc)
bool doLogBuildRectDetails    = true; // Log creation and buffering of BuildingRects
bool doLogBuildingBuffer      = true; // Buffering of Gables, sheds, etc

// Logging options for groups of objects
bool doLogGroupAdditions      = true; // Log when groups are created and objects are added.
bool doLogControlGroupInit    = true; // Log the process of initiating a new control group.
bool doLogGroupMatchRay       = true; // Log ray matching in groups.
bool doLogFindGroups          = true; // Log groups found during self validation.
bool doLogPathTubeBuffer      = true; // Log the process of rendering a pathTube

// Logging options for the HTTP debug interface
bool doLogRequestErrors       = true; // Log validity problems in the HTTP request
bool doLogResponseErrors      = true; // Log problems encountered building the response
bool doLogHTTPBufferOps       = true; // Log operations on the main HTTP buffers
bool doLogHTTPDetails         = true; // Log normal details of HTTP operations
bool doLogHTTPLoadBalance     = true; // Log which connections get processed where
bool doLogRequestParsing      = true; // Log exactly what happens when parsing a request
bool doLogPseudoActions       = true; // Log as the main thread processes pseudo-actions

// Logging options to do with quadtree operations
bool doLogQuadtreeCreation    = true; // Log the initial setup of the quadtree.
bool doLogQuadtreeInsertions  = true; // Log the a new object being put in the quadtree.
bool doLogQuadtreeBoundBox    = true; // Log changes to the quadtree bounding boxes.
bool doLogQuadtreeMatchRay    = true; // Log the process of matching a ray in the quadtree.
bool doLogDisplayListBuffer   = true; // Log objects being buffered for display.
bool doLogQuadtreeObjSizes    = true; // Log the process of estimating/changing object sizes

// Logging options to do with other infrastructure
bool doLogObjectCreation        = true; // Log the id of each newly created oject.
bool doLogTriangleBufferOps     = true; // Log the workings of the triangle buffers.
bool doLogTriangleBufEstimates  = true; // Log estimates of sizes needed.
bool doLogTriangleBufRebuilds   = true; // Log when the triangle buffer is rebuilt.
bool doLogValidTriangleBufs     = true; // Validate condition of triangle buffers on gpu send
bool doLogTriangleBufferErrs    = true; // Log actual errors in the triangle buffers.

// Logging options to do with the task queues and task queue farms
bool doLogTaskQueueFarmOps      = true; // Log what the task queue farms are doing

// Logging options to do with the Bezier Patch code
bool doLogBezierFit           = true; // Log the fitting of a Bezier patch to height data
bool doLogBezierMatchRay      = true; // Log the process of matching a ray to the patch.
bool doLogObjectAltitudes     = true; // Log finding the altitudes of objects above the land


// =======================================================================================
// Control variables for whether a log statement causes a flush or not

// Logging options to do with overall control flow and speed
bool flushLogFrameStarts     = false; // Log each frame as it begins
bool flushLogCloseDown       = false; // Log when we exit
bool flushLogStructureSizes  = false; // Log the size of structures on this platform at startup.
bool flushLogEnvVars         = false; // Log the value of environment variables we care about.
bool flushLogOpenGLConstants = false; // Log various openGL parameters
bool flushLogTreeValidation  = false; // Validate data structures after every frame

// Logging options to do with resource management and collection
bool flushLogResourceErrors        = true; // Clear errors in the resource manager
bool flushLogResourceActions       = false; // Stuff the resource manager actually does
bool flushLogResourceDetails       = false; // Micro-detail of the resource manager operating
bool flushLogHttpClientErrors      = true; // Log things going wrong in the HTTP Client
bool flushLogPermaservClientErrors = true; // Log errors in the Permaserv Client layer
bool flushLogPermaservClientOps    = true; // Log normal ops in the Permaserv Client layer
bool flushLogPermaservOps          = true; // Log normal operation of the Permaserv server
bool flushLogPermaservOpDetails    = true; // Log detailed operation of Permaserv for debugging

// Logging options to do with parsing and validating OLDF and OTDL files
bool flushLogOLDFValidity        = false; // Log validity problems in the file
bool flushLogOLDFDetails         = false; // Log details of valid OLDF objects
bool flushLogOTDLValidity        = false; // Log validity problems in an OTDL object
bool flushLogOTDLDetails         = false; // Log details of valid OTDL objects

// Logging options to do with interface controls
bool flushLogMouseLocation      = false; // where the mouse is on the screen each frame
bool flushLogMouseClick         = false; // Location of single mouse clicks
bool flushLogDoubleClick        = false; // Location and intra-click timing of double clicks
bool flushLogClickDetails       = false; // Extra detailed recording of parts of click detection
bool flushLogMouseRayPoint      = false; // Where the mouse is pointing in 3d space
bool flushLogLandHeights        = false; // Log when a new land height is entered.
bool flushLogObjectInsertions   = false; // Log when a new object is inserted in scene.
bool flushLogDumpObjectBuffer   = false; // Dump buffer when a new object is inserted.
bool flushLogSimulationControls = false; // Log control operations on the simulation.
bool flushLogWindowOperations   = false; // Log changes to the window (eg resizing).
bool flushLogCameraFrames       = false; // Log camera variables every frame
bool flushLogGLFWErrors         = true; // Log when the GLFW library needs to report errors

// Logging options to do with materials and textures
bool flushLogTextureAtlas        = false; // Log texture atlas creation
bool flushLogAtlasAnomalies      = false; // Log weird conditions in texture atlas creation
bool flushLogAtlasPlacement      = false; // Log details of which image is where in atlas
bool flushLogMaterialSelections  = false; // Log when a material is selected.
bool flushLogPathMap             = false; // Log storage and use of the path map

// Logging options to do with sky sampling and lighting
bool flushLogSkySampleInit       = false; // Log the setup of the sky sampling model
bool flushLogGdalError           = true; // Log problems in accessing files via libgdal

// Logging options to do with soil stuff
bool flushLogSoilDbErr           = true; // Log problems in the soil database operation
bool flushLogSoilDbOps           = true; // Log normal operations on the soil database
bool flushLogSoilDbDetails       = false; // Log extra details in the soil database operation
bool flushLogHSWDExhaustive      = false; // Log every field read from the HSWD database
bool flushLogBilFileDetails      = false; // Log details of reading from a BILFILE

// Logging options to do with climate data processing
bool flushLogClimateDbErr        = true;  // Log problems in the climate database operation
bool flushLogClimateDbOps        = true;  // Log normal operations on the climate database
bool flushLogClimateOutliers     = false; // Log climate observation outlier exclusion
bool flushLogClimateCompDetails  = false; // Log details of climate station comparisons
bool flushLogGHCNExhaustive      = false; // Log minute details of reading the GHCN info

// Logging options for Trees/Plants
bool flushLogTreeSelections      = false; // Log when a tree is selected.
bool flushLogTreeReads           = false; // Log when a tree is read in from a file or url
bool flushLogTreeSimOverview     = false; // Log high level actions in simulating tree growth.
bool flushLogTreeSimDetails      = false; // Log all the gory details of simulated tree growth.
bool flushLogGrowthModel         = false; // Log details of the growth model.
bool flushLogTreeBoundingBox     = false; // Log updating the tree's bounding box
bool flushLogTreeMatchRay        = false; // Log matching a ray to a tree
bool flushLogTreeVisualization   = false; // Log trees being rendered.
bool flushLogTreeVisDetails      = false; // Log every twig being rendered.
bool flushLogBarkDisplay         = false; // Log bark color and texture details.
bool flushLogTreeOpacity         = false; // Log tree bounding box opacity.
bool flushLogTreeGraph           = false; // Log tree thread analysis graph.
bool flushLogOTDLFileSearch      = false; // Log process of looking for OTDL files.
bool flushLogTreeErrors          = true; // Log clear errors in tree related operations.

// Logging options for buildings (gables, sheds, etc)
bool flushLogBuildRectDetails    = false; // Log creation and buffering of BuildingRects
bool flushLogBuildingBuffer      = false; // Buffering of Gables, sheds, etc

// Logging options for groups of objects
bool flushLogGroupAdditions      = false; // Log when groups are created and objects are added.
bool flushLogControlGroupInit    = false; // Log the process of initiating a new control group.
bool flushLogGroupMatchRay       = false; // Log ray matching in groups.
bool flushLogFindGroups          = false; // Log groups found during self validation.
bool flushLogPathTubeBuffer      = false; // Log the process of rendering a pathTube

// Logging options for the HTTP debug interface
bool flushLogRequestErrors       = true; // Log validity problems in the HTTP request
bool flushLogResponseErrors      = true; // Log problems encountered building the response
bool flushLogHTTPBufferOps       = false; // Log operations on the main HTTP buffers
bool flushLogHTTPDetails         = false; // Log normal details of HTTP operations
bool flushLogHTTPLoadBalance     = false; // Log which connections get processed where
bool flushLogRequestParsing      = false; // Log exactly what happens when parsing a request
bool flushLogPseudoActions       = false; // Log as the main thread processes pseudo-actions

// Logging options to do with quadtree operations
bool flushLogQuadtreeCreation    = false; // Log the initial setup of the quadtree.
bool flushLogQuadtreeInsertions  = false; // Log the a new object being put in the quadtree.
bool flushLogQuadtreeBoundBox    = false; // Log changes to the quadtree bounding boxes.
bool flushLogQuadtreeMatchRay    = false; // Log the process of matching a ray in the quadtree.
bool flushLogDisplayListBuffer   = false; // Log objects being buffered for display.
bool flushLogQuadtreeObjSizes    = false; // Log the process of estimating/changing object sizes

// Logging options to do with other infrastructure
bool flushLogObjectCreation       = false; // Log the id of each newly created oject.
bool flushLogTriangleBufferOps    = false; // Log the workings of the triangle buffers.
bool flushLogTriangleBufEstimates = false; // Log estimates of sizes needed.
bool flushLogTriangleBufRebuilds  = false; // Log when the triangle buffer is rebuilt.
bool flushLogValidTriangleBufs    = false; // Validate condition of triangle buffers on gpu send
bool flushLogTriangleBufferErrs   = true; // Log actual errors in the triangle buffers.

// Logging options to do with the task queues and task queue farms
bool flushLogTaskQueueFarmOps      = false; // Log what the task queue farms are doing

// Logging options to do with the Bezier Patch code
bool flushLogBezierFit           = false; // Log the fitting of a Bezier patch to height data
bool flushLogBezierMatchRay      = false; // Log the process of matching a ray to the patch.
bool flushLogObjectAltitudes     = false; // Log finding the altitudes of objects above the land


// =======================================================================================
// Function to start up logging

void LogInit(char* fileName)
{
  unless(fileName)
    fileName = (char*)"permaplan.log";
  
  LogFile = fopen(fileName, "w");
  if(!LogFile)
    err(-1, "Couldn't create log file %s\n", fileName);
  loggingStart.now();
  
  LogStructureSizes("Size of Vertex: %lu bytes.\n", sizeof(Vertex));
  LogStructureSizes("Size of BuildingRect: %lu bytes.\n", sizeof(BuildingRect));
  LogStructureSizes("Size of Shed: %lu bytes.\n", sizeof(Shed));
  LogStructureSizes("Size of Orientable: %lu bytes.\n", sizeof(Orientable));
  LogStructureSizes("Size of unsigned: %lu bytes.\n", sizeof(unsigned));
  LogStructureSizes("Size of unsigned long: %lu bytes.\n", sizeof(unsigned long));
  LogStructureSizes("Size of unsigned long long: %lu bytes.\n", sizeof(unsigned long long));
  
  if(getenv("DYLD_INSERT_LIBRARIES"))
   {
    LogEnvVars("DYLD_INSERT_LIBRARIES: %s.\n", getenv("DYLD_INSERT_LIBRARIES"));
   }
  else
   {
    LogEnvVars("DYLD_INSERT_LIBRARIES: NULL.\n");
   }
}


// =======================================================================================
// Flush everythign in the log stream.

void LogFlush(void)
{
  fflush(LogFile);
}


// =======================================================================================
// Helper function for LogControlHTML to check a particular variable.

bool oneLogControl(HttpDebug* serv, char* path, bool& variable)
{
  char* next;
  unless((next = index(path, ':')))
   {
    LogRequestErrors("oneLogControl couldn't find : in %s\n", path);
    return false;
   }
  next++;
  if(*next == '1')
   {
    httPrintf("OK\n");
    variable = true;
   }
  else if(*next == '0')
   {
    httPrintf("OK\n");
    variable = false;
   }
  else
   {
    LogRequestErrors("oneLogControl couldn't find valid command in %s\n", next);
    return false;
   }
  return true;
}


// =======================================================================================
// HTTP interface to turn particular types of logging on and off.

bool LogControlHTML(HttpDebug* serv, char* path)
{
  
// Logging options to do with overall control flow and speed
  
#ifdef LOG_FRAME_STARTS // Log each frame as it begins
  if(strncmp(path, "doLogFrameStarts", sizeof("doLogFrameStarts")-1)==0)
    return oneLogControl(serv, path, doLogFrameStarts);
#endif
  
#ifdef LOG_CLOSE_DOWN // Log when we exit
  if(strncmp(path, "doLogCloseDown", sizeof("doLogCloseDown")-1)==0)
    return oneLogControl(serv, path, doLogCloseDown);
#endif
  
#ifdef LOG_STRUCTURE_SIZES // Log the size of structures on this platform at startup.
  if(strncmp(path, "doLogStructureSizes", sizeof("doLogStructureSizes")-1)==0)
    return oneLogControl(serv, path, doLogStructureSizes);
#endif
  
#ifdef LOG_ENV_VARS // Log the value of environment variables we care about.
  if(strncmp(path, "doLogEnvVars", sizeof("doLogEnvVars")-1)==0)
    return oneLogControl(serv, path, doLogEnvVars);
#endif
  
#ifdef LOG_OPENGL_CONSTANTS // Log various openGL parameters
  if(strncmp(path, "doLogOpenGLConstants", sizeof("doLogOpenGLConstants")-1)==0)
    return oneLogControl(serv, path, doLogOpenGLConstants);
#endif


// Logging options to do with resource management and collection

#ifdef LOG_RESOURCE_ERRORS // Clear errors in the resource manager
  if(strncmp(path, "doLogResourceErrors", sizeof("doLogResourceErrors")-1)==0)
    return oneLogControl(serv, path, doLogResourceErrors);
#endif

#ifdef LOG_RESOURCE_ACTIONS // Stuff the resource manager actually does
  if(strncmp(path, "doLogResourceActions", sizeof("doLogResourceActions")-1)==0)
    return oneLogControl(serv, path, doLogResourceActions);
#endif

#ifdef LOG_RESOURCE_DETAILS // Micro-detail of the resource manager operating
  if(strncmp(path, "doLogResourceDetails", sizeof("doLogResourceDetails")-1)==0)
    return oneLogControl(serv, path, doLogResourceDetails);
#endif

#ifdef LOG_HTTP_CLIENT_ERRORS // Log things going wrong in the HTTP Client
  if(strncmp(path, "doLogHttpClientErrors", sizeof("doLogHttpClientErrors")-1)==0)
    return oneLogControl(serv, path, doLogHttpClientErrors);
#endif

#ifdef LOG_PERMASERV_CLIENT_ERRORS // Log things going wrong in the Permaserv Client layer
  if(strncmp(path, "doLogPermaservClientErrors", sizeof("doLogPermaservClientErrors")-1)==0)
    return oneLogControl(serv, path, doLogPermaservClientErrors);
#endif

#ifdef LOG_PERMASERV_CLIENT_OPS // Log normal operations in the Permaserv Client layer
  if(strncmp(path, "doLogPermaservClientOps", sizeof("doLogPermaservClientOps")-1)==0)
    return oneLogControl(serv, path, doLogPermaservClientOps);
#endif

#ifdef LOG_PERMASERV_OPS // Log normal operation of the Permaserv server
  if(strncmp(path, "doLogPermaservOps", sizeof("doLogPermaservOps")-1)==0)
    return oneLogControl(serv, path, doLogPermaservOps);
#endif

#ifdef LOG_PERMASERV_OP_DETAILS // Log detailed operation of Permaserv for debugging
  if(strncmp(path, "doLogPermaservOpDetails", sizeof("doLogPermaservOpDetails")-1)==0)
    return oneLogControl(serv, path, doLogPermaservOpDetails);
#endif

  
// Logging options to do with parsing and validating OLDF and OTDL files

#ifdef LOG_TREE_VALIDATION // Validate data structures after every frame
  if(strncmp(path, "doLogTreeValidation", sizeof("doLogTreeValidation")-1)==0)
    return oneLogControl(serv, path, doLogTreeValidation);
#endif

#ifdef LOG_OLDF_VALIDITY // Log validity problems in the file
  if(strncmp(path, "doLogOLDFValidity", sizeof("doLogOLDFValidity")-1)==0)
    return oneLogControl(serv, path, doLogOLDFValidity);
#endif
  
#ifdef LOG_OLDF_DETAILS // Log details of valid OLDF objects
  if(strncmp(path, "doLogOLDFDetails", sizeof("doLogOLDFDetails")-1)==0)
    return oneLogControl(serv, path, doLogOLDFDetails);
#endif
  
#ifdef LOG_OTDL_VALIDITY // Log validity problems in an OTDL object
  if(strncmp(path, "doLogOTDLValidity", sizeof("doLogOTDLValidity")-1)==0)
    return oneLogControl(serv, path, doLogOTDLValidity);
#endif
  
#ifdef LOG_OTDL_DETAILS // Log details of valid OTDL objects
  if(strncmp(path, "doLogOTDLDetails", sizeof("doLogOTDLDetails")-1)==0)
    return oneLogControl(serv, path, doLogOTDLDetails);
#endif

  
// Logging options to do with interface controls

#ifdef LOG_MOUSE_LOCATION // where the mouse is on the screen each frame
  if(strncmp(path, "doLogMouseLocation", sizeof("doLogMouseLocation")-1)==0)
    return oneLogControl(serv, path, doLogMouseLocation);
#endif
  
#ifdef LOG_MOUSE_CLICK // Location and intra-click timing of mouse double clicks
  if(strncmp(path, "doLogMouseClick", sizeof("doLogMouseClick")-1)==0)
    return oneLogControl(serv, path, doLogMouseClick);
#endif
   
#ifdef LOG_DOUBLE_CLICK // Location and intra-click timing of mouse double clicks
  if(strncmp(path, "doLogDoubleClick", sizeof("doLogDoubleClick")-1)==0)
    return oneLogControl(serv, path, doLogDoubleClick);
#endif
  
#ifdef LOG_CLICK_DETAILS // Extra detailed recording of parts of click detection
  if(strncmp(path, "doLogClickDetails", sizeof("doLogClickDetails")-1)==0)
    return oneLogControl(serv, path, doLogClickDetails);
#endif

#ifdef LOG_MOUSE_RAY_POINT // Where the mouse is pointing in 3d space
  if(strncmp(path, "doLogMouseRayPoint", sizeof("doLogMouseRayPoint")-1)==0)
    return oneLogControl(serv, path, doLogMouseRayPoint);
#endif

#ifdef LOG_LAND_HEIGHTS // Log when a new land height is entered.
  if(strncmp(path, "doLogLandHeights", sizeof("doLogLandHeights")-1)==0)
    return oneLogControl(serv, path, doLogLandHeights);
#endif
  
#ifdef LOG_OBJECT_INSERTIONS // Log when a new object is inserted.
  if(strncmp(path, "doLogObjectInsertions", sizeof("doLogObjectInsertions")-1)==0)
    return oneLogControl(serv, path, doLogObjectInsertions);
#endif
  
#ifdef LOG_DUMP_OBJECT_BUFFER // Dump buffer when a new object is inserted.
  if(strncmp(path, "doLogDumpObjectBuffer", sizeof("doLogDumpObjectBuffer")-1)==0)
    return oneLogControl(serv, path, doLogDumpObjectBuffer);
#endif
  
#ifdef LOG_SIMULATION_CONTROLS // Log control operations on the simulation.
  if(strncmp(path, "doLogSimulationControls", sizeof("doLogSimulationControls")-1)==0)
    return oneLogControl(serv, path, doLogSimulationControls);
#endif

#ifdef LOG_WINDOW_OPERATIONS    // Log camera variables every frame
  if(strncmp(path, "doLogWindowOperations", sizeof("doLogWindowOperations")-1)==0)
    return oneLogControl(serv, path, doLogWindowOperations);
#endif

#ifdef LOG_CAMERA_FRAMES // Log changes to the window (eg resizing).
  if(strncmp(path, "doLogCameraFrames", sizeof("doLogCameraFrames")-1)==0)
    return oneLogControl(serv, path, doLogCameraFrames);
#endif

#ifdef LOG_GLFW_ERRORS // Log when the GLFW library needs to report errors
  if(strncmp(path, "doLogGLFWErrors", sizeof("doLogGLFWErrors")-1)==0)
    return oneLogControl(serv, path, doLogGLFWErrors);
#endif


// Logging options to do with materials and textures
  
#ifdef LOG_TEXTURE_ATLAS // Log texture atlas creation
  if(strncmp(path, "doLogTextureAtlas", sizeof("doLogTextureAtlas")-1)==0)
    return oneLogControl(serv, path, doLogTextureAtlas);
#endif
  
#ifdef LOG_ATLAS_ANOMALIES // Log weird conditions in texture atlas creation
  if(strncmp(path, "doLogAtlasAnomalies", sizeof("doLogAtlasAnomalies")-1)==0)
    return oneLogControl(serv, path, doLogAtlasAnomalies);
#endif
  
#ifdef LOG_ATLAS_PLACEMENT // Log details of which image is where in atlas
  if(strncmp(path, "doLogAtlasPlacement", sizeof("doLogAtlasPlacement")-1)==0)
    return oneLogControl(serv, path, doLogAtlasPlacement);
#endif
  
#ifdef LOG_MATERIAL_SELECTIONS // Log when a material is selected.
  if(strncmp(path, "doLogMaterialSelections", sizeof("doLogMaterialSelections")-1)==0)
    return oneLogControl(serv, path, doLogMaterialSelections);
#endif
  
#ifdef LOG_PATH_MAP // Log storage and use of the path map
  if(strncmp(path, "doLogPathMap", sizeof("doLogPathMap")-1)==0)
    return oneLogControl(serv, path, doLogPathMap);
#endif

  
// Logging options to do with sky sampling and lighting

#ifdef LOG_SKY_SAMPLE_INIT // Log the setup of the sky sampling model
  if(strncmp(path, "doLogSkySampleInit", sizeof("doLogSkySampleInit")-1)==0)
    return oneLogControl(serv, path, doLogSkySampleInit);
#endif

#ifdef LOG_GDAL_ERROR // Log problems in accessing files via libgdal
  if(strncmp(path, "doLogGdalError", sizeof("doLogGdalError")-1)==0)
    return oneLogControl(serv, path, doLogGdalError);
#endif


// Logging options to do with soil stuff

#ifdef LOG_SOIL_DB_ERR // Log problems in the soil database operation
  if(strncmp(path, "doLogSoilDbErr", sizeof("doLogSoilDbErr")-1)==0)
    return oneLogControl(serv, path, doLogSoilDbErr);
#endif

#ifdef LOG_SOIL_DB_OPS // Log normal operations on the soil database
  if(strncmp(path, "doLogSoilDbOps", sizeof("doLogSoilDbOps")-1)==0)
    return oneLogControl(serv, path, doLogSoilDbOps);
#endif

#ifdef LOG_SOIL_DB_DETAILS // Log extra details in the soil database operation
  if(strncmp(path, "doLogSoilDbDetails", sizeof("doLogSoilDbDetails")-1)==0)
    return oneLogControl(serv, path, doLogSoilDbDetails);
#endif

#ifdef LOG_HSWD_EXHAUSTIVE // Log every field read from the HSWD database
  if(strncmp(path, "doLogHSWDExhaustive", sizeof("doLogHSWDExhaustive")-1)==0)
    return oneLogControl(serv, path, doLogHSWDExhaustive);
#endif

#ifdef LOG_BILFILE_DETAILS // Log details of reading from a BILFILE
  if(strncmp(path, "doLogBilFileDetails", sizeof("doLogBilFileDetails")-1)==0)
    return oneLogControl(serv, path, doLogBilFileDetails);
#endif

  
// Logging options to do with climate data processing

#ifdef LOG_CLIMATE_DB_ERR // Log problems in the climate database operation
  if(strncmp(path, "doLogClimateDbErr", sizeof("doLogClimateDbErr")-1)==0)
    return oneLogControl(serv, path, doLogClimateDbErr);
#endif

#ifdef LOG_CLIMATE_DB_OPS // Log normal operations on the climate database
  if(strncmp(path, "doLogClimateDbOps", sizeof("doLogClimateDbOps")-1)==0)
    return oneLogControl(serv, path, doLogClimateDbOps);
#endif

#ifdef LOG_CLIMATE_OUTLIERS // Log climate observation outlier exclusion
  if(strncmp(path, "doLogClimateOutliers", sizeof("doLogClimateOutliers")-1)==0)
    return oneLogControl(serv, path, doLogClimateOutliers);
#endif

#ifdef LOG_CLIMATE_COMP_DETAILS // Log details of climate station comparisons
  if(strncmp(path, "doLogClimateCompDetails", sizeof("doLogClimateCompDetails")-1)==0)
    return oneLogControl(serv, path, doLogClimateCompDetails);
#endif

#ifdef LOG_GHCN_EXHAUSTIVE // Log minute details of reading the GHCN info
  if(strncmp(path, "doLogGHCNExhaustive", sizeof("doLogGHCNExhaustive")-1)==0)
    return oneLogControl(serv, path, doLogGHCNExhaustive);
#endif


// Logging options for Trees/Plants

#ifdef LOG_TREE_SELECTIONS // Log when a tree is selected.
  if(strncmp(path, "doLogTreeSelections", sizeof("doLogTreeSelections")-1)==0)
    return oneLogControl(serv, path, doLogTreeSelections);
#endif
  
#ifdef LOG_TREE_READS // Log when a tree is read in from a file or url
  if(strncmp(path, "doLogTreeReads", sizeof("doLogTreeReads")-1)==0)
    return oneLogControl(serv, path, doLogTreeReads);
#endif
  
#ifdef LOG_TREE_SIM_OVERVIEW // Log high level actions in simulating tree growth.
  if(strncmp(path, "doLogTreeSimOverview", sizeof("doLogTreeSimOverview")-1)==0)
    return oneLogControl(serv, path, doLogTreeSimOverview);
#endif
  
#ifdef LOG_TREE_SIM_DETAILS // Log all the gory details of simulated tree growth.
  if(strncmp(path, "doLogTreeSimDetails", sizeof("doLogTreeSimDetails")-1)==0)
    return oneLogControl(serv, path, doLogTreeSimDetails);
#endif
  
#ifdef LOG_GROWTH_MODEL // Log details of the growth model.
  if(strncmp(path, "doLogGrowthModel", sizeof("doLogGrowthModel")-1)==0)
    return oneLogControl(serv, path, doLogGrowthModel);
#endif
  
#ifdef LOG_TREE_BOUNDING_BOX // Log updating the tree's bounding box
  if(strncmp(path, "doLogTreeBoundingBox", sizeof("doLogTreeBoundingBox")-1)==0)
    return oneLogControl(serv, path, doLogTreeBoundingBox);
#endif
  
#ifdef LOG_TREE_MATCH_RAY // Log matching a ray to a tree
  if(strncmp(path, "doLogTreeMatchRay", sizeof("doLogTreeMatchRay")-1)==0)
    return oneLogControl(serv, path, doLogTreeMatchRay);
#endif
  
#ifdef LOG_TREE_VISUALIZATION // Log trees being rendered.
  if(strncmp(path, "doLogTreeVisualization", sizeof("doLogTreeVisualization")-1)==0)
    return oneLogControl(serv, path, doLogTreeVisualization);
#endif
  
#ifdef LOG_TREE_VIS_DETAILS // Log every twig being rendered.
  if(strncmp(path, "doLogTreeVisDetails", sizeof("doLogTreeVisDetails")-1)==0)
    return oneLogControl(serv, path, doLogTreeVisDetails);
#endif
  
#ifdef LOG_BARK_DISPLAY // Log bark color and texture details.
  if(strncmp(path, "doLogBarkDisplay", sizeof("doLogBarkDisplay")-1)==0)
    return oneLogControl(serv, path, doLogBarkDisplay);
#endif
  
#ifdef LOG_TREE_OPACITY // Log tree bounding box opacity.
  if(strncmp(path, "doLogTreeOpacity", sizeof("doLogTreeOpacity")-1)==0)
    return oneLogControl(serv, path, doLogTreeOpacity);
#endif
 
#ifdef LOG_TREE_GRAPH  // Log tree thread analysis graph.
  if(strncmp(path, "doLogTreeGraph", sizeof("doLogTreeGraph")-1)==0)
    return oneLogControl(serv, path, doLogTreeGraph);
#endif
  
#ifdef LOG_OTDL_FILE_SEARCH // Log process of looking for OTDL files.
  if(strncmp(path, "doLogOTDLFileSearch", sizeof("doLogOTDLFileSearch")-1)==0)
    return oneLogControl(serv, path, doLogOTDLFileSearch);
#endif
  
#ifdef LOG_TREE_ERRORS // Log clear errors in tree related operations.
  if(strncmp(path, "doLogTreeErrors", sizeof("doLogTreeErrors")-1)==0)
    return oneLogControl(serv, path, doLogTreeErrors);
#endif
 
  
// Logging options for buildings (gables, sheds, etc)

#ifdef LOG_BUILD_RECT_DETAILS // Log creation and buffering of BuildingRects
  if(strncmp(path, "doLogBuildRectDetails", sizeof("doLogBuildRectDetails")-1)==0)
    return oneLogControl(serv, path, doLogBuildRectDetails);
#endif

#ifdef LOG_BUILDING_BUFFER // Buffering of Gables, sheds, etc
  if(strncmp(path, "doLogBuildingBuffer", sizeof("doLogBuildingBuffer")-1)==0)
    return oneLogControl(serv, path, doLogBuildingBuffer);
#endif

  
// Logging options for groups of objects

#ifdef LOG_GROUP_ADDITIONS // Log when groups are created and objects are added.
  if(strncmp(path, "doLogGroupAdditions", sizeof("doLogGroupAdditions")-1)==0)
    return oneLogControl(serv, path, doLogGroupAdditions);
#endif
  
#ifdef LOG_CONTROL_GROUP_INIT // Log the process of initiating a new control group.
  if(strncmp(path, "doLogControlGroupInit", sizeof("doLogControlGroupInit")-1)==0)
    return oneLogControl(serv, path, doLogControlGroupInit);
#endif
  
#ifdef LOG_GROUP_MATCH_RAY // Log ray matching in groups.
  if(strncmp(path, "doLogGroupMatchRay", sizeof("doLogGroupMatchRay")-1)==0)
    return oneLogControl(serv, path, doLogGroupMatchRay);
#endif
  
#ifdef LOG_FIND_GROUPS // Log groups found during self validation.
  if(strncmp(path, "doLogFindGroups", sizeof("doLogFindGroups")-1)==0)
    return oneLogControl(serv, path, doLogFindGroups);
#endif
  
#ifdef LOG_PATHTUBE_BUFFER // Log the process of rendering a pathTube
  if(strncmp(path, "doLogPathTubeBuffer", sizeof("doLogPathTubeBuffer")-1)==0)
    return oneLogControl(serv, path, doLogPathTubeBuffer);
#endif

  
// Logging options for the HTTP debug interface
  
#ifdef LOG_REQUEST_ERRORS // Log validity problems in the HTTP request
  if(strncmp(path, "doLogRequestErrors", sizeof("doLogRequestErrors")-1)==0)
    return oneLogControl(serv, path, doLogRequestErrors);
#endif
  
#ifdef LOG_RESPONSE_ERRORS // Log problems encountered building the response
  if(strncmp(path, "doLogResponseErrors", sizeof("doLogResponseErrors")-1)==0)
    return oneLogControl(serv, path, doLogResponseErrors);
#endif

#ifdef LOG_HTTP_BUFFER_OPS // Log operations on the main HTTP buffers
  if(strncmp(path, "doLogHTTPBufferOps", sizeof("doLogHTTPBufferOps")-1)==0)
    return oneLogControl(serv, path, doLogHTTPBufferOps);
#endif
  
#ifdef LOG_HTTP_DETAILS // Log normal details of HTTP operations
  if(strncmp(path, "doLogHTTPDetails", sizeof("doLogHTTPDetails")-1)==0)
    return oneLogControl(serv, path, doLogHTTPDetails);
#endif

#ifdef LOG_HTTP_LOAD_BALANCE // Log which connections get processed where
  if(strncmp(path, "doLogHTTPLoadBalance", sizeof("doLogHTTPLoadBalance")-1)==0)
    return oneLogControl(serv, path, doLogHTTPLoadBalance);
#endif

#ifdef LOG_REQUEST_PARSING // Log exactly what happens when parsing a request
  if(strncmp(path, "doLogRequestParsing", sizeof("doLogRequestParsing")-1)==0)
    return oneLogControl(serv, path, doLogRequestParsing);
#endif

#ifdef LOG_PSEUDO_ACTIONS // Log as the main thread processes pseudo-actions
  if(strncmp(path, "doLogPseudoActions", sizeof("doLogPseudoActions")-1)==0)
    return oneLogControl(serv, path, doLogPseudoActions);
#endif

  
// Logging options to do with quadtree insertions
  
#ifdef LOG_QUADTREE_CREATION // Log the initial setup of the quadtree.
  if(strncmp(path, "doLogQuadtreeCreation", sizeof("doLogQuadtreeCreation")-1)==0)
    return oneLogControl(serv, path, doLogQuadtreeCreation);
#endif
  
#ifdef LOG_QUADTREE_INSERTIONS // Log the process of a new object being put in the quadtree.
  if(strncmp(path, "doLogQuadtreeInsertions", sizeof("doLogQuadtreeInsertions")-1)==0)
    return oneLogControl(serv, path, doLogQuadtreeInsertions);
#endif
  
#ifdef LOG_QUADTREE_BOUND_BOX // Log changes to the quadtree bounding boxes.
  if(strncmp(path, "doLogQuadtreeBoundBox", sizeof("doLogQuadtreeBoundBox")-1)==0)
    return oneLogControl(serv, path, doLogQuadtreeBoundBox);
#endif
  
#ifdef LOG_QUADTREE_MATCH_RAY // Log the process of matching a ray in the quadtree.
  if(strncmp(path, "doLogQuadtreeMatchRay", sizeof("doLogQuadtreeMatchRay")-1)==0)
    return oneLogControl(serv, path, doLogQuadtreeMatchRay);
#endif
  
#ifdef LOG_DISPLAYLIST_BUFFER // Log process of objects being buffered for display.
  if(strncmp(path, "doLogDisplayListBuffer", sizeof("doLogDisplayListBuffer")-1)==0)
    return oneLogControl(serv, path, doLogDisplayListBuffer);
#endif
  
#ifdef LOG_QUADTREE_OBJ_SIZES // Log the process of estimating/changing object sizes
  if(strncmp(path, "doLogQuadtreeObjSizes", sizeof("doLogQuadtreeObjSizes")-1)==0)
    return oneLogControl(serv, path, doLogQuadtreeObjSizes);
#endif

  
// Logging options to do with other infrastructure
  
#ifdef LOG_OBJECT_CREATION // Log the id of each newly created oject
  if(strncmp(path, "doLogObjectCreation", sizeof("doLogObjectCreation")-1)==0)
    return oneLogControl(serv, path, doLogObjectCreation);
#endif
  
#ifdef LOG_TRIANGLE_BUFFER_OPS // Log the workings of the triangle buffers.
  if(strncmp(path, "doLogTriangleBufferOps", sizeof("doLogTriangleBufferOps")-1)==0)
    return oneLogControl(serv, path, doLogTriangleBufferOps);
#endif
  
#ifdef LOG_TRIANGLE_BUF_ESTIMATES // Log estimates of sizes needed.
  if(strncmp(path, "doLogTriangleBufEstimates", sizeof("doLogTriangleBufEstimates")-1)==0)
    return oneLogControl(serv, path, doLogTriangleBufEstimates);
#endif
  
#ifdef LOG_TRIANGLE_BUF_REBUILDS // Log when the triangle buffer is rebuilt.
  if(strncmp(path, "doLogTriangleBufRebuilds", sizeof("doLogTriangleBufRebuilds")-1)==0)
    return oneLogControl(serv, path, doLogTriangleBufRebuilds);
#endif
  
#ifdef LOG_VALID_TRIANGLE_BUFS // Validate the condition of triangle buffers on gpu send
  if(strncmp(path, "doLogValidTriangleBufs", sizeof("doLogValidTriangleBufs")-1)==0)
    return oneLogControl(serv, path, doLogValidTriangleBufs);
#endif
  
#ifdef LOG_TRIANGLE_BUFFER_ERRS // Log actual errors in the triangle buffers.
  if(strncmp(path, "doLogTriangleBufferErrs", sizeof("doLogTriangleBufferErrs")-1)==0)
    return oneLogControl(serv, path, doLogTriangleBufferErrs);
#endif


// Logging options to do with the task queues and task queue farms
  
#ifdef LOG_TASK_QUEUE_FARM_OPS // Log what the task queue farms are doing
  if(strncmp(path, "doLogTaskQueueFarmOps", sizeof("doLogTaskQueueFarmOps")-1)==0)
    return oneLogControl(serv, path, doLogTaskQueueFarmOps);
#endif

  
// Logging options to do with the Bezier Patch code
  
#ifdef LOG_BEZIER_FIT // Log the process of fitting the Bezier patch to height data
  if(strncmp(path, "doLogBezierFit", sizeof("doLogBezierFit")-1)==0)
    return oneLogControl(serv, path, doLogBezierFit);
#endif
  
#ifdef LOG_BEZIER_MATCH_RAY // Log the process of matching ray intersection with a Bezier patch
  if(strncmp(path, "doLogBezierMatchRay", sizeof("doLogBezierMatchRay")-1)==0)
    return oneLogControl(serv, path, doLogBezierMatchRay);
#endif
  
#ifdef LOG_OBJECT_ALTITUDES // Log finding the altitudes of objects above the land
  if(strncmp(path, "doLogObjectAltitudes", sizeof("doLogObjectAltitudes")-1)==0)
    return oneLogControl(serv, path, doLogObjectAltitudes);
#endif
  
  LogRequestErrors("Couldn't find valid log action to perform from %s\n", path);
  return false;
}


// =======================================================================================

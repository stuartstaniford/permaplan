// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// File that controls and implements the logging system.  A variety of options
// are defined at the beginning that implement conditional compilation of all the
// different kinds of logging.

#ifndef LOGGING_H
#define LOGGING_H

#include "MemoryTracker.h"
#include "Timeval.h"
#include "Lockable.h"
#include <cstdio>


// =======================================================================================
// Logging options to do with overall control flow and speed

#define LOG_FRAME_STARTS        // Log each frame as it begins
#define LOG_CLOSE_DOWN          // Log when we exit.
#define LOG_STRUCTURE_SIZES     // Log the size of structures on this platform at startup.
#define LOG_ENV_VARS            // Log the value of environment variables we care about.
#define LOG_OPENGL_CONSTANTS    // Log various openGL parameters
#define LOG_TREE_VALIDATION     // Validate data structures after every frame


// =======================================================================================
// Logging options to do with resource management and collection

#define LOG_RESOURCE_ERRORS         // Clear errors in the resource manager
#define LOG_RESOURCE_ACTIONS        // Stuff the resource manager actually does
#define LOG_RESOURCE_DETAILS        // Micro-detail of the resource manager operating
#define LOG_HTTP_CLIENT_ERRORS      // Log things going wrong in the HTTP Client
#define LOG_PERMASERV_CLIENT_ERRORS // Log things going wrong in the Permaserv Client layer
#define LOG_PERMASERV_CLIENT_OPS    // Log normal operations in the Permaserv Client layer
#define LOG_PERMASERV_OPS           // Log normal operation of the Permaserv server
#define LOG_PERMASERV_OP_DETAILS    // Log detailed operation of Permaserv for debugging


// =======================================================================================
// Logging options to do with parsing and validating OLDF and OTDL files

#define LOG_OLDF_VALIDITY      // Log validity problems in the file
//#define LOG_OLDF_DETAILS       // Log details of valid OLDF objects
#define LOG_OTDL_VALIDITY      // Log validity problems in an OTDL object
//#define LOG_OTDL_DETAILS       // Log details of valid OTDL objects


// =======================================================================================
// Logging options to do with interface controls

//#define LOG_MOUSE_LOCATION      // where the mouse is on the screen each frame
//#define LOG_MOUSE_CLICK         // Location of single mouse clicks
#define LOG_DOUBLE_CLICK        // Location and intra-click timing of mouse double clicks
//#define LOG_CLICK_DETAILS        // Extra detailed recording of parts of click detection
//#define LOG_MOUSE_RAY_POINT     // Where the mouse is pointing in 3d space
//#define LOG_LAND_HEIGHTS        // Log when a new land height is entered.
#define LOG_OBJECT_INSERTIONS   // Log when a new object is inserted in scene.
#define LOG_DUMP_OBJECT_BUFFER   // Dump buffer when a new object is inserted.
#define LOG_SIMULATION_CONTROLS // Log control operations on the simulation.
#define LOG_WINDOW_OPERATIONS   // Log changes to the window (eg resizing).
#define LOG_CAMERA_FRAMES       // Log camera variables every frame
#define LOG_GLFW_ERRORS         // Log when the GLFW library needs to report errors


// =======================================================================================
// Logging options to do with materials and textures

//#define LOG_TEXTURE_ATLAS       // Log texture atlas creation
#define LOG_ATLAS_ANOMALIES     // Log weird conditions in texture atlas creation
//#define LOG_ATLAS_PLACEMENT     // Log details of which image is where in atlas
//#define LOG_MATERIAL_SELECTIONS // Log when a material is selected.
//#define LOG_PATH_MAP            // Log storage and use of the path map


// =======================================================================================
// Logging options to do with sky sampling and lighting

#define LOG_SKY_SAMPLE_INIT       // Log the setup of the sky sampling model
#define LOG_GDAL_ERROR            // Log problems in accessing files via libgdal


// =======================================================================================
// Logging options to do with soil stuff

#define LOG_SOIL_DB_ERR             // Log problems in the soil database operation
#define LOG_SOIL_DB_OPS             // Log normal operations on the soil database
#define LOG_SOIL_DB_DETAILS         // Log extra details in the soil database operation
#define LOG_HSWD_EXHAUSTIVE         // Log every field read from the HSWD database
#define LOG_BILFILE_DETAILS         // Log details of reading from a BILFILE


// =======================================================================================
// Logging options for Trees/Plants

#define LOG_TREE_SELECTIONS       // Log when a tree is selected.
//#define LOG_TREE_READS            // Log when a tree is read in from a file or url
//#define LOG_TREE_SIM_OVERVIEW     // Log high level actions in simulating tree growth.
//#define LOG_TREE_SIM_DETAILS      // Log all the gory details of simulated tree growth.
//#define LOG_GROWTH_MODEL          // Log details of the growth model.
//#define LOG_TREE_BOUNDING_BOX     // Log updating the tree's bounding box
//#define LOG_TREE_MATCH_RAY        // Log matching a ray to a tree
//#define LOG_TREE_VISUALIZATION    // Log trees being rendered.
//#define LOG_TREE_VIS_DETAILS      // Log every twig being rendered.
//#define LOG_BARK_DISPLAY          // Log bark color and texture details.
//#define LOG_TREE_OPACITY          // Log tree bounding box opacity.
//#define LOG_TREE_GRAPH            // Log tree thread analysis graph.
//#define LOG_OTDL_FILE_SEARCH      // Log process of looking for OTDL files.
#define LOG_TREE_ERRORS           // Log clear errors in tree related operations.


// =======================================================================================
// Logging options for buildings (gables, sheds, etc)

#define LOG_BUILD_RECT_DETAILS      // Log creation and buffering of BuildingRects
#define LOG_BUILDING_BUFFER         // Buffering of Gables, sheds, etc


// =======================================================================================
// Logging options for groups of objects

#define LOG_GROUP_ADDITIONS       // Log when groups are created and objects are added.
#define LOG_CONTROL_GROUP_INIT    // Log the process of initiating a new control group.
//#define LOG_GROUP_MATCH_RAY       // Log ray matching in groups.
//#define LOG_FIND_GROUPS           // Log groups found during self validation.
#define LOG_PATHTUBE_BUFFER       // Log the process of rendering a pathTube


// =======================================================================================
// Logging options for the HTTP debug interface

#define LOG_REQUEST_ERRORS      // Log validity problems in the HTTP request
#define LOG_RESPONSE_ERRORS     // Log problems encountered building the response
//#define LOG_HTTP_BUFFER_OPS        // Log operations on the main HTTP buffers
//#define LOG_HTTP_DETAILS        // Log normal details of HTTP operations
//#define LOG_HTTP_LOAD_BALANCE     // Log which connections get processed where
//#define LOG_REQUEST_PARSING        // Log exactly what happens when parsing a request
//#define LOG_PSEUDO_ACTIONS      // Log as the main thread processes pseudo-actions


// =======================================================================================
// Logging options to do with quadtree operations

//#define LOG_QUADTREE_CREATION   // Log the initial setup of the quadtree.
//#define LOG_QUADTREE_INSERTIONS // Log a new object being put in the quadtree.
//#define LOG_QUADTREE_BOUND_BOX  // Log changes to the quadtree bounding boxes.
//#define LOG_QUADTREE_MATCH_RAY  // Log the process of matching a ray in the quadtree.
//#define LOG_DISPLAYLIST_BUFFER  // Log the process of objects being buffered for display.
//#define LOG_QUADTREE_OBJ_SIZES  // Log the process of estimating/changing object sizes


// =======================================================================================
// Logging options to do with other infrastructure

//#define LOG_OBJECT_CREATION            // Log the id of each newly created oject
//#define LOG_TRIANGLE_BUFFER_OPS     // Log the workings of the triangle buffers.
//#define LOG_TRIANGLE_BUF_ESTIMATES  // Log estimates of sizes needed.
//#define LOG_TRIANGLE_BUF_REBUILDS  // Log when the triangle buffer is rebuilt.
#define LOG_VALID_TRIANGLE_BUFS // Validate the condition of triangle buffers on gpu send
#define LOG_TRIANGLE_BUFFER_ERRS    // Log actual errors in the triangle buffers.


// =======================================================================================
// Logging options to do with the task queues and task queue farms

#define LOG_TASK_QUEUE_FARM_OPS   // Log what the task queue farms are doing


// =======================================================================================
// Logging options to do with the Bezier Patch code

//#define LOG_BEZIER_FIT        // Log the process of fitting the Bezier patch to height data
//#define LOG_BEZIER_MATCH_RAY  // Log the process of matching a ray to the patch.
//#define LOG_OBJECT_ALTITUDES   // Log finding the altitudes of objects above the land


// =======================================================================================
// =======================================================================================
// =======================================================================================
// This is the end of the control directives, now we switch to the actual implementation
// of the logging mechanism.

// =======================================================================================
// General logging mechanism

#define LOGGING_BUF_SIZE 2048

#define LogStatement(...) { \
    logLock.lock();\
    logWritePt = loggingBuf;\
    loggingNow.now();\
    logWritePt += snprintf(logWritePt, LOGGING_BUF_SIZE - (logWritePt-loggingBuf),\
                    "%8.3lf ", loggingNow - loggingStart);\
    logWritePt += snprintf(logWritePt, LOGGING_BUF_SIZE - (logWritePt-loggingBuf), \
                    __VA_ARGS__);\
    fputs(loggingBuf, LogFile);\
    logLock.unlock();\
    }

class HTTPDebug;
void LogInit(char* fileName = NULL);
void LogFlush(void);
bool LogControlHTML(HttpDebug* serv, char* path);


// =======================================================================================
// Variables needed in the logging implementation

#ifdef LOGGING_IMPLEMENTATION

char      loggingBuf[LOGGING_BUF_SIZE];
char*     logWritePt;
Timeval   loggingStart;
Timeval   loggingNow;
FILE*     LogFile;
Lockable  logLock;

#else

extern char     loggingBuf[];
extern char*    logWritePt;
extern Timeval  loggingStart;
extern Timeval  loggingNow;
extern FILE*    LogFile;
extern Lockable logLock;


// =======================================================================================
// Control variables for real time control of which logging is turned on

// Logging options to do with overall control flow and speed
extern bool doLogFrameStarts;     // Log each frame as it begins
extern bool doLogCloseDown;       // Log when we exit
extern bool doLogStructureSizes;  // Log the size of structures on this platform at startup.
extern bool doLogEnvVars;         // Log the value of environment variables we care about.
extern bool doLogOpenGLConstants; // Log various openGL parameters
extern bool doLogTreeValidation;  // Validate data structures after every frame

// Logging options to do with resource management and collection
extern bool doLogResourceErrors;        // Clear errors in the resource manager
extern bool doLogResourceActions;       // Stuff the resource manager actually does
extern bool doLogResourceDetails;       // Micro-detail of the resource manager operating
extern bool doLogHttpClientErrors;      // Log things going wrong in the HTTP Client
extern bool doLogPermaservClientErrors; // Log errors in the Permaserv Client layer
extern bool doLogPermaservClientOps;    // Log normal operations in the Permaserv Client layer
extern bool doLogPermaservOps;          // Log normal operation of the Permaserv server
extern bool doLogPermaservOpDetails;    // Log detailed operation of Permaserv for debugging

// Logging options to do with parsing and validating OLDF and OTDL files
extern bool doLogOLDFValidity;    // Log validity problems in the file
extern bool doLogOLDFDetails;     // Log details of valid OLDF objects
extern bool doLogOTDLValidity;    // Log validity problems in an OTDL object
extern bool doLogOTDLDetails;     // Log details of valid OTDL objects

// Logging options to do with interface controls
extern bool doLogMouseLocation;       // where the mouse is on the screen each frame
extern bool doLogMouseClick;          // Location of single mouse clicks
extern bool doLogDoubleClick;         // Location and intra-click timing of double clicks
extern bool doLogClickDetails;        // Extra detailed recording of parts of click detection
extern bool doLogMouseRayPoint;       // Where the mouse is pointing in 3d space
extern bool doLogLandHeights;         // Log when a new land height is entered.
extern bool doLogObjectInsertions;    // Log when a new object is inserted in scene.
extern bool doLogDumpObjectBuffer;    // Dump buffer when a new object is inserted.
extern bool doLogSimulationControls;  // Log control operations on the simulation.
extern bool doLogWindowOperations;    // Log changes to the window (eg resizing).
extern bool doLogCameraFrames;        // Log camera variables every frame
extern bool doLogGLFWErrors;          // Log when the GLFW library needs to report errors

// Logging options to do with materials and textures
extern bool doLogTextureAtlas;        // Log texture atlas creation
extern bool doLogAtlasAnomalies;      // Log weird conditions in texture atlas creation
extern bool doLogAtlasPlacement;      // Log details of which image is where in atlas
extern bool doLogMaterialSelections;  // Log when a material is selected.
extern bool doLogPathMap;             // Log storage and use of the path map

// Logging options to do with sky sampling and lighting
extern bool doLogSkySampleInit;       // Log the setup of the sky sampling model
extern bool doLogGdalError;           // Log problems in accessing files via libgdal

// Logging options to do with soil stuff
extern bool doLogSoilDbErr;           // Log problems in the soil database operation
extern bool doLogSoilDbOps;           // Log normal operations on the soil database
extern bool doLogSoilDbDetails;       // Log extra details in the soil database operation
extern bool doLogHSWDExhaustive;      // Log every field read from the HSWD database
extern bool doLogBilFileDetails;      // Log details of reading from a BILFILE

// Logging options for Trees/Plants
extern bool doLogTreeSelections;      // Log when a tree is selected.
extern bool doLogTreeReads;           // Log when a tree is read in from a file or url
extern bool doLogTreeSimOverview;     // Log high level actions in simulating tree growth.
extern bool doLogTreeSimDetails;      // Log all the gory details of simulated tree growth.
extern bool doLogGrowthModel;         // Log details of the growth model.
extern bool doLogTreeBoundingBox;     // Log updating the tree's bounding box
extern bool doLogTreeMatchRay;        // Log matching a ray to a tree
extern bool doLogTreeVisualization;   // Log trees being rendered.
extern bool doLogTreeVisDetails;      // Log every twig being rendered.
extern bool doLogBarkDisplay;         // Log bark color and texture details.
extern bool doLogTreeOpacity;         // Log tree bounding box opacity.
extern bool doLogTreeGraph;           // Log tree thread analysis graph.
extern bool doLogOTDLFileSearch;      // Log process of looking for OTDL files.
extern bool doLogTreeErrors;          // Log clear errors in tree related operations.

// Logging options for buildings (gables, sheds, etc)
extern bool doLogBuildRectDetails;    // Log creation and buffering of BuildingRects
extern bool doLogBuildingBuffer;      // Buffering of Gables, sheds, etc

// Logging options for groups of objects
extern bool doLogGroupAdditions;      // Log when groups are created and objects are added.
extern bool doLogControlGroupInit;    // Log the process of initiating a new control group.
extern bool doLogGroupMatchRay;       // Log ray matching in groups.
extern bool doLogFindGroups;          // Log groups found during self validation.
extern bool doLogPathTubeBuffer;      // Log the process of rendering a pathTube

// Logging options for the HTTP debug interface
extern bool doLogRequestErrors;       // Log validity problems in the HTTP request
extern bool doLogResponseErrors;      // Log problems encountered building the response
extern bool doLogHTTPBufferOps;       // Log operations on the main HTTP buffers
extern bool doLogHTTPDetails;         // Log normal details of HTTP operations
extern bool doLogHTTPLoadBalance;     // Log which connections get processed where
extern bool doLogRequestParsing;      // Log exactly what happens when parsing a request
extern bool doLogPseudoActions;       // Log as the main thread processes pseudo-actions

// Logging options to do with quadtree operations
extern bool doLogQuadtreeCreation;    // Log the initial setup of the quadtree.
extern bool doLogQuadtreeInsertions;  // Log the a new object being put in the quadtree.
extern bool doLogQuadtreeBoundBox;    // Log changes to the quadtree bounding boxes.
extern bool doLogQuadtreeMatchRay;    // Log the process of matching a ray in the quadtree.
extern bool doLogDisplayListBuffer;   // Log process of objects being buffered for display.
extern bool doLogQuadtreeObjSizes;    // Log the process of estimating/changing object sizes

// Logging options to do with other infrastructure
extern bool doLogObjectCreation;        // Log the id of each newly created oject.
extern bool doLogTriangleBufferOps;     // Log the workings of the triangle buffers.
extern bool doLogTriangleBufEstimates;  // Log estimates of sizes needed.
extern bool doLogTriangleBufRebuilds;   // Log when the triangle buffer is rebuilt.
extern bool doLogValidTriangleBufs;     // Validate condition of triangle buffers on gpu send
extern bool doLogTriangleBufferErrs;    // Log actual errors the triangle buffers.

// Logging options to do with the task queues and task queue farms
extern bool doLogTaskQueueFarmOps;    // Log what the task queue farms are doing

// Logging options to do with the Bezier Patch code
extern bool doLogBezierFit;           // Log the fitting of a Bezier patch to height data
extern bool doLogBezierMatchRay;      // Log the process of matching a ray to the patch.
extern bool doLogObjectAltitudes;     // Log finding the altitudes of objects above the land

// =======================================================================================
// Control variables for whether a log statement causes a flush or not

// Logging options to do with overall control flow and speed
extern bool flushLogFrameStarts;    // Log each frame as it begins
extern bool flushLogCloseDown;      // Log when we exit
extern bool flushLogStructureSizes; // Log the size of structures on this platform at startup.
extern bool flushLogEnvVars;        // Log the value of environment variables we care about.
extern bool flushLogOpenGLConstants;// Log various openGL parameters
extern bool flushLogTreeValidation; // Validate data structures after every frame

// Logging options to do with resource management and collection
extern bool flushLogResourceErrors;     // Clear errors in the resource manager
extern bool flushLogResourceActions;    // Stuff the resource manager actually does
extern bool flushLogResourceDetails;    // Micro-detail of the resource manager operating
extern bool flushLogHttpClientErrors;   // Log things going wrong in the HTTP Client
extern bool flushLogPermaservClientErrors; // Log errors in the Permaserv Client layer
extern bool flushLogPermaservClientOps; // Log normal operations in the Permaserv Client layer
extern bool flushLogPermaservOps;       // Log normal operation of the Permaserv server
extern bool flushLogPermaservOpDetails; // Log detailed operation of Permaserv for debugging

// Logging options to do with parsing and validating OLDF and OTDL files
extern bool flushLogOLDFValidity;    // Log validity problems in the file
extern bool flushLogOLDFDetails;     // Log details of valid OLDF objects
extern bool flushLogOTDLValidity;    // Log validity problems in an OTDL object
extern bool flushLogOTDLDetails;     // Log details of valid OTDL objects

// Logging options to do with interface controls
extern bool flushLogMouseLocation;     // where the mouse is on the screen each frame
extern bool flushLogMouseClick;        // Location of single mouse clicks
extern bool flushLogDoubleClick;       // Location and intra-click timing of double clicks
extern bool flushLogClickDetails;      // Extra detailed recording of parts of click detection
extern bool flushLogMouseRayPoint;     // Where the mouse is pointing in 3d space
extern bool flushLogLandHeights;       // Log when a new land height is entered.
extern bool flushLogObjectInsertions;  // Log when a new object is inserted in scene.
extern bool flushLogDumpObjectBuffer;  // Dump buffer when a new object is inserted.
extern bool flushLogSimulationControls;// Log control operations on the simulation.
extern bool flushLogWindowOperations;  // Log changes to the window (eg resizing).
extern bool flushLogCameraFrames;      // Log camera variables every frame
extern bool flushLogGLFWErrors;        // Log when the GLFW library needs to report errors

// Logging options to do with materials and textures
extern bool flushLogTextureAtlas;        // Log texture atlas creation
extern bool flushLogAtlasAnomalies;      // Log weird conditions in texture atlas creation
extern bool flushLogAtlasPlacement;      // Log details of which image is where in atlas
extern bool flushLogMaterialSelections;  // Log when a material is selected.
extern bool flushLogPathMap;             // Log storage and use of the path map

// Logging options to do with sky sampling and lighting
extern bool flushLogSkySampleInit;       // Log the setup of the sky sampling model
extern bool flushLogGdalError;           // Log problems in accessing files via libgdal

// Logging options to do with soil stuff
extern bool flushLogSoilDbErr;           // Log problems in the soil database operation
extern bool flushLogSoilDbOps;           // Log normal operations on the soil database
extern bool flushLogSoilDbDetails;       // Log extra details in the soil database operation
extern bool flushLogHSWDExhaustive;      // Log every field read from the HSWD database
extern bool flushLogBilFileDetails;      // Log details of reading from a BILFILE

// Logging options for Trees/Plants
extern bool flushLogTreeSelections;      // Log when a tree is selected.
extern bool flushLogTreeReads;           // Log when a tree is read in from a file or url
extern bool flushLogTreeSimOverview;     // Log high level actions in simulating tree growth.
extern bool flushLogTreeSimDetails;      // Log all the gory details of simulated tree growth.
extern bool flushLogGrowthModel;         // Log details of the growth model.
extern bool flushLogTreeBoundingBox;     // Log updating the tree's bounding box
extern bool flushLogTreeMatchRay;        // Log matching a ray to a tree
extern bool flushLogTreeVisualization;   // Log trees being rendered.
extern bool flushLogTreeVisDetails;      // Log every twig being rendered.
extern bool flushLogBarkDisplay;         // Log bark color and texture details.
extern bool flushLogTreeOpacity;         // Log tree bounding box opacity.
extern bool flushLogTreeGraph;           // Log tree thread analysis graph.
extern bool flushLogOTDLFileSearch;      // Log process of looking for OTDL files.
extern bool flushLogTreeErrors;          // Log clear errors in tree related operations.

// Logging options for buildings (gables, sheds, etc)
extern bool flushLogBuildRectDetails;    // Log creation and buffering of BuildingRects
extern bool flushLogBuildingBuffer;      // Buffering of Gables, sheds, etc

// Logging options for groups of objects
extern bool flushLogGroupAdditions;      // Log when groups are created and objects are added.
extern bool flushLogControlGroupInit;    // Log the process of initiating a new control group.
extern bool flushLogGroupMatchRay;       // Log ray matching in groups.
extern bool flushLogFindGroups;          // Log groups found during self validation.
extern bool flushLogPathTubeBuffer;      // Log the process of rendering a pathTube

// Logging options for the HTTP debug interface
extern bool flushLogRequestErrors;       // Log validity problems in the HTTP request
extern bool flushLogResponseErrors;      // Log problems encountered building the response
extern bool flushLogHTTPBufferOps;       // Log operations on the main HTTP buffers
extern bool flushLogHTTPDetails;         // Log normal details of HTTP operations
extern bool flushLogHTTPLoadBalance;     // Log which connections get processed where
extern bool flushLogRequestParsing;      // Log exactly what happens when parsing a request
extern bool flushLogPseudoActions;       // Log as the main thread processes pseudo-actions

// Logging options to do with quadtree operations
extern bool flushLogQuadtreeCreation;   // Log the initial setup of the quadtree.
extern bool flushLogQuadtreeInsertions; // Log the a new object being put in the quadtree.
extern bool flushLogQuadtreeBoundBox;   // Log changes to the quadtree bounding boxes.
extern bool flushLogQuadtreeMatchRay;   // Log the process of matching a ray in the quadtree.
extern bool flushLogDisplayListBuffer;  // Log process of objects being buffered for display.
extern bool flushLogQuadtreeObjSizes;   // Log the process of estimating/changing object sizes

// Logging options to do with other infrastructure
extern bool flushLogObjectCreation;      // Log the id of each newly created oject.
extern bool flushLogTriangleBufferOps;   // Log the workings of the triangle buffers.
extern bool flushLogTriangleBufEstimates;// Log estimates of sizes needed.
extern bool flushLogTriangleBufRebuilds; // Log when the triangle buffer is rebuilt.
extern bool flushLogValidTriangleBufs;   // Validate condition of triangle buffers on gpu send
extern bool flushLogTriangleBufferErrs;  // Log actual errors the triangle buffers.

// Logging options to do with the task queues and task queue farms
extern bool flushLogTaskQueueFarmOps;    // Log what the task queue farms are doing

// Logging options to do with the Bezier Patch code
extern bool flushLogBezierFit;         // Log the fitting of a Bezier patch to height data
extern bool flushLogBezierMatchRay;    // Log the process of matching a ray to the patch.
extern bool flushLogObjectAltitudes;   // Log finding the altitudes of objects above the land

#endif  //else on ifdef LOGGING_IMPLEMENTATION - need this here.


// =======================================================================================
// Logging options to do with overall control flow and speed

// Log each frame as it begins
#ifdef LOG_FRAME_STARTS
#define LogFrameStarts(...)     {\
                                  if(doLogFrameStarts) \
                                    LogStatement("LogFrameStarts: " __VA_ARGS__)\
                                  if(flushLogFrameStarts)\
                                    LogFlush();\
                                }
#else
#define LogFrameStarts(...)
#endif

// Log when we exit
#ifdef LOG_CLOSE_DOWN
#define LogCloseDown(...)     {\
                                if(doLogCloseDown) \
                                  LogStatement("LogCloseDown: " __VA_ARGS__)\
                                if(flushLogCloseDown)\
                                  LogFlush();\
                              }
#else
#define LogCloseDown(...)
#endif

// Log the size of structures on this platform at startup.
#ifdef LOG_STRUCTURE_SIZES
#define LogStructureSizes(...)  {\
                                  if(doLogStructureSizes) \
                                    LogStatement("LogStructureSizes: "  __VA_ARGS__)\
                                  if(flushLogStructureSizes)\
                                    LogFlush();\
                                }
#else
#define LogStructureSizes(...)
#endif

// Log the value of environment variables we care about.
#ifdef LOG_ENV_VARS
#define LogEnvVars(...)     {\
                              if(doLogEnvVars) \
                                LogStatement("LogEnvVars: "  __VA_ARGS__)\
                              if(flushLogEnvVars)\
                                LogFlush();\
                             }
#else
#define LogEnvVars(...)
#endif

// Log various openGL parameters
#ifdef LOG_OPENGL_CONSTANTS
#define LogOpenGLConstants(...)   {\
                                    if(doLogOpenGLConstants) \
                                      LogStatement("LogOpenGLConstants: " __VA_ARGS__)\
                                    if(flushLogOpenGLConstants)\
                                      LogFlush();\
                                  }
#else
#define LogOpenGLConstants(...)
#endif

// Validate data structures after every frame
#ifdef LOG_TREE_VALIDATION
#define LogTreeValidation(...)    {\
                                    if(doLogTreeValidation) \
                                      LogStatement("LogTreeValidation: " __VA_ARGS__)\
                                    if(flushLogTreeValidation) \
                                      LogFlush();\
                                  }
#else
#define LogTreeValidation(...)
#endif


// =======================================================================================
// Logging options to do with resource management and collection

// Clear errors in the resource manager
#ifdef LOG_RESOURCE_ERRORS
#define LogResourceErrors(...)    {\
                                    if(doLogResourceErrors) \
                                      LogStatement("LogResourceErrors: " __VA_ARGS__)\
                                    if(flushLogResourceErrors)\
                                      LogFlush();\
                                  }
#else
#define LogResourceErrors(...)
#endif

// Stuff the resource manager actually does
#ifdef LOG_RESOURCE_ACTIONS
#define LogResourceActions(...)   {\
                                    if(doLogResourceActions) \
                                      LogStatement("LogResourceActions: " __VA_ARGS__)\
                                    if(flushLogResourceActions)\
                                      LogFlush();\
                                  }
#else
#define LogResourceActions(...)
#endif

// Micro-detail of the resource manager operating
#ifdef LOG_RESOURCE_DETAILS
#define LogResourceDetails(...)   {\
                                    if(doLogResourceDetails) \
                                      LogStatement("LogResourceDetails: " __VA_ARGS__)\
                                    if(flushLogResourceDetails)\
                                      LogFlush();\
                                  }
#else
#define LogResourceDetails(...)
#endif

// Log things going wrong in the HTTP Client
#ifdef LOG_HTTP_CLIENT_ERRORS
#define LogHttpClientErrors(...)    {\
                                      if(doLogHttpClientErrors) \
                                        LogStatement("LogHttpClientErrors: " __VA_ARGS__)\
                                      if(flushLogHttpClientErrors)\
                                        LogFlush();\
                                    }
#else
#define LogHttpClientErrors(...)
#endif
      
// Log things going wrong in the Permaserv Client layer
#ifdef LOG_PERMASERV_CLIENT_ERRORS
#define LogPermaservClientErrors(...)   \
                              {\
                                if(doLogPermaservClientErrors) \
                                  LogStatement("LogPermaservClientErrors: " __VA_ARGS__)\
                                if(flushLogPermaservClientErrors)\
                                  LogFlush();\
                              }
#else
#define LogPermaservClientErrors(...)
#endif

// Log normal operations in the Permaserv Client layer
#ifdef LOG_PERMASERV_CLIENT_OPS
#define LogPermaservClientOps(...)\
                                {\
                                  if(doLogPermaservClientOps) \
                                    LogStatement("LogPermaservClientOps: " __VA_ARGS__)\
                                  if(flushLogPermaservClientOps)\
                                    LogFlush();\
                                }
#else
#define LogPermaservClientOps(...)
#endif

// Log normal operation of the Permaserv server
#ifdef LOG_PERMASERV_OPS
#define LogPermaservOps(...)    {\
                                  if(doLogPermaservOps) \
                                    LogStatement("LogPermaservOps: " __VA_ARGS__)\
                                  if(flushLogPermaservOps)\
                                    LogFlush();\
                                }
#else
#define LogPermaservOps(...)
#endif

// Log detailed operation of Permaserv for debugging
#ifdef LOG_PERMASERV_OP_DETAILS
#define LogPermaservOpDetails(...)    \
                                  {\
                                    if(doLogPermaservOpDetails) \
                                      LogStatement("LogPermaservOpDetails: " __VA_ARGS__)\
                                    if(flushLogPermaservOpDetails)\
                                      LogFlush();\
                                  }
#else
#define LogPermaservOpDetails(...)
#endif


// =======================================================================================
// Logging options to do with parsing and validating OLDF and OTDL files

// Log validity problems in the file
#ifdef LOG_OLDF_VALIDITY
#define LogOLDFValidity(...)    {\
                                  if(doLogOLDFValidity) \
                                    LogStatement("LogOLDFValidity: " __VA_ARGS__)\
                                  if(flushLogOLDFValidity)\
                                    LogFlush();\
                                }
#else
#define LogOLDFValidity(...)
#endif

// Log details of valid OLDF objects
#ifdef LOG_OLDF_DETAILS
#define LogOLDFDetails(...)   {\
                                if(doLogOLDFDetails) \
                                  LogStatement("LogOLDFDetails: " __VA_ARGS__)\
                                if(flushLogOLDFDetails)\
                                  LogFlush();\
                              }
#else
#define LogOLDFDetails(...)
#endif

// Log validity problems in an OTDL object
#ifdef LOG_OTDL_VALIDITY
#define LogOTDLValidity(...)    {\
                                  if(doLogOTDLValidity) \
                                    LogStatement("LogOTDLValidity: " __VA_ARGS__)\
                                  if(flushLogOTDLValidity)\
                                    LogFlush();\
                                }
#else
#define LogOTDLValidity(...)
#endif

// Log details of valid OTDL objects
#ifdef LOG_OTDL_DETAILS
#define LogOTDLDetails(...)   {\
                                if(doLogOTDLDetails) \
                                  LogStatement("LogOTDLDetails: " __VA_ARGS__)\
                                if(flushLogOTDLDetails)\
                                  LogFlush();\
                              }
#else
#define LogOTDLDetails(...)
#endif


// =======================================================================================
// Logging options to do with interface controls

// where the mouse is on the screen each frame
#ifdef LOG_MOUSE_LOCATION
#define LogMouseLocation(...)   {\
                                  if(doLogMouseLocation) \
                                    LogStatement("LogMouseLocation: " __VA_ARGS__)\
                                  if(flushLogMouseLocation)\
                                    LogFlush();\
                                }
#else
#define LogMouseLocation(...)
#endif

// Location and intra-click timing of mouse double clicks
#ifdef LOG_MOUSE_CLICK
#define LogMouseClick(...)    {\
                                if(doLogMouseClick) \
                                  LogStatement("LogMouseClick: " __VA_ARGS__)\
                                if(flushLogMouseClick)\
                                  LogFlush();\
                              }
#else
#define LogMouseClick(...)
#endif

// Location and intra-click timing of mouse double clicks
#ifdef LOG_DOUBLE_CLICK
#define LogDoubleClick(...)   {\
                                if(doLogDoubleClick) \
                                  LogStatement("LogDoubleClick: " __VA_ARGS__)\
                                if(flushLogDoubleClick)\
                                  LogFlush();\
                              }
#else
#define LogDoubleClick(...)
#endif

// Extra detailed recording of parts of click detection
#ifdef LOG_CLICK_DETAILS
#define LogClickDetails(...)    {\
                                  if(doLogClickDetails) \
                                    LogStatement("LogClickDetails: " __VA_ARGS__)\
                                  if(flushLogClickDetails)\
                                    LogFlush();\
                                }
#else
#define LogClickDetails(...)
#endif

// Where the mouse is pointing in 3d space
#ifdef LOG_MOUSE_RAY_POINT
#define LogMouseRayPoint(...)   {\
                                  if(doLogMouseRayPoint) \
                                    LogStatement("LogMouseRayPoint: " __VA_ARGS__)\
                                  if(flushLogMouseRayPoint)\
                                    LogFlush();\
                                }
#else
#define LogMouseRayPoint(...)
#endif

// Log when a new land height is entered.
#ifdef LOG_LAND_HEIGHTS
#define LogLandHeights(...)   {\
                                if(doLogLandHeights) \
                                  LogStatement("LogLandHeights: " __VA_ARGS__)\
                                if(flushLogLandHeights)\
                                  LogFlush();\
                              }
#else
#define LogLandHeights(...)
#endif

// Log when a new object is inserted.
#ifdef LOG_OBJECT_INSERTIONS
#define LogObjectInsertions(...) \
                                {\
                                  if(doLogObjectInsertions) \
                                    LogStatement("LogObjectInsertions: " __VA_ARGS__)\
                                  if(flushLogObjectInsertions)\
                                    LogFlush();\
                                }
#else
#define LogObjectInsertions(...)
#endif

// Dump buffer when a new object is inserted.
#ifdef LOG_DUMP_OBJECT_BUFFER
#define LogDumpObjectBuffer(...)\
                                {\
                                  if(doLogDumpObjectBuffer) \
                                    LogStatement("LogDumpObjectBuffer: " __VA_ARGS__)\
                                  if(flushLogDumpObjectBuffer)\
                                    LogFlush();\
                                }
#else
#define LogDumpObjectBuffer(...)
#endif

// Log control operations on the simulation.
#ifdef LOG_SIMULATION_CONTROLS
#define LogSimulationControls(...)  \
                                {\
                                  if(doLogSimulationControls) \
                                    LogStatement("LogSimulationControls: " __VA_ARGS__)\
                                  if(flushLogSimulationControls)\
                                    LogFlush();\
                                }
#else
#define LogSimulationControls(...)
#endif

// Log changes to the window (eg resizing).
#ifdef LOG_WINDOW_OPERATIONS
#define LogWindowOperations(...)   \
                                {\
                                  if(doLogWindowOperations) \
                                    LogStatement("LogWindowOperations: " __VA_ARGS__)\
                                  if(flushLogWindowOperations)\
                                    LogFlush();\
                                }
#else
#define LogWindowOperations(...)
#endif

// Log camera variables every frame
#ifdef LOG_CAMERA_FRAMES
#define LogCameraFrames(...)    {\
                                  if(doLogCameraFrames) \
                                    LogStatement("LogCameraFrames: " __VA_ARGS__)\
                                  if(flushLogCameraFrames)\
                                    LogFlush();\
                                }
#else
#define LogCameraFrames(...)
#endif

// Log when the GLFW library needs to report errors
#ifdef LOG_GLFW_ERRORS
#define LogGLFWErrors(...)    {\
                                if(doLogGLFWErrors) \
                                  LogStatement("LogGLFWErrors: " __VA_ARGS__)\
                                if(flushLogGLFWErrors)\
                                  LogFlush();\
                              }
#else
#define LogGLFWErrors(...)
#endif


// =======================================================================================
// Logging options to do with materials and textures

// Log texture atlas creation
#ifdef LOG_TEXTURE_ATLAS
#define LogTextureAtlas(...)    {\
                                  if(doLogTextureAtlas) \
                                    LogStatement("LogTextureAtlas: " __VA_ARGS__)\
                                  if(flushLogTextureAtlas)\
                                    LogFlush();\
                                }
#else
#define LogTextureAtlas(...)
#endif

// Log weird conditions in texture atlas creation
#ifdef LOG_ATLAS_ANOMALIES
#define LogAtlasAnomalies(...)    {\
                                    if(doLogAtlasAnomalies) \
                                      LogStatement("LogAtlasAnomalies: " __VA_ARGS__)\
                                    if(flushLogAtlasAnomalies)\
                                      LogFlush();\
                                  }
#else
#define LogAtlasAnomalies(...)
#endif

// Log details of which image is where in atlas
#ifdef LOG_ATLAS_PLACEMENT
#define LogAtlasPlacement(...)    {\
                                    if(doLogAtlasPlacement) \
                                      LogStatement("LogAtlasPlacement: " __VA_ARGS__)\
                                    if(flushLogAtlasPlacement)\
                                      LogFlush();\
                                  }
#else
#define LogAtlasPlacement(...)
#endif

// Log when a material is selected.
#ifdef LOG_MATERIAL_SELECTIONS
#define LogMaterialSelections(...)  \
                                {\
                                  if(doLogMaterialSelections) \
                                    LogStatement("LogMaterialSelections: " __VA_ARGS__)\
                                  if(flushLogMaterialSelections)\
                                    LogFlush();\
                                }
#else
#define LogMaterialSelections(...)
#endif

// Log storage and use of the path map
#ifdef LOG_PATH_MAP
#define LogPathMap(...)   {\
                            if(doLogPathMap) \
                              LogStatement("LogPathMap: " __VA_ARGS__)\
                            if(flushLogPathMap)\
                              LogFlush();\
                          }
#else
#define LogPathMap(...)
#endif


// =======================================================================================
// Logging options to do with sky sampling and lighting

// Log the setup of the sky sampling model
#ifdef LOG_SKY_SAMPLE_INIT
#define LogSkySampleInit(...)   {\
                                  if(doLogSkySampleInit) \
                                    LogStatement("LogSkySampleInit: " __VA_ARGS__)\
                                  if(flushLogSkySampleInit)\
                                    LogFlush();\
                                }
#else
#define LogSkySampleInit(...) 
#endif

// Log problems in accessing files via libgdal
#ifdef LOG_GDAL_ERROR
#define LogGdalError(...)   {\
                              if(doLogGdalError) \
                                LogStatement("LogGdalError: " __VA_ARGS__)\
                              if(flushLogGdalError)\
                                LogFlush();\
                            }
#else
#define LogGdalError(...)
#endif


// =======================================================================================
// Logging options to do with soil stuff
            
// Log problems in the soil database operation
#ifdef LOG_SOIL_DB_ERR
#define LogSoilDbErr(...)   {\
                              if(doLogSoilDbErr) \
                                LogStatement("LogSoilDbErr: " __VA_ARGS__)\
                              if(flushLogSoilDbErr)\
                                LogFlush();\
                            }
#else
#define LogSoilDbErr(...)
#endif

// Log normal operations on the soil database
#ifdef LOG_SOIL_DB_OPS
#define LogSoilDbOps(...)   {\
                              if(doLogSoilDbOps) \
                                LogStatement("LogSoilDbOps: " __VA_ARGS__)\
                              if(flushLogSoilDbOps)\
                                LogFlush();\
                            }
#else
#define LogSoilDbOps(...)
#endif

// Log extra details in the soil database operation
#ifdef LOG_SOIL_DB_DETAILS
#define LogSoilDbDetails(...)   {\
                                  if(doLogSoilDbDetails) \
                                    LogStatement("LogSoilDbDetails: " __VA_ARGS__)\
                                  if(flushLogSoilDbDetails)\
                                    LogFlush();\
                                }
#else
#define LogSoilDbDetails(...)
#endif

// Log every field read from the HSWD database
#ifdef LOG_HSWD_EXHAUSTIVE
#define LogHSWDExhaustive(...)    {\
                                    if(doLogHSWDExhaustive) \
                                      LogStatement("LogHSWDExhaustive: " __VA_ARGS__)\
                                    if(flushLogHSWDExhaustive)\
                                      LogFlush();\
                                  }
#else
#define LogHSWDExhaustive(...)
#endif

// Log details of reading from a BILFILE
#ifdef LOG_BILFILE_DETAILS
#define LogBilFileDetails(...)    {\
                                    if(doLogBilFileDetails) \
                                      LogStatement("LogBilFileDetails: " __VA_ARGS__)\
                                    if(flushLogBilFileDetails)\
                                      LogFlush();\
                                  }
#else
#define LogBilFileDetails(...)
#endif


// =======================================================================================
// Logging options for Trees/Plants

// Log when a tree is selected.
#ifdef LOG_TREE_SELECTIONS
#define LogTreeSelections(...)    {\
                                    if(doLogTreeSelections) \
                                      LogStatement("LogTreeSelections: " __VA_ARGS__)\
                                    if(flushLogTreeSelections)\
                                      LogFlush();\
                                  }
#else
#define LogTreeSelections(...)
#endif

// Log when a tree is read in from a file or url
#ifdef LOG_TREE_READS
#define LogTreeReads(...)   {\
                              if(doLogTreeReads) \
                                LogStatement("LogTreeReads: " __VA_ARGS__)\
                              if(flushLogTreeReads)\
                                LogFlush();\
                            }
#else
#define LogTreeReads(...)
#endif

// Log high level actions in simulating tree growth.
#ifdef LOG_TREE_SIM_OVERVIEW
#define LogTreeSimOverview(...)   {\
                                    if(doLogTreeSimOverview) \
                                      LogStatement("LogTreeSimOverview: " __VA_ARGS__)\
                                    if(flushLogTreeSimOverview)\
                                      LogFlush();\
                                  }
#else
#define LogTreeSimOverview(...)
#endif

// Log all the gory details of simulated tree growth.
#ifdef LOG_TREE_SIM_DETAILS
#define LogTreeSimDetails(...)    {\
                                    if(doLogTreeSimDetails) \
                                      LogStatement("LogTreeSimDetails: " __VA_ARGS__)\
                                    if(flushLogTreeSimDetails)\
                                      LogFlush();\
                                  }
#else
#define LogTreeSimDetails(...)
#endif

// Log details of the growth model.
#ifdef LOG_GROWTH_MODEL
#define LogGrowthModel(...)   {\
                                if(doLogGrowthModel) \
                                  LogStatement("LogGrowthModel: " __VA_ARGS__)\
                                if(flushLogGrowthModel)\
                                  LogFlush();\
                              }
#else
#define LogGrowthModel(...)
#endif

// Log updating the tree's bounding box
#ifdef LOG_TREE_BOUNDING_BOX
#define LogTreeBoundingBox(...)   {\
                                    if(doLogTreeBoundingBox) \
                                      LogStatement("LogTreeBoundingBox: " __VA_ARGS__)\
                                    if(flushLogTreeBoundingBox)\
                                      LogFlush();\
                                  }
#else
#define LogTreeBoundingBox(...)
#endif

// Log matching a ray to a tree
#ifdef LOG_TREE_MATCH_RAY
#define LogTreeMatchRay(...)    {\
                                  if(doLogTreeMatchRay) \
                                    LogStatement("LogTreeMatchRay: " __VA_ARGS__)\
                                  if(flushLogTreeMatchRay)\
                                    LogFlush();\
                                }
#else
#define LogTreeMatchRay(...)
#endif

// Log trees being rendered.
#ifdef LOG_TREE_VISUALIZATION
#define LogTreeVisualization(...)   {\
                                      if(doLogTreeVisualization) \
                                        LogStatement("LogTreeVisualization: " __VA_ARGS__)\
                                      if(flushLogTreeVisualization)\
                                        LogFlush();\
                                    }
#else
#define LogTreeVisualization(...)
#endif

// Log every twig being rendered.
#ifdef LOG_TREE_VIS_DETAILS
#define LogTreeVisDetails(...)    {\
                                    if(doLogTreeVisDetails) \
                                      LogStatement("LogTreeVisDetails: " __VA_ARGS__)\
                                    if(flushLogTreeVisDetails)\
                                      LogFlush();\
                                  }
#else
#define LogTreeVisDetails(...)
#endif

// Log bark color and texture details.
#ifdef LOG_BARK_DISPLAY
#define LogBarkDisplay(...)   {\
                                if(doLogBarkDisplay) \
                                  LogStatement("LogBarkDisplay: " __VA_ARGS__)\
                                if(flushLogBarkDisplay)\
                                  LogFlush();\
                              }
#else
#define LogBarkDisplay(...)
#endif

// Log tree bounding box opacity.
#ifdef LOG_TREE_OPACITY
#define LogTreeOpacity(...)   {\
                                if(doLogTreeOpacity) \
                                  LogStatement("LogTreeOpacity: " __VA_ARGS__)\
                                if(flushLogTreeOpacity)\
                                  LogFlush();\
                              }
#else
#define LogTreeOpacity(...)
#endif

// Log tree thread analysis graph.
#ifdef LOG_TREE_GRAPH
#define LogTreeGraph(...)   {\
                              if(doLogTreeGraph) \
                                LogStatement("LogTreeGraph: " __VA_ARGS__)\
                              if(flushLogTreeGraph)\
                                LogFlush();\
                            }
#else
#define LogTreeGraph(...)
#endif

// Log process of looking for OTDL files.
#ifdef LOG_OTDL_FILE_SEARCH
#define LogOTDLFileSearch(...)    {\
                                    if(doLogOTDLFileSearch) \
                                      LogStatement("LogOTDLFileSearch: " __VA_ARGS__)\
                                    if(flushLogOTDLFileSearch)\
                                      LogFlush();\
                                  }
#else
#define LogOTDLFileSearch(...)
#endif

// Log clear errors in tree related operations.
#ifdef LOG_TREE_ERRORS
#define LogTreeErrors(...)    {\
                                if(doLogTreeErrors) \
                                  LogStatement("LogTreeErrors: " __VA_ARGS__)\
                                if(flushLogTreeErrors)\
                                  LogFlush();\
                              }
#else
#define LogTreeErrors(...)
#endif


// =======================================================================================
// Logging options for buildings (gables, sheds, etc)

// Log creation and buffering of BuildingRects
#ifdef LOG_BUILD_RECT_DETAILS
#define LogBuildRectDetails(...)    {\
                                      if(doLogBuildRectDetails) \
                                        LogStatement("LogBuildRectDetails: " __VA_ARGS__)\
                                      if(flushLogBuildRectDetails)\
                                        LogFlush();\
                                    }
#else
#define LogBuildRectDetails(...)
#endif

// Buffering of Gables, sheds, etc
#ifdef LOG_BUILDING_BUFFER
#define LogBuildingBuffer(...)    {\
                                    if(doLogBuildingBuffer) \
                                      LogStatement("LogBuildingBuffer: " __VA_ARGS__)\
                                    if(flushLogBuildingBuffer)\
                                      LogFlush();\
                                  }
#else
#define LogBuildingBuffer(...)
#endif


// =======================================================================================
// Logging options for groups of objects

// Log when groups are created and objects are added.
#ifdef LOG_GROUP_ADDITIONS
#define LogGroupAdditions(...)    {\
                                    if(doLogGroupAdditions) \
                                      LogStatement("LogGroupAdditions: " __VA_ARGS__)\
                                    if(flushLogGroupAdditions)\
                                      LogFlush();\
                                  }
#else
#define LogGroupAdditions(...)
#endif

// Log the process of initiating a new control group.
#ifdef LOG_CONTROL_GROUP_INIT
#define LogControlGroupInit(...)    {\
                                      if(doLogControlGroupInit) \
                                        LogStatement("LogControlGroupInit: " __VA_ARGS__)\
                                      if(flushLogControlGroupInit)\
                                        LogFlush();\
                                    }
#else
#define LogControlGroupInit(...)
#endif

// Log ray matching in groups.
#ifdef LOG_GROUP_MATCH_RAY
#define LogGroupMatchRay(...)   {\
                                  if(doLogGroupMatchRay) \
                                    LogStatement("LogGroupMatchRay: " __VA_ARGS__)\
                                  if(flushLogMatchRay)\
                                    LogFlush();\
                                }
#else
#define LogGroupMatchRay(...)
#endif

// Log groups found during self validation.
#ifdef LOG_FIND_GROUPS
#define LogFindGroups(...)    {\
                                if(doLogFindGroups) \
                                  LogStatement("LogFindGroups: " __VA_ARGS__)\
                                if(flushLogFindGroups)\
                                  LogFlush();\
                              }
#else
#define LogFindGroups(...)
#endif

// Log the process of rendering a pathTube
#ifdef LOG_PATHTUBE_BUFFER
#define LogPathTubeBuffer(...)    {\
                                    if(doLogPathTubeBuffer) \
                                      LogStatement("LogPathTubeBuffer: " __VA_ARGS__)\
                                    if(flushLogPathTubeBuffer)\
                                      LogFlush();\
                                  }
#else
#define LogPathTubeBuffer(...)
#endif


// =======================================================================================
// Logging options for the HTTP debug interface

// Log validity problems in the HTTP request
#ifdef LOG_REQUEST_ERRORS
#define LogRequestErrors(...)   {\
                                  if(doLogRequestErrors) \
                                    LogStatement("LogRequestErrors: " __VA_ARGS__)\
                                  if(flushLogRequestErrors)\
                                    LogFlush();\
                                }
#else
#define LogRequestErrors(...)
#endif

// Log problems encountered building the response
#ifdef LOG_RESPONSE_ERRORS
#define LogResponseErrors(...)    {\
                                    if(doLogResponseErrors) \
                                      LogStatement("LogResponseErrors: " __VA_ARGS__)\
                                    if(flushLogResponseErrors)\
                                      LogFlush();\
                                  }
#else
#define LogResponseErrors(...)
#endif

// Log operations on the main HTTP buffers
#ifdef LOG_HTTP_BUFFER_OPS
#define LogHTTPBufferOps(...)   {\
                                  if(doLogHTTPBufferOps) \
                                    LogStatement("LogHTTPBufferOps: " __VA_ARGS__)\
                                  if(flushLogHTTPBufferOps)\
                                    LogFlush();\
                                }
#else
#define LogHTTPBufferOps(...)
#endif

// Log normal details of HTTP operations
#ifdef LOG_HTTP_DETAILS
#define LogHTTPDetails(...)   {\
                                if(doLogHTTPDetails) \
                                  LogStatement("LogHTTPDetails: " __VA_ARGS__)\
                                if(flushLogHTTPDetails)\
                                  LogFlush();\
                              }
#else
#define LogHTTPDetails(...)
#endif

// Log which connections get processed where
#ifdef LOG_HTTP_LOAD_BALANCE
#define LogHTTPLoadBalance(...)   {\
                                    if(doLogHTTPLoadBalance) \
                                      LogStatement("LogHTTPLoadBalance: " __VA_ARGS__)\
                                    if(flushLogHTTPLoadBalance)\
                                      LogFlush();\
                                  }
#else
#define LogHTTPLoadBalance(...)
#endif


// Log exactly what happens when parsing a request
#ifdef LOG_REQUEST_PARSING
#define LogRequestParsing(...)    {\
                                    if(doLogRequestParsing) \
                                      LogStatement("LogRequestParsing: " __VA_ARGS__)\
                                    if(flushLogRequestParsing)\
                                      LogFlush();\
                                  }
#else
#define LogRequestParsing(...)
#endif

// Log as the main thread processes pseudo-actions
#ifdef LOG_PSEUDO_ACTIONS
#define LogPseudoActions(...)   {\
                                  if(doLogPseudoActions) \
                                    LogStatement("LogPseudoActions: " __VA_ARGS__)\
                                  if(flushLogPseudoActions)\
                                    LogFlush();\
                                }
#else
#define LogPseudoActions(...)
#endif


// =======================================================================================
// Logging options to do with quadtree insertions

// Log the initial setup of the quadtree.
#ifdef LOG_QUADTREE_CREATION
#define LogQuadtreeCreation(...)    {\
                                      if(doLogQuadtreeCreation) \
                                        LogStatement("LogQuadtreeCreation: " __VA_ARGS__)\
                                      if(flushLogQuadtreeCreation)\
                                        LogFlush();\
                                    }
#else
#define LogQuadtreeCreation(...)
#endif

// Log the process of a new object being put in the quadtree.
#ifdef LOG_QUADTREE_INSERTIONS
#define LogQuadtreeInsertions(...)  {\
                                      if(doLogQuadtreeInsertions) \
                                        LogStatement("LogQuadtreeInsertions: " __VA_ARGS__)\
                                      if(flushLogQuadtreeInsertions)\
                                        LogFlush();\
                                    }
#else
#define LogQuadtreeInsertions(...)
#endif

// Log changes to the quadtree bounding boxes.
#ifdef LOG_QUADTREE_BOUND_BOX
#define LogQuadtreeBoundBox(...)    {\
                                      if(doLogQuadtreeBoundBox) \
                                        LogStatement("LogQuadtreeBoundBox: " __VA_ARGS__)\
                                      if(flushLogQuadtreeBoundBox)\
                                        LogFlush();\
                                    }
#else
#define LogQuadtreeBoundBox(...)
#endif

// Log the process of matching a ray in the quadtree.
#ifdef LOG_QUADTREE_MATCH_RAY
#define LogQuadtreeMatchRay(...)    {\
                                      if(doLogQuadtreeMatchRay) \
                                        LogStatement("LogQuadtreeMatchRay: " __VA_ARGS__)\
                                      if(flushLogQuadtreeMatchRay)\
                                        LogFlush();\
                                    }
#else
#define LogQuadtreeMatchRay(...)
#endif

// Log process of objects being buffered for display.
#ifdef LOG_DISPLAYLIST_BUFFER
#define LogDisplayListBuffer(...)   {\
                                      if(doLogDisplayListBuffer) \
                                        LogStatement("LogDisplayListBuffer: " __VA_ARGS__)\
                                      if(flushLogDisplayListBuffer)\
                                        LogFlush();\
                                    }
#else
#define LogDisplayListBuffer(...)
#endif

// Log the process of estimating/changing object sizes
#ifdef LOG_QUADTREE_OBJ_SIZES
#define LogQuadtreeObjSizes(...)    {\
                                      if(doLogQuadtreeObjSizes) \
                                        LogStatement("LogQuadtreeObjSizes: " __VA_ARGS__)\
                                      if(flushLogQuadtreeObjSizes)\
                                        LogFlush();\
                                    }
#else
#define LogQuadtreeObjSizes(...)
#endif


// =======================================================================================
// Logging options to do with other infrastructure

// Log the id of each newly created oject
#ifdef LOG_OBJECT_CREATION
#define LogObjectCreation(...)    {\
                                    if(doLogObjectCreation) \
                                      LogStatement("LogObjectCreation: " __VA_ARGS__)\
                                    if(flushLogObjectCreation)\
                                      LogFlush();\
                                  }
#else
#define LogObjectCreation(...)
#endif

// Log the workings of the triangle buffers.
#ifdef LOG_TRIANGLE_BUFFER_OPS
#define LogTriangleBufferOps(...)   {\
                                      if(doLogTriangleBufferOps) \
                                        LogStatement("LogTriangleBufferOps: " __VA_ARGS__)\
                                      if(flushLogTriangleBufferOps)\
                                        LogFlush();\
                                    }
#else
#define LogTriangleBufferOps(...)
#endif

// Log estimates of sizes needed.
#ifdef LOG_TRIANGLE_BUF_ESTIMATES
#define LogTriangleBufEstimates(...)  \
                              {\
                                if(doLogTriangleBufEstimates) \
                                  LogStatement("LogTriangleBufEstimates: " __VA_ARGS__)\
                                if(flushLogTriangleBufEstimates)\
                                  LogFlush();\
                              }
#else
#define LogTriangleBufEstimates(...)
#endif

// Log when the triangle buffer is rebuilt.
#ifdef LOG_TRIANGLE_BUF_REBUILDS
#define LogTriangleBufRebuilds(...)  \
                              {\
                                if(doLogTriangleBufRebuilds) \
                                  LogStatement("LogTriangleBufRebuilds: " __VA_ARGS__)\
                                if(flushLogTriangleBufRebuilds)\
                                  LogFlush();\
                              }
#else
#define LogTriangleBufRebuilds(...)
#endif

// Validate the condition of triangle buffers on gpu send
#ifdef LOG_VALID_TRIANGLE_BUFS
#define LogValidTriangleBufs(...)  \
                              {\
                                if(doLogValidTriangleBufs) \
                                  LogStatement("LogValidTriangleBufs: " __VA_ARGS__)\
                                if(flushLogValidTriangleBufs)\
                                  LogFlush();\
                              }
#else
#define LogValidTriangleBufs(...)
#endif

// Log actual errors in the triangle buffers.
#ifdef LOG_TRIANGLE_BUFFER_ERRS
#define LogTriangleBufferErrs(...) \
                              {\
                                if(doLogTriangleBufferErrs) \
                                  LogStatement("LogTriangleBufferErrs: " __VA_ARGS__)\
                                if(flushLogTriangleBufferErrs)\
                                  LogFlush();\
                              }
#else
#define LogTriangleBufferErrs(...)
#endif


// =======================================================================================
// Logging options to do with the task queues and task queue farms

// Log what the task queue farms are doing
#ifdef LOG_TASK_QUEUE_FARM_OPS
#define LogTaskQueueFarmOps(...)    \
                              {\
                                if(doLogTaskQueueFarmOps) \
                                  LogStatement("LogTaskQueueFarmOps: " __VA_ARGS__)\
                                if(flushLogTaskQueueFarmOps)\
                                  LogFlush();\
                              }
#else
#define LogTaskQueueFarmOps(...)
#endif


// =======================================================================================
// Logging options to do with the Bezier Patch code

// Log the process of fitting the Bezier patch to height data
#ifdef LOG_BEZIER_FIT
#define LogBezierFit(...)   {\
                              if(doLogBezierFit) \
                                LogStatement("LogBezierFit: " __VA_ARGS__)\
                              if(flushLogBezierFit)\
                                LogFlush();\
                            }
#else
#define LogBezierFit(...)
#endif

// Log the process of matching ray intersection with a Bezier patch
#ifdef LOG_BEZIER_MATCH_RAY
#define LogBezierMatchRay(...)    {\
                                    if(doLogBezierMatchRay) \
                                      LogStatement("LogBezierMatchRay: " __VA_ARGS__)\
                                    if(flushLogBezierMatchRay)\
                                      LogFlush();\
                                  }
#else
#define LogBezierMatchRay(...)
#endif

// Log finding the altitudes of objects above the land
#ifdef LOG_OBJECT_ALTITUDES
#define LogObjectAltitudes(...)   {\
                                    if(doLogObjectAltitudes) \
                                      LogStatement("LogObjectAltitudes: " __VA_ARGS__)\
                                    if(flushLogObjectAltitudes)\
                                      LogFlush();\
                                  }
#else
#define LogObjectAltitudes(...)
#endif


#endif //LOGGING_H

// =======================================================================================

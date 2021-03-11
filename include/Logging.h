// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// File that controls and implements the logging system.  A variety of options
// are defined at the beginning that implement conditional compilation of all the
// different kinds of logging.

#include "MemoryTracker.h"

// =======================================================================================
// Logging options to do with overall control flow and speed

#define LOG_FRAME_STARTS      // Log each frame as it begins
#define LOG_CLOSE_DOWN        // Log when we exit.
#define LOG_STRUCTURE_SIZES   // Log the size of structures on this platform at startup.
#define LOG_OPENGL_CONSTANTS  // Log various openGL parameters
#define LOG_TREE_VALIDATION   // Validate data structures after every frame


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
//#define LOG_DOUBLE_CLICK        // Location and intra-click timing of mouse double clicks
//#define LOG_MOUSE_RAY_POINT     // Where the mouse is pointing in 3d space
//#define LOG_LAND_HEIGHTS        // Log when a new land height is entered.
//#define LOG_OBJECT_INSERTIONS   // Log when a new object is inserted in scene.
#define LOG_SIMULATION_CONTROLS // Log control operations on the simulation.


// =======================================================================================
// Logging options to do with materials and textures

//#define LOG_TEXTURE_ATLAS       // Log texture atlas creation
//#define LOG_ATLAS_ANOMALIES     // Log weird conditions in texture atlas creation
//#define LOG_ATLAS_PLACEMENT     // Log details of which image is where in atlas
//#define LOG_MATERIAL_SELECTIONS // Log when a material is selected.
//#define LOG_PATH_MAP            // Log storage and use of the path map


// =======================================================================================
// Logging options for Trees/Plants

#define LOG_TREE_SELECTIONS       // Log when a tree is selected.
#define LOG_TREE_READS            // Log when a tree is read in from a file or url
//#define LOG_TREE_SIM_OVERVIEW     // Log high level actions in simulating tree growth.
//#define LOG_TREE_SIM_DETAILS      // Log all the gory details of simulated tree growth.
//#define LOG_GROWTH_MODEL          // Log details of the growth model.
#define LOG_TREE_BOUNDING_BOX     // Log updating the tree's bounding box
//#define LOG_TREE_MATCH_RAY        // Log matching a ray to a tree
//#define LOG_TREE_VISUALIZATION    // Log trees being rendered.
//#define LOG_TREE_VIS_DETAILS      // Log every twig being rendered.
//#define LOG_BARK_DISPLAY          // Log bark color and texture details.
#define LOG_TREE_ERRORS           // Log clear errors in tree related operations.


// =======================================================================================
// Logging options for the HTTP debug interface

#define LOG_REQUEST_ERRORS      // Log validity problems in the HTTP request
#define LOG_RESPONSE_ERRORS     // Log problems encountered building the response
//#define LOG_HTTP_BUFFER_OPS        // Log operations on the main HTTP buffers
//#define LOG_HTTP_DETAILS        // Log normal details of HTTP operations


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

//#define LOG_TRIANGLE_BUFFER_OPS     // Log the workings of the triangle buffers.
//#define LOG_TRIANGLE_BUF_ESTIMATES  // Log estimates of sizes needed.
#define LOG_TRIANGLE_BUFFER_ERRS    // Log actual errors the triangle buffers.


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

//XX __DATE__ and __TIME__ are compile times, not run times!!
#define LogStatement(...) fprintf(LogFile, __DATE__ " " __TIME__ ": " __VA_ARGS__)

void LogInit(void);
extern FILE* LogFile;

// =======================================================================================
// Control variables for real time control of which logging is turned on

#ifndef LOGGING_IMPLEMENTATION

// Logging options to do with overall control flow and speed
extern bool doLogFrameStarts;     // Log each frame as it begins
extern bool doLogCloseDown;       // Log when we exit
extern bool doLogStructureSizes;  // Log the size of structures on this platform at startup.
extern bool doLogOpenGLConstants; // Log various openGL parameters
extern bool doLogTreeValidation;  // Validate data structures after every frame

// Logging options to do with parsing and validating OLDF and OTDL files
extern bool doLogOLDFValidity;    // Log validity problems in the file
extern bool doLogOLDFDetails;     // Log details of valid OLDF objects
extern bool doLogOTDLValidity;    // Log validity problems in an OTDL object
extern bool doLogOTDLDetails;     // Log details of valid OTDL objects

// Logging options to do with interface controls
extern bool doLogMouseLocation;       // where the mouse is on the screen each frame
extern bool doLogMouseClick;          // Location of single mouse clicks
extern bool doLogDoubleClick;         // Location and intra-click timing of mouse double clicks
extern bool doLogMouseRayPoint;       // Where the mouse is pointing in 3d space
extern bool doLogLandHeights;         // Log when a new land height is entered.
extern bool doLogObjectInsertions;    // Log when a new object is inserted in scene.
extern bool doLogSimulationControls;  // Log control operations on the simulation.

// Logging options to do with materials and textures
extern bool doLogTextureAtlas;        // Log texture atlas creation
extern bool doLogAtlasAnomalies;      // Log weird conditions in texture atlas creation
extern bool doLogAtlasPlacement;      // Log details of which image is where in atlas
extern bool doLogMaterialSelections;  // Log when a material is selected.
extern bool doLogPathMap;             // Log storage and use of the path map

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
extern bool doLogTreeErrors;          // Log clear errors in tree related operations.

// Logging options for the HTTP debug interface
extern bool doLogRequestErrors;       // Log validity problems in the HTTP request
extern bool doLogResponseErrors;      // Log problems encountered building the response
extern bool doLogHTTPBufferOps;       // Log operations on the main HTTP buffers
extern bool doLogHTTPDetails;         // Log normal details of HTTP operations

// Logging options to do with quadtree operations

extern bool doLogQuadtreeCreation;    // Log the initial setup of the quadtree.
extern bool doLogQuadtreeInsertions;  // Log the a new object being put in the quadtree.
extern bool doLogQuadtreeBoundBox;    // Log changes to the quadtree bounding boxes.
extern bool doLogQuadtreeMatchRay;    // Log the process of matching a ray in the quadtree.
extern bool doLogDisplayListBuffer;   // Log the process of objects being buffered for display.
extern bool doLogQuadtreeObjSizes;    // Log the process of estimating/changing object sizes

// Logging options to do with other infrastructure
extern bool doLogTriangleBufferOps;     // Log the workings of the triangle buffers.
extern bool doLogTriangleBufEstimates;  // Log estimates of sizes needed.
extern bool doLogTriangleBufferErrs;    // Log actual errors the triangle buffers.

// Logging options to do with the Bezier Patch code
extern bool doLogBezierFit;           // Log the fitting of a Bezier patch to height data
extern bool doLogBezierMatchRay;      // Log the process of matching a ray to the patch.
extern bool doLogObjectAltitudes;     // Log finding the altitudes of objects above the land

#endif  //LOGGING_IMPLEMENTATION - need this here.


// =======================================================================================
// Logging options to do with overall control flow and speed

// Log each frame as it begins
#ifdef LOG_FRAME_STARTS
#define LogFrameStarts(...) if(doLogFrameStarts) LogStatement(__VA_ARGS__)
#else
#define LogFrameStarts(...)
#endif

// Log when we exit
#ifdef LOG_CLOSE_DOWN
#define LogCloseDown(...) if(doLogCloseDown) LogStatement(__VA_ARGS__)
#else
#define LogCloseDown(...)
#endif

// Log the size of structures on this platform at startup.
#ifdef LOG_STRUCTURE_SIZES
#define LogStructureSizes(...) if(doLogStructureSizes) LogStatement(__VA_ARGS__)
#else
#define LogStructureSizes(...)
#endif

// Log various openGL parameters
#ifdef LOG_OPENGL_CONSTANTS
#define LogOpenGLConstants(...) if(doLogOpenGLConstants) LogStatement(__VA_ARGS__)
#else
#define LogOpenGLConstants(...)
#endif

// Validate data structures after every frame
#ifdef LOG_TREE_VALIDATION
#define LogTreeValidation(...) if(doLogTreeValidation) LogStatement(__VA_ARGS__)
#else
#define LogTreeValidation(...)
#endif

// =======================================================================================
// Logging options to do with parsing and validating OLDF and OTDL files

// Log validity problems in the file
#ifdef LOG_OLDF_VALIDITY
#define LogOLDFValidity(...) if(doLogOLDFValidity) LogStatement(__VA_ARGS__)
#else
#define LogOLDFValidity(...)
#endif

// Log details of valid OLDF objects
#ifdef LOG_OLDF_DETAILS
#define LogOLDFDetails(...) if(doLogOLDFDetails) LogStatement(__VA_ARGS__)
#else
#define LogOLDFDetails(...)
#endif

// Log validity problems in an OTDL object
#ifdef LOG_OTDL_VALIDITY
#define LogOTDLValidity(...) if(doLogOTDLValidity) LogStatement(__VA_ARGS__)
#else
#define LogOTDLValidity(...)
#endif

// Log details of valid OTDL objects
#ifdef LOG_OTDL_DETAILS
#define LogOTDLDetails(...) if(doLogOTDLDetails) LogStatement(__VA_ARGS__)
#else
#define LogOTDLDetails(...)
#endif


// =======================================================================================
// Logging options to do with interface controls

// where the mouse is on the screen each frame
#ifdef LOG_MOUSE_LOCATION
#define LogMouseLocation(...) if(doLogMouseLocation) LogStatement(__VA_ARGS__)
#else
#define LogMouseLocation(...)
#endif

// Location and intra-click timing of mouse double clicks
#ifdef LOG_MOUSE_CLICK
#define LogMouseClick(...) if(doLogMouseClick) LogStatement(__VA_ARGS__)
#else
#define LogMouseClick(...)
#endif

// Location and intra-click timing of mouse double clicks
#ifdef LOG_DOUBLE_CLICK
#define LogDoubleClick(...) if(doLogDoubleClick) LogStatement(__VA_ARGS__)
#else
#define LogDoubleClick(...)
#endif

// Where the mouse is pointing in 3d space
#ifdef LOG_MOUSE_RAY_POINT
#define LogMouseRayPoint(...) if(doLogMouseRayPoint) LogStatement(__VA_ARGS__)
#else
#define LogMouseRayPoint(...)
#endif

// Log when a new land height is entered.
#ifdef LOG_LAND_HEIGHTS
#define LogLandHeights(...) if(doLogLandHeights) LogStatement(__VA_ARGS__)
#else
#define LogLandHeights(...)
#endif

// Log when a new object is inserted.
#ifdef LOG_OBJECT_INSERTIONS
#define LogObjectInsertions(...) if(doLogObjectInsertions) LogStatement(__VA_ARGS__)
#else
#define LogObjectInsertions(...)
#endif

// Log control operations on the simulation.
#ifdef LOG_SIMULATION_CONTROLS
#define LogSimulationControls(...) if(doLogSimulationControls) LogStatement(__VA_ARGS__)
#else
#define LogSimulationControls(...)
#endif


// =======================================================================================
// Logging options to do with materials and textures

// Log texture atlas creation
#ifdef LOG_TEXTURE_ATLAS
#define LogTextureAtlas(...) if(doLogTextureAtlas) LogStatement(__VA_ARGS__)
#else
#define LogTextureAtlas(...)
#endif

// Log weird conditions in texture atlas creation
#ifdef LOG_ATLAS_ANOMALIES
#define LogAtlasAnomalies(...) if(doLogAtlasAnomalies) LogStatement(__VA_ARGS__)
#else
#define LogAtlasAnomalies(...)
#endif

// Log details of which image is where in atlas
#ifdef LOG_ATLAS_PLACEMENT
#define LogAtlasPlacement(...) if(doLogAtlasPlacement) LogStatement(__VA_ARGS__)
#else
#define LogAtlasPlacement(...)
#endif

// Log when a material is selected.
#ifdef LOG_MATERIAL_SELECTIONS
#define LogMaterialSelections(...) if(doLogMaterialSelections) LogStatement(__VA_ARGS__)
#else
#define LogMaterialSelections(...)
#endif

// Log storage and use of the path map
#ifdef LOG_PATH_MAP
#define LogPathMap(...) if(doLogPathMap) LogStatement(__VA_ARGS__)
#else
#define LogPathMap(...)
#endif


// =======================================================================================
// Logging options for Trees/Plants

// Log when a tree is selected.
#ifdef LOG_TREE_SELECTIONS
#define LogTreeSelections(...) if(doLogTreeSelections) LogStatement(__VA_ARGS__)
#else
#define LogTreeSelections(...)
#endif

// Log when a tree is read in from a file or url
#ifdef LOG_TREE_READS
#define LogTreeReads(...) if(doLogTreeReads) LogStatement(__VA_ARGS__)
#else
#define LogTreeReads(...)
#endif

// Log high level actions in simulating tree growth.
#ifdef LOG_TREE_SIM_OVERVIEW
#define LogTreeSimOverview(...) if(doLogTreeSimOverview) LogStatement(__VA_ARGS__)
#else
#define LogTreeSimOverview(...)
#endif

// Log all the gory details of simulated tree growth.
#ifdef LOG_TREE_SIM_DETAILS
#define LogTreeSimDetails(...) if(doLogTreeSimDetails) LogStatement(__VA_ARGS__)
#else
#define LogTreeSimDetails(...)
#endif

// Log details of the growth model.
#ifdef LOG_GROWTH_MODEL
#define LogGrowthModel(...) if(doLogGrowthModel) LogStatement(__VA_ARGS__)
#else
#define LogGrowthModel(...)
#endif

// Log updating the tree's bounding box
#ifdef LOG_TREE_BOUNDING_BOX
#define LogTreeBoundingBox(...) if(doLogTreeBoundingBox) LogStatement(__VA_ARGS__)
#else
#define LogTreeBoundingBox(...)
#endif

// Log matching a ray to a tree
#ifdef LOG_TREE_MATCH_RAY
#define LogTreeMatchRay(...) if(doLogTreeMatchRay) LogStatement(__VA_ARGS__)
#else
#define LogTreeMatchRay(...)
#endif

// Log trees being rendered.
#ifdef LOG_TREE_VISUALIZATION
#define LogTreeVisualization(...) if(doLogTreeVisualization) LogStatement(__VA_ARGS__)
#else
#define LogTreeVisualization(...)
#endif

// Log every twig being rendered.
#ifdef LOG_TREE_VIS_DETAILS
#define LogTreeVisDetails(...) if(doLogTreeVisDetails) LogStatement(__VA_ARGS__)
#else
#define LogTreeVisDetails(...)
#endif

// Log bark color and texture details.
#ifdef LOG_BARK_DISPLAY
#define LogBarkDisplay(...) if(doLogBarkDisplay) LogStatement(__VA_ARGS__)
#else
#define LogBarkDisplay(...)
#endif

// Log clear errors in tree related operations.
#ifdef LOG_TREE_ERRORS
#define LogTreeErrors(...) if(doLogTreeErrors) LogStatement(__VA_ARGS__)
#else
#define LogTreeErrors(...)
#endif


// =======================================================================================
// Logging options for the HTTP debug interface

// Log validity problems in the HTTP request
#ifdef LOG_REQUEST_ERRORS
#define LogRequestErrors(...) if(doLogRequestErrors) LogStatement(__VA_ARGS__)
#else
#define LogRequestErrors(...)
#endif

// Log problems encountered building the response
#ifdef LOG_RESPONSE_ERRORS
#define LogResponseErrors(...) if(doLogResponseErrors) LogStatement(__VA_ARGS__)
#else
#define LogResponseErrors(...)
#endif

// Log operations on the main HTTP buffers
#ifdef LOG_HTTP_BUFFER_OPS
#define LogHTTPBufferOps(...) if(doLogHTTPBufferOps) LogStatement(__VA_ARGS__)
#else
#define LogHTTPBufferOps(...)
#endif

// Log normal details of HTTP operations
#ifdef LOG_HTTP_DETAILS
#define LogHTTPDetails(...) if(doLogHTTPDetails) LogStatement(__VA_ARGS__)
#else
#define LogHTTPDetails(...)
#endif


// =======================================================================================
// Logging options to do with quadtree insertions

// Log the initial setup of the quadtree.
#ifdef LOG_QUADTREE_CREATION
#define LogQuadtreeCreation(...) if(doLogQuadtreeCreation) LogStatement(__VA_ARGS__)
#else
#define LogQuadtreeCreation(...)
#endif

// Log the process of a new object being put in the quadtree.
#ifdef LOG_QUADTREE_INSERTIONS
#define LogQuadtreeInsertions(...) if(doLogQuadtreeInsertions) LogStatement(__VA_ARGS__)
#else
#define LogQuadtreeInsertions(...)
#endif

// Log changes to the quadtree bounding boxes.
#ifdef LOG_QUADTREE_BOUND_BOX
#define LogQuadtreeBoundBox(...) if(doLogQuadtreeBoundBox) LogStatement(__VA_ARGS__)
#else
#define LogQuadtreeBoundBox(...)
#endif

// Log the process of matching a ray in the quadtree.
#ifdef LOG_QUADTREE_MATCH_RAY
#define LogQuadtreeMatchRay(...) if(doLogQuadtreeMatchRay) LogStatement(__VA_ARGS__)
#else
#define LogQuadtreeMatchRay(...)
#endif

// Log the process of a new object being put in the quadtree.
#ifdef LOG_DISPLAYLIST_BUFFER
#define LogDisplayListBuffer(...) if(doLogDisplayListBuffer) LogStatement(__VA_ARGS__)
#else
#define LogDisplayListBuffer(...)
#endif

// Log the process of estimating/changing object sizes
#ifdef LOG_QUADTREE_OBJ_SIZES
#define LogQuadtreeObjSizes(...) if(doLogQuadtreeObjSizes) LogStatement(__VA_ARGS__)
#else
#define LogQuadtreeObjSizes(...)
#endif


// =======================================================================================
// Logging options to do with other infrastructure

// Log the workings of the triangle buffers.
#ifdef LOG_TRIANGLE_BUFFER_OPS
#define LogTriangleBufferOps(...) if(doLogTriangleBufferOps) LogStatement(__VA_ARGS__)
#else
#define LogTriangleBufferOps(...)
#endif

// Log estimates of sizes needed.
#ifdef LOG_TRIANGLE_BUF_ESTIMATES
#define LogTriangleBufEstimates(...) if(doLogTriangleBufEstimates) LogStatement(__VA_ARGS__)
#else
#define LogTriangleBufEstimates(...)
#endif

// Log actual errors the triangle buffers.
#ifdef LOG_TRIANGLE_BUFFER_ERRS
#define LogTriangleBufferErrs(...) if(doLogTriangleBufferErrs) LogStatement(__VA_ARGS__)
#else
#define LogTriangleBufferErrs(...)
#endif


// =======================================================================================
// Logging options to do with the Bezier Patch code

// Log the process of fitting the Bezier patch to height data
#ifdef LOG_BEZIER_FIT
#define LogBezierFit(...) if(doLogBezierFit) LogStatement(__VA_ARGS__)
#else
#define LogBezierFit(...)
#endif

// Log the process of matching ray intersection with a Bezier patch
#ifdef LOG_BEZIER_MATCH_RAY
#define LogBezierMatchRay(...) if(doLogBezierMatchRay) LogStatement(__VA_ARGS__)
#else
#define LogBezierMatchRay(...)
#endif

// Log finding the altitudes of objects above the land
#ifdef LOG_OBJECT_ALTITUDES
#define LogObjectAltitudes(...) if(doLogObjectAltitudes) LogStatement(__VA_ARGS__)
#else
#define LogObjectAltitudes(...)
#endif


// =======================================================================================

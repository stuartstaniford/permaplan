// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// File that controls and implements the logging system.  A variety of options
// are defined at the beginning that implement conditional compilation of all the
// different kinds of logging.

// =======================================================================================
// Logging options to do with overall control flow and speed

#define LOG_FRAME_STARTS      // Log each frame as it begins
#define LOG_CLOSE_DOWN        // Log when we exit.
#define LOG_STRUCTURE_SIZES   // Log the size of structures on this platform at startup.
#define LOG_OPENGL_CONSTANTS  // Log various openGL parameters


// =======================================================================================
// Logging options to do with parsing and validating OLDF and OTDL files

//#define LOG_OLDF_VALIDITY      // Log validity problems in the file
//#define LOG_OLDF_DETAILS       // Log details of valid OLDF objects
//#define LOG_OTDL_VALIDITY      // Log validity problems in an OTDL object
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

#define LOG_TREE_SELECTIONS     // Log when a tree is selected.
#define LOG_TREE_READS          // Log when a tree is read in from a file or url
#define LOG_TREE_SIM_OVERVIEW   // Log high level actions in simulating tree growth.
#define LOG_TREE_SIM_DETAILS    // Log all the gory details of simulated tree growth.
#define LOG_TREE_VISUALIZATION  // Log trees being rendered.
#define LOG_TREE_ERRORS         // Log clear errors in tree related operations.


// =======================================================================================
// Logging options for the HTTP debug interface

//#define LOG_REQUEST_ERRORS      // Log validity problems in the HTTP request
//#define LOG_RESPONSE_ERRORS     // Log problems encountered building the response
//#define LOG_HTTP_DETAILS        // Log normal details of HTTP operations


// =======================================================================================
// Logging options to do with quadtree operations

//#define LOG_QUADTREE_INSERTIONS // Log the process of a new object being put in the quadtree.
//#define LOG_DISPLAYLIST_BUFFER  // Log the process of objects being buffered for display.


// =======================================================================================
// Logging options to do with the Bezier Patch code

//#define LOG_BEZIER_FIT        // Log the process of fitting the Bezier patch to height data
//#define LOG_BEZIER_MATCH_RAY  // Log the process of matching a ray to the patch.


// =======================================================================================
// =======================================================================================
// =======================================================================================
// This is the end of the control directives, now we switch to the actual implementation
// of the logging mechanism.

// =======================================================================================
// General logging mechanism

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

#endif

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


// =======================================================================================
// Logging options to do with parsing and validating OLDF and OTDL files

// Log validity problems in the file
#ifdef LOG_OLDF_VALIDITY
#define LogOLDFValidity(...) LogStatement(__VA_ARGS__)
#else
#define LogOLDFValidity(...)
#endif

// Log details of valid OLDF objects
#ifdef LOG_OLDF_DETAILS
#define LogOLDFDetails(...) LogStatement(__VA_ARGS__)
#else
#define LogOLDFDetails(...)
#endif

// Log validity problems in an OTDL object
#ifdef LOG_OTDL_VALIDITY
#define LogOTDLValidity(...) LogStatement(__VA_ARGS__)
#else
#define LogOTDLValidity(...)
#endif

// Log details of valid OTDL objects
#ifdef LOG_OTDL_DETAILS
#define LogOTDLDetails(...) LogStatement(__VA_ARGS__)
#else
#define LogOTDLDetails(...)
#endif


// =======================================================================================
// Logging options to do with interface controls

// where the mouse is on the screen each frame
#ifdef LOG_MOUSE_LOCATION
#define LogMouseLocation(...) LogStatement(__VA_ARGS__)
#else
#define LogMouseLocation(...)
#endif

// Location and intra-click timing of mouse double clicks
#ifdef LOG_MOUSE_CLICK
#define LogMouseClick(...) LogStatement(__VA_ARGS__)
#else
#define LogMouseClick(...)
#endif

// Location and intra-click timing of mouse double clicks
#ifdef LOG_DOUBLE_CLICK
#define LogDoubleClick(...) LogStatement(__VA_ARGS__)
#else
#define LogDoubleClick(...)
#endif

// Where the mouse is pointing in 3d space
#ifdef LOG_MOUSE_RAY_POINT
#define LogMouseRayPoint(...) LogStatement(__VA_ARGS__)
#else
#define LogMouseRayPoint(...)
#endif

// Log when a new land height is entered.
#ifdef LOG_LAND_HEIGHTS
#define LogLandHeights(...) LogStatement(__VA_ARGS__)
#else
#define LogLandHeights(...)
#endif

// Log when a new object is inserted.
#ifdef LOG_OBJECT_INSERTIONS
#define LogObjectInsertions(...) LogStatement(__VA_ARGS__)
#else
#define LogObjectInsertions(...)
#endif

// Log control operations on the simulation.
#ifdef LOG_SIMULATION_CONTROLS
#define LogSimulationControls(...) LogStatement(__VA_ARGS__)
#else
#define LogSimulationControls(...)
#endif


// =======================================================================================
// Logging options to do with materials and textures

// Log texture atlas creation
#ifdef LOG_TEXTURE_ATLAS
#define LogTextureAtlas(...) LogStatement(__VA_ARGS__)
#else
#define LogTextureAtlas(...)
#endif

// Log weird conditions in texture atlas creation
#ifdef LOG_ATLAS_ANOMALIES
#define LogAtlasAnomalies(...) LogStatement(__VA_ARGS__)
#else
#define LogAtlasAnomalies(...)
#endif

// Log details of which image is where in atlas
#ifdef LOG_ATLAS_PLACEMENT
#define LogAtlasPlacement(...) LogStatement(__VA_ARGS__)
#else
#define LogAtlasPlacement(...)
#endif

// Log when a material is selected.
#ifdef LOG_MATERIAL_SELECTIONS
#define LogMaterialSelections(...) LogStatement(__VA_ARGS__)
#else
#define LogMaterialSelections(...)
#endif

// Log storage and use of the path map
#ifdef LOG_PATH_MAP
#define LogPathMap(...) LogStatement(__VA_ARGS__)
#else
#define LogPathMap(...)
#endif


// =======================================================================================
// Logging options for Trees/Plants

// Log when a tree is selected.
#ifdef LOG_TREE_SELECTIONS
#define LogTreeSelections(...) LogStatement(__VA_ARGS__)
#else
#define LogTreeSelections(...)
#endif

// Log when a tree is read in from a file or url
#ifdef LOG_TREE_READS
#define LogTreeReads(...) LogStatement(__VA_ARGS__)
#else
#define LogTreeReads(...)
#endif

// Log high level actions in simulating tree growth.
#ifdef LOG_TREE_SIM_OVERVIEW
#define LogTreeSimOverview(...) LogStatement(__VA_ARGS__)
#else
#define LogTreeSimOverview(...)
#endif

// Log all the gory details of simulated tree growth.
#ifdef LOG_TREE_SIM_DETAILS
#define LogTreeSimDetails(...) LogStatement(__VA_ARGS__)
#else
#define LogTreeSimDetails(...)
#endif

// Log trees being rendered.
#ifdef LOG_TREE_VISUALIZATION
#define LogTreeVisualization(...) LogStatement(__VA_ARGS__)
#else
#define LogTreeVisualization(...)
#endif

// Log clear errors in tree related operations.
#ifdef LOG_TREE_ERRORS
#define LogTreeErrors(...) LogStatement(__VA_ARGS__)
#else
#define LogTreeErrors(...)
#endif


// =======================================================================================
// Logging options for the HTTP debug interface

// Log validity problems in the HTTP request
#ifdef LOG_REQUEST_ERRORS
#define LogRequestErrors(...) LogStatement(__VA_ARGS__)
#else
#define LogRequestErrors(...)
#endif

// Log problems encountered building the response
#ifdef LOG_RESPONSE_ERRORS
#define LogResponseErrors(...) LogStatement(__VA_ARGS__)
#else
#define LogResponseErrors(...)
#endif

// Log normal details of HTTP operations
#ifdef LOG_HTTP_DETAILS
#define LogHTTPDetails(...) LogStatement(__VA_ARGS__)
#else
#define LogHTTPDetails(...)
#endif


// =======================================================================================
// Logging options to do with quadtree insertions

// Log the process of a new object being put in the quadtree.
#ifdef LOG_QUADTREE_INSERTIONS
#define LogQuadtreeInsertions(...) LogStatement(__VA_ARGS__)
#else
#define LogQuadtreeInsertions(...)
#endif

// Log the process of a new object being put in the quadtree.
#ifdef LOG_DISPLAYLIST_BUFFER
#define LogDisplayListBuffer(...) LogStatement(__VA_ARGS__)
#else
#define LogDisplayListBuffer(...)
#endif


// =======================================================================================
// Logging options to do with the Bezier Patch code

#ifdef LOG_BEZIER_FIT
#define LogBezierFit(...) LogStatement(__VA_ARGS__)
#else
#define LogBezierFit(...)
#endif

#ifdef LOG_BEZIER_MATCH_RAY
#define LogBezierMatchRay(...) LogStatement(__VA_ARGS__)
#else
#define LogBezierMatchRay(...)
#endif


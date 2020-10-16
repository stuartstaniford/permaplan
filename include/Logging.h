// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// File that controls and implements the logging system.  A variety of options
// are defined at the beginning that implement conditional compilation of all the
// different kinds of logging.

// =======================================================================================
// Logging options to do with overall control flow and speed

#define LOG_FRAME_STARTS  // Log each frame as it begins
#define LOG_CLOSE_DOWN    // Log when we exit.


// =======================================================================================
// Logging options to do with interface controls

#define LOG_MOUSE_LOCATION    // where the mouse is on the screen each frame
#define LOG_MOUSE_CLICK       // Location of single mouse clicks
#define LOG_DOUBLE_CLICK      // Location and intra-click timing of mouse double clicks
#define LOG_MOUSE_RAY_POINT   // Where the mouse is pointing in 3d space
#define LOG_LAND_HEIGHTS      // Log when a new land height is entered.
#define LOG_OBJECT_INSERTIONS // Log when a new object is inserted in scene.


// =======================================================================================
// Logging options to do with the Bezier Patch code

#define LOG_BEZIER_FIT  // Log the process of fitting the Bezier patch to height data
#define LOG_BEZIER_MATCH_RAY //Log the process of matching a ray to the patch.


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
// Logging options to do with overall control flow and speed

// Log each frame as it begins
#ifdef LOG_FRAME_STARTS
#define LogFrameStarts(...) LogStatement(__VA_ARGS__)
#else
#define LogFrameStarts(...)
#endif

// Log when we exit
#ifdef LOG_CLOSE_DOWN
#define LogCloseDown(...) LogStatement(__VA_ARGS__)
#else
#define LogCloseDown(...)
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

// Log when a new land height is entered.
#ifdef LOG_OBJECT_INSERTIONS
#define LogObjectInsertions(...) LogStatement(__VA_ARGS__)
#else
#define LogObjectInsertions(...)
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


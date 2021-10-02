// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -

#ifndef LANDSURFACE_H
#define LANDSURFACE_H

#include <cglm/cglm.h>
#include <vector>
#include "rapidjson/document.h"
#include "BezierPatch.h"  // required for VISUALIZE_FITTING


// =======================================================================================
// Forward declarations

class TexturedRect;
class Quadtree;
class HeightMarker;
class Camera;
class Scene;
class TriangleBuffer;
class HttpDebug;


// =======================================================================================
/// @brief Encapsulates the model of the entire surface of the land.
/// 
/// This class loads a series of records of surface height at various locations
/// and produces a tiling of the surface, as well as being able to texture it
/// with a provided map.

class LandSurface
{
  friend Scene;
 public:
  
  // Instance variables - public
  TexturedRect*   rect;
  Quadtree*       qtree;
  
  // Member functions - public
  LandSurface(void);
  void bufferGeometry(Quadtree* q);
  ~LandSurface(void);
  void newLandHeight(HeightMarker* hM);
  void redoBezierLandSurface(BezierPatch* bez);
  void draw(Camera& camera);
  bool diagnosticHTML(HttpDebug* serv);
  bool nextInitialHeightLocation(vec3 location, const char*& label);
  inline unsigned  getLocationCount(void) {return locationCount;}
  void writeOLDFSection(FILE* file, char* indent);

 private:
  
  // Instance variables - private
  TriangleBuffer*       tbuf;
  unsigned              locationCount;
  std::vector<float*>   heightLocations;
  std::vector<char*>    heightLabels;
  bool                  inFitMode;
  unsigned              initialHeightCount;
  unsigned              initialHeightIndex;
  rapidjson::Value*     altitudeArray;
  float                 rectOffset[2];
#ifdef VISUALIZE_FITTING
  TriangleBuffer*       fitTBuf;
#endif

  //Texture               texture;
  
  // Member functions - private
  void highlightNode(Quadtree* targetNode, vec4& color, float accent);
  LandSurface(const LandSurface&);                 // Prevent copy-construction
  LandSurface& operator=(const LandSurface&);      // Prevent assignment
};


// =======================================================================================

#endif




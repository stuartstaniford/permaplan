// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -

#ifndef HEIGHT_MARKER_H
#define HEIGHT_MARKER_H

#include "VisualObject.h"
#include "Positionable.h"


// =======================================================================================
/// @brief Mark where heights have been supplied.
/// 
/// Class for storing and rendering the user supplied locations of height (altitude)
/// measurements.  Renders them as an octahedron balanced on the actual height location.
/// Note that Height markers are currently immutable once created, and cannot be rotated
/// (that is they are Positionable, but not Orientable).


class HeightMarker: public VisualObject, public Positionable
{
 public:
  
  // Instance variables - public

  // Member functions - public
  HeightMarker(float x, float y, float z);
  HeightMarker(vec3 loc);
  ~HeightMarker(void);
  bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  bool        bufferGeometryOfObject(TriangleBuffer* T);
  void        updateBoundingBox(void);
  void        selfValidate(unsigned l);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  const char* objectName(void);
  bool        diagnosticHTMLSummary(HttpDebug* serv);
  bool        diagnosticHTML(HttpDebug* serv);

 private:
  int    index;
  
  // Instance variables - private
  
  // Member functions - private
  HeightMarker(const HeightMarker&);                 // Prevent copy-construction
  HeightMarker& operator=(const HeightMarker&);      // Prevent assignment

};


// =======================================================================================

#endif





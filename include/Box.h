// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class for storing and rendering rectangular boxes (eg straw bales, durisol blocks,
// timber beams, etc).

#ifndef BOX_H
#define BOX_H

#include "VisualObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Class variable initialization


class Box: public VisualObject
{
 public:
  
  // Instance variables - public
  mat4 trans;

  // Member functions - public
  Box(mat4 transform);   // We take the unit box on [0,0,0] to [1,1,1] and apply transform to it.
  ~Box(void);
  bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  int         getNextIndex(bool resetToFirst);
  bool        bufferGeometry(TriangleBuffer* T);
  //bool        matchRay(vec3& position, vec3& direction, float& lambda);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void        selfValidate(unsigned l);
  const char* objectName(void);
  bool        diagnosticHTMLSummary(HttpDebug* serv);

  // Static methods
  static bool validateOLDF(rapidjson::Value& boxJsonObject);
  
 private:
  
  // Instance variables - private
  char iterI, iterJ, iterK;
  int  index;
  
  // Member functions - private
  Box(const Box&);                 // Prevent copy-construction
  Box& operator=(const Box&);      // Prevent assignment
};

#endif





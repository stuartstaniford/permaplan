// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef GABLE_H
#define GABLE_H
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include "VisualObject.h"

// =======================================================================================
// Needed forward declarations

class MenuGablePanel;


// =======================================================================================
/// @brief For rendering gables.
/// 
/// This class is for use in rendering parts of buildings - specifically it renders a
/// rectangular box with a gable roof above it.  Multiple of these can be superimposed
/// to create approximations of many buildings.  Support for texturing is provided.
/// A gable is implemented as two sheds

class Gable: public VisualObject
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Gable(MenuGablePanel& gablePanel);
  ~Gable(void);
  
  // Public methods implementing the VisualObject interface
  bool        bufferGeometryOfObject(TriangleBuffer* T);
  bool        matchRayToObject(vec3& position, vec3& direction, float& lambda);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void        selfValidate(unsigned l);
  const char* objectName(void);
  bool        diagnosticHTMLSummary(HttpDebug* serv);
  
  // Static methods
  static bool validateOLDF(rapidjson::Value& gableJsonObject);

private:
  
  // Instance variables - private
  
  // Member functions - private
  Gable(const Gable&);                 // Prevent copy-construction
  Gable& operator=(const Gable&);      // Prevent assignment

};

#endif





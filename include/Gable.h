// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef GABLE_H
#define GABLE_H
#include "VisualObject.h"
#include "BuildingRect.h"
#include "Orientable.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Needed forward declarations

class MenuGablePanel;


// =======================================================================================
/// @brief The minimal date required to specify a gable.  Gable, and MenuGablePanel,
/// inherit from this.

class GableParamData: public Orientable
{
  public:
    GableParamData(void);
  
  protected:
  
  // Instance variables - private
  float height;
  float length;
  float width;
  float roofAngle;
  float overhang;  
};


// =======================================================================================
/// @brief For rendering gables.
/// 
/// This class is for use in rendering parts of buildings - specifically it renders a
/// rectangular box with a gable roof above it.  Multiple of these can be superimposed
/// to create approximations of many buildings.  Support for texturing is provided.
/// A gable is implemented as a set of BuildingRects.  The naming convention internally
/// to this class is that the building is sitting at the origin with it's floor in the
/// x-y plane, and it's roof ridge parallel to the y-axis (that is, running north-south).

class Gable: public VisualObject, public GableParamData
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Gable(MenuGablePanel& gablePanel);
  ~Gable(void);
  
  // Public methods implementing the VisualObject interface
  bool        bufferGeometryOfObject(TriangleBuffer* T);
  bool        matchRayToObject(vec3& pos, vec3& dir, float& lambda);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void        updateBoundingBox(void);
  void        selfValidate(unsigned l);
  const char* objectName(void);
  bool        diagnosticHTMLSummary(HttpDebug* serv);
  
  // Static methods
  static bool validateOLDF(rapidjson::Value& gableJsonObject);

private:
  
  // Instance variables - private.
  BuildingRect westWall;
  BuildingRect eastWall;
  BuildingRect northWall;
  BuildingRect southWall;
  BuildingRect westRoof;
  BuildingRect eastRoof;
  
  // Member functions - private
  Gable(const Gable&);                 // Prevent copy-construction
  Gable& operator=(const Gable&);      // Prevent assignment

};

#endif





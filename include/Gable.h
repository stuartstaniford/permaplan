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
class Gable;

// =======================================================================================
/// @brief The minimal date required to specify a gable.  Gable, and MenuGablePanel,
/// inherit from this.

class GableParamData: public Orientable
{
  friend MenuGablePanel;
  friend Gable;
  
  public:
    GableParamData(void);
    bool httPrintGableParamTable(HttpDebug* serv);

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
/// to this class is that the building is sitting at the vector value of position.  When
/// angleFromNorth is zero, the position refers to the southwest corner of the building,
/// with it's floor in the x-y plane, and it's roof ridge parallel to the y-axis (that 
/// is, running north-south).  The length of the walls in this direction is "length", 
/// while the length in the east-west drection is "width".  "Height" is the height of
/// the short walls, not the full height to the roof ridge.

class Gable: public VisualObject, public GableParamData
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Gable(MenuGablePanel& gablePanel);
  ~Gable(void);
  void        rebuildRects(void);
  
  // Public methods implementing the VisualObject interface
  bool        bufferGeometryOfObject(TriangleBuffer* T);
  bool        matchRayToObject(vec3& pos, vec3& dir, float& lambda);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void        updateBoundingBox(void);
 void         selfValidate(unsigned l);
  const char* objectName(void);
  bool        diagnosticHTMLSummary(HttpDebug* serv);
  bool        diagnosticHTML(HttpDebug* serv);

  // Static methods
  static bool validateOLDF(rapidjson::Value& gableJsonObject);

private:
  
  // Instance variables - private.
  BuildingRect  westWall;
  BuildingRect  eastWall;
  BuildingRect  northWall;
  BuildingRect  southWall;
  BuildingRect  westRoof;
  BuildingRect  eastRoof;
  BuildingRect  westSoffit;
  BuildingRect  eastSoffit;
  BuildingRect  northSoffit;
  BuildingRect  southSoffit;
  float         roofRise;
  
  // Member functions - private
  bool addEndTrianglesToBuffer(TriangleBuffer* T);

  /// @brief Prevent copy-construction.
  Gable(const Gable&);        
  /// @brief Prevent assignment.
  Gable& operator=(const Gable&);      

};

#endif





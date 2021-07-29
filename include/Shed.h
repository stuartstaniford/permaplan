// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef SHED_H
#define SHED_H

#include "VisualObject.h"
#include "BuildingRect.h"
#include "Orientable.h"
#include <cglm/cglm.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Needed forward declarations

class MenuShedPanel;


// =======================================================================================
/// @brief The minimal date required to specify a shed.  Shed, and MenuShedPanel,
/// inherit from this.

class ShedParamData: public Orientable
{
  public:
    ShedParamData(void);
    bool httPrintShedParamTable(HttpDebug* serv);

  protected:
  
  // Instance variables - protected
  float height;
  float length;
  float width;
  float roofAngle;
  float rightOverhang;
  float leftOverhang;
  float frontOverhang;
  float backOverhang;
  bool  westWallPresent;
  bool  eastWallPresent;
};


// =======================================================================================
/// @brief For rendering sheds.
/// 
/// This class is for rendering sheds - either standalone buildings, or (with an open
/// side) parts of larger buildings.  Note that the convention here in the default 
/// orientation (angleFromNorth==0) is that the tall side of the shed is to the west and
/// the short side is to the east.  Height is the height of the tall west wall.  Length
/// is the length of that wall, while width is the length of the walls that run 
/// east-west.


class Shed: public VisualObject, public ShedParamData
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Shed(MenuShedPanel& shedPanel);
  ~Shed(void);
  void rebuildRects(void);

  // Public methods implementing the VisualObject interface
  void triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void updateBoundingBox(void);
  bool bufferGeometryOfObject(TriangleBuffer* T);
  bool matchRayToObject(vec3& pos, vec3& dir, float& lambda);
  const char* objectName(void);
  bool diagnosticHTML(HttpDebug* serv);
  
  // Static methods
  static bool validateOLDF(rapidjson::Value& shedJsonObject);
  
private:
  
  // Instance variables - private
  BuildingRect westWall;
  BuildingRect eastWall;
  BuildingRect northWall;
  BuildingRect southWall;
  BuildingRect roof;
 

  // Member functions - private
  Shed(const Shed&);                 // Prevent copy-construction
  Shed& operator=(const Shed&);      // Prevent assignment

};

#endif





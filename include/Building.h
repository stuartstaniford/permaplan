// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef BUILDING_H
#define BUILDING_H

#include "ObjectGroup.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
// Needed forward declarations

class JSONStructureChecker;
class Quadtree;
class Scene;


// =======================================================================================
/// @brief Combine multiple building components into a composite entity representing the 
/// whole building.
///
/// This class exists to allow us to track the relationship of a series of gables, sheds,
/// etc that are managed as a single things (unless disaggregated to manage the
/// relationship of the individual pieces.

class Building: public ObjectGroup
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Building(VisualObject* firstComponent);
  ~Building(void);

  // static public functions  
  static bool validateOneBuilding(rapidjson::Value& buildingJson, int i, 
                                                              JSONStructureChecker* jCheck);
  static bool validateBuildings(rapidjson::Value& buildings, JSONStructureChecker* jCheck);
  static void writeBuildings(FILE* writeFile, char* indent);
  static void readBuildingsFromDesign(Quadtree* qtree, Scene& scene);
  static bool buildingsPresent; // allow us to remember an empty list in OLDF file

private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  Building(const Building&);       
  /// @brief Prevent assignment.
  Building& operator=(const Building&);      
};

#endif





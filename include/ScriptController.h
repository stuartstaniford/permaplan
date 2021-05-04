// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// Class for reading a set of scripted UI actions from the JSON file, and then
// spoon feeding them to the Window3D class as it runs, to simulate some particular
// sequence of user actions.  Useful for creating test cases, canned demos, and the
// like.


#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H

#include <unordered_map>
#include <string>
#include "HttpDebug.h"
#include "PmodDesign.h"

// =======================================================================================
// Enum for interface actions to be simulated

enum InterfaceActionType
{
  IA_None,
  IA_HeightMarker,
  IA_Unknown,
};


// =======================================================================================
// Class variable initialization

class ScriptController
{
 public:
  
  // Instance variables - public
  
  // Member functions - public
  ScriptController(void);
  ~ScriptController(void);
  void      processNewScriptControl(void);
  void      setupMaps(void);
  void      processInterfaceAction(rapidjson::Value& V);
  void      processCameraMovement(const char* type);
  unsigned  simulatedKeys(float delta);
  bool      checkInterfaceAction(InterfaceActionType I);
  bool      diagnosticHTML(HttpDebug* serv);
  void      setHeight(rapidjson::Value& V);
  
  inline float getHeight(void) {return args[0];}

 private:
  
  // Instance variables - private
  double                                                timeSec;
  double                                                timeLimit;
  unsigned                                              index;
  rapidjson::Value                                      scriptArray;
  rapidjson::Value                                      nextObject;
  std::unordered_map<std::string, unsigned>             camActionMap;
  std::unordered_map<std::string, InterfaceActionType>  interfaceActionMap;
  unsigned                                              currentCamAction;
  InterfaceActionType                                   currentInterfaceAction;
  float                                                 args[16];
  
  // Member functions - private
  ScriptController(const ScriptController&);                 // Prevent copy-construction
  ScriptController& operator=(const ScriptController&);      // Prevent assignment

};

#endif




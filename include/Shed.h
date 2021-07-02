// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef SHED_H
#define SHED_H

#include "VisualObject.h"
#include <cglm/cglm.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
// Needed forward declarations

class MenuShedPanel;


// =======================================================================================
/// @brief For rendering sheds.
/// 
/// This class is for rendering sheds - either standalone buildings, or (with an open
/// side) parts of larger buildings.) 


class Shed: public VisualObject
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Shed(MenuShedPanel& shedPanel);
  ~Shed(void);

  // Static methods
  static bool validateOLDF(rapidjson::Value& shedJsonObject);
  
private:
  
  // Instance variables - private
  vec3 pos;       // the direction of the tall wall of the shed
  vec3 longAxis; 
  vec3 shortAxis; // the direction of the short walls
  vec3 vertAxis;   // the direction of the corner where longAxis and shortAxis walls meet 
  vec3 roofVec;   // Vector from the top of the tall wall down the roof along the short wall 
  vec4 overhangs; // tall, long wall; low long wall; short walls (the same)
  

  // Member functions - private
  Shed(const Shed&);                 // Prevent copy-construction
  Shed& operator=(const Shed&);      // Prevent assignment

};

#endif





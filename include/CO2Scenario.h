// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class for pulling a CO2 scenario out of the JSON file and providing it to the
// rest of the code as needed.

#ifndef CO2_SCENARIO_H
#define CO2_SCENARIO_H

#include <map>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Class variable initialization


class CO2Scenario: public std::map<float, float>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  CO2Scenario(char* scenarioName);
  float getConcentration(float year);
  ~CO2Scenario(void);
  
private:
  
  // Instance variables - private
  
  // static variables associated with the file
  static rapidjson::Document doc;
  static bool               jsonLoaded;
  static char*              fileName;
  
  // Member functions - private
  void loadScenarioFile(void);
  CO2Scenario(const CO2Scenario&);                 // Prevent copy-construction
  CO2Scenario& operator=(const CO2Scenario&);      // Prevent assignment

};

#endif





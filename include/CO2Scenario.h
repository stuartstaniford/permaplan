// Copyright Staniford Systems.  All Rights Reserved.  March 2021 -

#ifndef CO2_SCENARIO_H
#define CO2_SCENARIO_H

#include <map>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
/// @brief Class for pulling a CO2 scenario out of a JSON file and providing it to the
/// rest of the code as needed.
///
/// This is used since over the life of trees in the simulation, the CO2 concentration
/// will vary dramatically, and this is expected to have significant impacts on plant
/// growth and physiology (eg water use).  The scenarios are presently drawn from a 
/// file with the hard-coded name co2-scenarios.json


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
  /// @brief Prevent copy-construction
  CO2Scenario(const CO2Scenario&);        
  /// @brief Prevent assignment
  CO2Scenario& operator=(const CO2Scenario&);      

};

#endif





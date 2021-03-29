// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class for pulling a CO2 scenario out of the JSON file and providing it to the
// rest of the code as needed.

#ifndef CO2_SCENARIO_H
#define CO2_SCENARIO_H

// =======================================================================================
// Class variable initialization


class CO2Scenario
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  CO2Scenario(void);
  ~CO2Scenario(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  CO2Scenario(const CO2Scenario&);                 // Prevent copy-construction
  CO2Scenario& operator=(const CO2Scenario&);      // Prevent assignment

};

#endif





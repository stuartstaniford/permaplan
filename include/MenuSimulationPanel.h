// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef MENU_SIMULATION_PANEL_H
#define MENU_SIMULATION_PANEL_H

#include "MenuPanel.h"
#include "Shed.h"


// =======================================================================================
/// @brief The panel (usually in top right) for monitoring and control of the simulation.
/// 
/// This class provides for the main panel that is used to stop, start, and pause the
/// simulation, and also displays basic status of it (like the year and the CO2
/// concentration. 

class MenuSimulationPanel: public MenuPanel
{  
  friend MenuInterface;
  friend Shed;

public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuSimulationPanel(MenuInterface* menu);
  virtual ~MenuSimulationPanel(void);
  void    display(void);
  char*   errorInFields(void);

private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuSimulationPanel(const MenuSimulationPanel&);            // Prevent copy-construction
  MenuSimulationPanel& operator=(const MenuSimulationPanel&); // Prevent assignment

};

#endif





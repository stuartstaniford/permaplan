// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_SIMULATION_PANEL_H
#define MENU_SIMULATION_PANEL_H

#include "MenuPanel.h"
#include "Shed.h"
#include "Scene.h"


// =======================================================================================
/// @brief The panel (usually in top right) for monitoring and control of the simulation.
/// 
/// This class provides for the main panel that is used to stop, start, and pause the
/// simulation, and also displays basic status of it (like the year and the CO2
/// concentration. It's a transparent overlay in the top right corner.

class MenuSimulationPanel: public MenuPanel
{  
  friend MenuInterface;
  friend Shed;

public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuSimulationPanel(MenuInterface* menu, Scene* S);
  virtual ~MenuSimulationPanel(void);
  
  // Our abstract interface operations
  void simulatePause(void);
  void simulateReset(void);
  void simulateStart(void);
  void simulateSpring(void);
  void simulateSummer(void);
  void simulateFall(void);
  void simulateWinter(void);
  
  // API coming from MenuPanel and overridden here.
  void        display(void);
  char*       errorInFields(void);
  bool        handleHTTPRequest(HttpDebug* serv, char* path);
  ActionType  processAction(InterfaceAction* action);

private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuSimulationPanel(const MenuSimulationPanel&);            // Prevent copy-construction
  MenuSimulationPanel& operator=(const MenuSimulationPanel&); // Prevent assignment

};

#endif


// =======================================================================================



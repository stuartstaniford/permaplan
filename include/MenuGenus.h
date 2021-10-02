// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_GENUS_H
#define MENU_GENUS_H

#include "MenuPanel.h"


// =======================================================================================
// Forward declarations

class Species;


// =======================================================================================
/// @brief Obtain details of tree to insert once we know the genus.
/// 
/// This class implements the floating menu to select a particular species to insert 
/// after a genus has been selected in imguiTreeMenu.  Also gets the tree age at 
/// planting time.
/// @todo Scriptability via HTTP is not implemented.

class MenuGenus: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuGenus(MenuInterface* menu, Scene* S, const char* G);
  virtual ~MenuGenus(void);
  
  // Our abstract interface operations
  void speciesSelected(Species* S);
  
  // API coming from MenuPanel and overridden here.
  void        imGuiDisplay(void);

private:
  
  // Instance variables - private
  const char*   genusSelected;
  char          ageBuf[16];

  // Member functions - private
  MenuGenus(const MenuGenus&);                 // Prevent copy-construction
  MenuGenus& operator=(const MenuGenus&);      // Prevent assignment
};


// =======================================================================================

#endif





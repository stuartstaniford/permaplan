// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_HEIGHT_H
#define MENU_HEIGHT_H


// =======================================================================================
/// @brief Accept a new height estimate at a particular location.
///
/// This panel appears on clicking "Height" from the insert menu, but should also be
/// available from the HTTP interface, and in particular it's anticipated in future that
/// the most used entry point to this interface will be via a phone app using GPS.

class MenuHeightPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuHeightPanel(void);
  ~MenuHeightPanel(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuHeightPanel(const MenuHeightPanel&);                 // Prevent copy-construction
  MenuHeightPanel& operator=(const MenuHeightPanel&);      // Prevent assignment

};

#endif





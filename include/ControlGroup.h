// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a subclass of ObjectGroup specifically to handle the visual controls
// that appear around an object after it has been double-clicked.

#ifndef CONTROL_GROUP_H
#define CONTROL_GROUP_H

// =======================================================================================
// Class variable initialization

class ControlGroup
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ControlGroup(void);
  ~ControlGroup(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  ControlGroup(const ControlGroup&);                 // Prevent copy-construction
  ControlGroup& operator=(const ControlGroup&);      // Prevent assignment

};

#endif





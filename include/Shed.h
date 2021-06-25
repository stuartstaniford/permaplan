// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for rendering sheds - either standalone buildings, or (with an open
// side) parts of larger buildings.

#ifndef SHED_H
#define SHED_H

// =======================================================================================
// Class variable initialization

class Shed
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Shed(void);
  ~Shed(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  Shed(const Shed&);                 // Prevent copy-construction
  Shed& operator=(const Shed&);      // Prevent assignment

};

#endif





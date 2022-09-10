// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -

#ifndef GROUND_LITTER_LAYER_H
#define GROUND_LITTER_LAYER_H

#include "GroundLayer.h"


// =======================================================================================
/// @brief Class for modeling layers of organic material on the ground.
///
/// This is a subclass of GroundLayer, and it can be used for modeling organic mulches/
/// litters on the ground.  It's mainly focussed on managing the movement of carbon (eg
/// from plants above, and to the topsoil below).  Subclasses can be used to provide
/// visual representation of particular types of litter (eg leaf mold in the forest, 
/// dead grass in a meadow).

class GroundLitterLayer: public GroundLayer
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  GroundLitterLayer(void);
  ~GroundLitterLayer(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  GroundLitterLayer(const GroundLitterLayer&);       
  /// @brief Prevent assignment.
  GroundLitterLayer& operator=(const GroundLitterLayer&);      
};


// =======================================================================================

#endif





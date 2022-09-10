// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Class for modeling layers of organic material on the ground.  This is a 
// subclass of GroundLayer, and it can be used for modeling organic mulches/litters 
// on the ground.  It's mainly focussed on managing the movement of carbon (eg 
// from plants above, and to the topsoil below).  Subclasses can be used to 
// provide visual representation of particular types of litter (eg leaf mold in 
// the forest, dead grass in a meadow).

#include "GroundLitterLayer.h"


// =======================================================================================
/// @brief Constructor

GroundLitterLayer::GroundLitterLayer(void): GroundLayer((char*)"GroundLitter")
{
}


// =======================================================================================
/// @brief Destructor

GroundLitterLayer::~GroundLitterLayer(void)
{
}


// =======================================================================================

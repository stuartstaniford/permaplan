// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -

#ifndef VISUAL_LAYER_H
#define VISUAL_LAYER_H


// =======================================================================================
/// @brief This class is for modeling information that supports the visual layer system
/// (allowing certain things to be turned on and off in the renderings).
///
/// More details of the class go here.

class VisualLayer
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  VisualLayer(void);
  ~VisualLayer(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  VisualLayer(const VisualLayer&);       
  /// @brief Prevent assignment.
  VisualLayer& operator=(const VisualLayer&);      

};

#endif





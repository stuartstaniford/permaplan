// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -

#ifndef VISUAL_LAYER_H
#define VISUAL_LAYER_H


// =======================================================================================
/// @brief This class is for modeling information that supports the visual layer system.
///
/// The purpose of the layer system is to allow visual objects to be classified as to the
/// kind of thing they are, and then to be turned on and off in the renderings based on
/// those classes.  A few prepopulated layers will be supplied, but the intent is to 
/// allow the user to also create custom layers.  Layers are implemnted as a bitmap, and
/// thus are currently restricted to 32 total layers.

class VisualLayer
{
public:
  
  // Instance variables - public
  unsigned layerFlags;
  
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





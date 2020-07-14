// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class just draws X, Y, and Z axes in red, green, and blue fixed colors


#ifndef COLOREDAXES_H
#define COLOREDAXES_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include <cglm/cglm.h>

// =======================================================================================
// Class variable initialization


class ColoredAxes
{
 public:
  
  // Instance variables - public
  Shader&   shader;
  
  // Member functions - public
  ColoredAxes(Shader& S, float axesLen);
  ~ColoredAxes(void);
  void draw(void);

 private:
  
  // Instance variables - private
  VertexArrayObject     axesVAOs;
  float                 axesLength;
  VertexBufferObject*   VBO[3];
  
  // Member functions - private
  

};

#endif




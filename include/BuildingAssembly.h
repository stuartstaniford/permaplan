// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef EMPTY_CLASS_H
#define EMPTY_CLASS_H

#include "VisualObject.h"
#include "Orientable.h"

// =======================================================================================
// Required forward declarations

class BuildingRect;


// =======================================================================================
/// @brief Functionality common to building sub-assemblies such as gables, sheds, etc.
///
/// At the moment, largely has functionality for managing an array of BuildingRects.

class BuildingAssembly: public VisualObject, public Orientable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BuildingAssembly(int N);
  ~BuildingAssembly(void);
  virtual bool bufferGeometryOfObject(TriangleBuffer* T);
  virtual void updateBoundingBox(void);

protected:

  // Instance variables - protected
  int nRects;
  BuildingRect* rects;
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  BuildingAssembly(const BuildingAssembly&);       
  /// @brief Prevent assignment.
  BuildingAssembly& operator=(const BuildingAssembly&);      

};

#endif





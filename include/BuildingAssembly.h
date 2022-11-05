// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef EMPTY_CLASS_H
#define EMPTY_CLASS_H

#include "VisualObject.h"

// =======================================================================================
// Required forward declarations

class BuildingRect;
class BuildingAssembly;


// =======================================================================================
/// @brief A triangular extension to one of the BuildingRects in the class.  
/// 
/// It is currently assumed that these triangles essentially "fill in" between the 
/// rectangles (rather than, say, poking out beyond the envelope of the rectangles).  
/// Thus for example, they are ignored in updateBoundingBox.  However, they are 
/// considered in bufferGeometry (so they appear visually) and matchRay (so that their 
/// effect in shading or mouse pointing is correctly captured.  Note that the two
/// selected vertex of the existing rectangle must be ordered such that taking the first
/// of them, then the second, then the newly added extending vertex, will result in 
/// counterclockwise winding from the front face (per OpenGL culling convention).

class RectExtension
{  
  public:
  ///@brief The point of the triangle that is to be added.
  vec3 extensionPoint;
  ///@brief Which rectangle is to be extended;
  unsigned rectIndex;
  ///@brief the two vertices of the rectangle that form the base of the added triangle.
  unsigned vertexIndices[2];
};


// =======================================================================================
/// @brief Functionality common to building sub-assemblies such as gables, sheds, etc.
///
/// At the moment, largely has functionality for managing an array of BuildingRects,
/// and another of RectExtensions.
/// 

class BuildingAssembly: public VisualObject
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BuildingAssembly(int nRectangles, int nExtensions = 0);
  ~BuildingAssembly(void);
  
  // Functions implementing the VirtualObject interface
  void triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual bool bufferGeometryOfObject(TriangleBuffer* T);
  virtual void updateBoundingBox(void);
  virtual bool matchRayToObject(vec3& pos, vec3& dir, float& lambda);
  virtual void getPosition(vec3 result);
  void selfValidate(unsigned l);
  virtual const char* objectName(void);

protected:

  // Instance variables - protected
  int nRects;
  int nExts;
  BuildingRect* rects;
  RectExtension* exts;
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  bool bufferExtensions(TriangleBuffer* T);
  /// @brief Prevent copy-construction.
  BuildingAssembly(const BuildingAssembly&);       
  /// @brief Prevent assignment.
  BuildingAssembly& operator=(const BuildingAssembly&);      

};


// =======================================================================================

#endif





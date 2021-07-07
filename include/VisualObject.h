// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -


#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "VisualElement.h"
#include "Lockable.h"
#include <unordered_map>

// =======================================================================================
// Forward declarations required

class LandSurfaceRegion;
class Quadtree;
class Vertex;
class ObjectGroup;


// =======================================================================================
/// @brief Superclass for renderable whole objects.
/// 
/// This class is not used directly, but instead is a superclass for anything that
/// should be displayed and be stored in the quadtree.  A VisualObject is expected
/// to be a "whole object" in some sense (even though it may also be a part of a larger
/// group).  But it knows it's own absolute location and is capabable of standing on
/// it's own in the Quadtree etc.


class VisualObject: public Lockable, public VisualElement 
{
  friend Quadtree;
  friend ObjectGroup;
  friend Vertex;
  
 public:
  
  // Instance variables - public
  BoundingBox*  box;
  char*         label;

  // Member functions - public
  VisualObject(bool absHeights, BoundingBox* B = NULL);
  virtual ~VisualObject(void);

  // Public member functions coming from VisualElement
  inline bool         updateBoundingBox(BoundingBox* B, vec3 offset)
                    {return VisualElement::updateBoundingBox(B, offset);}
  virtual bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual int         getNextIndex(bool resetToFirst);
  virtual bool        bufferGeometryOfObject(TriangleBuffer* T);
  virtual bool        matchRayToObject(vec3& position, vec3& direction, float& lambda);
  virtual void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual float       estimateOpacity(vec3 direction);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);
  virtual bool        diagnosticHTMLSummary(HttpDebug* serv);
  static bool         diagnosticHTMLSelection(HttpDebug* serv, char* path);
  
  // Public member functions arising here
  void                setLabel(const char* inLabel);
  void                setNoTexColor(unsigned color);
  virtual void        setAltitude(LandSurfaceRegion* surface);
  virtual void        getGroundContact(float& x, float& y);
  virtual void        updateBoundingBox(void);
  void                removeFromQuadtree(void);
#ifdef LOG_TREE_VALIDATION
  virtual void        selfValidate(unsigned l);
#endif

 protected:
  
  // Instance variables - protected
  Quadtree*       qTreeNode;
  ObjectGroup*    groupOwner;
  unsigned        noTexColor;
  unsigned        objIndex;
  float           altitude;
  bool            isGroup;
  bool            useNoTexColor;
  bool            absoluteHeights;  // if true, heights are absolute.  Otherwise, they
                                  // are relative to local ground level.
  
  // static (class) protected variables
  static          Lockable staticLock;
  static          unsigned nextIndex;
  static          std::unordered_map<unsigned, VisualObject*> allObjects;
  
private:
  
  // Member functions - private
  VisualObject(const VisualObject&);                 // Prevent copy-construction
  VisualObject& operator=(const VisualObject&);      // Prevent assignment

};

#endif





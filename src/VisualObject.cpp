
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree

#include "VisualObject.h"
#include "LandSurfaceRegion.h"
#include "Quadtree.h"
#include "ObjectGroup.h"


// =======================================================================================
// Static variables

unsigned VisualObject::nextIndex = 1;
std::unordered_map<unsigned, VisualObject*> VisualObject::allObjects;
Lockable VisualObject::staticLock;

// =======================================================================================
/// @brief Constructor for the VisualObject.
/// 
/// In addition to doing default values for the object state, we also allocate it a unique
/// index and keep track of that in the allObjects container.
/// 
/// @param absHeights This is true if the VisualObject should have absolute height, false
/// if they should be relative to the land surface.
/// @param B A pointer to a bounding box.  This defaults to NULL and that is the usual 
/// case - the object will allocate it's own BoundingBox.  But if there's a reason the
/// object should use a different bounding box, it is supplied here.
///  

VisualObject::VisualObject(bool absHeights, BoundingBox* B):
                                box(B),
                                label(NULL),
                                qTreeNode(NULL),
                                groupOwner(NULL),
                                altitude(0.0f),
                                isGroup(false),
                                useNoTexColor(false),
                                absoluteHeights(absHeights)
{
  staticLock.lock();
  objIndex = nextIndex++;
  allObjects[objIndex] = this;
  staticLock.unlock();
}


// =======================================================================================
/// @brief Destructor
///
/// Deletes box (the bounding box pointer), it's label (a char array on the heap), 
/// and erases its objIndex from the allObjects container.

VisualObject::~VisualObject(void)
{
  LogObjectCreation("Object destroyed with id %u.\n", objIndex);
  if(box)
    delete box;
  if(label)
    delete[] label;
  
  staticLock.lock();
  allObjects.erase(objIndex);
  staticLock.unlock();
}


// =======================================================================================
/// @brief Sets the label for the VisualObect.
///
/// Function to set the label of this object (not normally over-ridden).  Makes a copy
/// on the heap (and will delete the last version if called more than once).  The label
/// is a way to store some kind of explanatory text or annotation specific to a particular
/// VisualObject.
/// 
/// @param inLabel - supplied label.  Will be copied and not relied on after this call.

void VisualObject::setLabel(const char* inLabel)
{
  if(label)
    delete[] label;
  label = new char[strlen(inLabel)+1];
  strcpy(label, inLabel);
}


// =======================================================================================
/// @brief Function to setAltitude of objects with relative height.
///
/// VisualObjects can have either relative height (to the landsurface where they are, or
/// absolute heights).  This function to set our altitude based on the land height where 
/// we are for the relative height case.  This version can be overwritten by implementing 
/// subclasses, but should do the right thing in most cases (relies on calling out to
/// getGroundContact().
///
/// @param surface The LandSurfaceRegion where we are.

void VisualObject::setAltitude(LandSurfaceRegion* surface)
{
  unless(absoluteHeights)
   {
    float x,y;
    getGroundContact(x,y);
    altitude = surface->getAltitude(x,y);
    LogObjectAltitudes("Object of type %s at %.2f,%.2f has altitude %.2f.\n",
                       objectName(), x, y, altitude);
   }
  else
    LogObjectAltitudes("Object of type %s has absolute altitudes.\n", objectName());
}


// =======================================================================================
/// @brief Function to return the location at which we officially contact ground/grade 
/// level.
///
/// This version in this class finds the midpoint of the underside of the bounding box for 
/// the object.  Subclasses may want to override this behavior with something specific
/// to the geometry of the particular kind of object, as this approximation may give
/// subtle errors in some cases.
///
/// @param x A reference to the x location to be filled out by the object.
/// @param y A reference to the y location to be filled out by the object.

void VisualObject::getGroundContact(float& x, float& y)
{
  unless(box)
    err(-1, "Called VisualObject::getGroundContact with no bounding box.");

  x = (box->lower[0] + box->upper[0])/2.0f;
  y = (box->lower[1] + box->upper[1])/2.0f;
}


// =======================================================================================
/// @brief Function to remove ourselves from the quadtree if necessary.
///
/// This is removal from the quadtree without deletion of the object (eg if it is about
/// to be incorporated into an object group).  It's currently believed that subclasses
/// do not need to override this.

void VisualObject::removeFromQuadtree(void)
{
 {if(qTreeNode) 
   qTreeNode->removeVisualObject(this);}
}


// =======================================================================================
// Used when this visual object is going to be a visual indicator with a fixed color.
// XX - should this be hived off into a different class, that could use multiple
// inheritance to pull in, so this isn't present in non-visual indicator subclasses?

void VisualObject::setNoTexColor(unsigned color)
{
  noTexColor    = color;
  useNoTexColor = true;
}


// =======================================================================================
/// @brief This defines the interface for any visual object to turn itself into a set of 
/// vertices/indices for rendering.
///
/// The actual definition in this class is a stub that should be overwritten by 
/// implementing subclasses.  This version will abort with an error if ever called.
/// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

bool VisualObject::bufferGeometryOfObject(TriangleBuffer* T)
{
  err(-1, "Called unimplemented superclass VisualObject::bufferGeometryOfObject.\n");
  return false;
}


// =======================================================================================
/// @brief Decide if a ray touches us.  
/// 
/// This is one of the major interface functions for the VisualObject class - to decide 
/// whether a given line touches the object or not.  It is extensively used for things
/// like identifying the object at mouse position, deciding if light hits trees from
/// particular angles, etc, etc.  The version here will work in many cases, but there may 
/// be more efficient methods possible for specific subclasses.  This will only work if 
/// the subclass has implemented getNextVertex (because it calls on 
/// VisualElement::matchRayToElement).
///
/// @param position The vec3 for a point on the ray to be matched.
/// @param direction The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.

/// @todo XX This might return a match on the far side of the element, instead of nearest.

bool VisualObject::matchRayToObject(vec3& position, vec3& direction, float& lambda)
{
  unless(box->matchRay(position, direction, lambda))
    return false;
  return VisualElement::matchRayToElement(position, direction, lambda, zeroVec);    
}


// =======================================================================================
/// @brief Estimate the opacity of an object's bounding box.
/// 
/// This interface is for a function to estimate the likilihood that a ray of light will 
/// make it through our bounding box in a particular direction.  This superclass does
/// not provide an implementation and it needs to be overridden (it will abort if called).
/// This function is used when simulating the growth of vegetation so that we can know
/// how much shading an object does.
/// 
/// @param direction A vec3 of the direction in which opacity is to be estimated.
/// @returns A float value from 0 (completely transparent) to 1.0 (completely opaque).

float VisualObject::estimateOpacity(vec3 direction)
{
  err(-1, "Called unimplemented superclass VisualObject::estimateTransparency.\n");
}


// =======================================================================================
/// @brief Compute the bounding box.  
/// 
/// Function to update the axis-aligned bounding box (which all VisualObjects have).  
/// This version will work in many cases as long as the subclass has implemented 
/// getNextUniqueVertex but there may be more efficient methods possible for specific 
/// subclasses.  Note that subclass version of this must also take on the responsibility 
/// of notifying our qTreeNode if we've changed our extent.

void VisualObject::updateBoundingBox(void)
{
  bool boxChanged = false;
  if(!box)
   {
    box = new BoundingBox();
    boxChanged = true;
   }
  else
    box->hugeValify();

  VisualElement::updateBoundingBox(box, zeroVec);
  if(boxChanged && qTreeNode)
    qTreeNode->notifyObjectBoxChange(this);
  return;
}


// =======================================================================================
/// @brief An interface by which any VisualObject should return a pointer to its name.
///
/// Should be overriden - this one will abort.

const char* VisualObject::objectName(void)
{
  err(-1, "Called unimplemented superclass VisualObject::objectName.\n");
}


// =======================================================================================
/// @brief Interface for a function to provide a whole page with details about the 
/// specific object.
/// 
/// This should be overwritten by implementing subclasses as this version will abort with
/// and error if ever called.
/// @returns True if the object was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool VisualObject::diagnosticHTML(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualObject::diagnosticHTML.\n");
}


// =======================================================================================
/// @brief Interface to a function which should provide a single row in an HTML table.
///  
/// The type of visual object in the first column, and any details provided in the second 
/// column.  A link should be provided to whatever URL serves diagnosticHTML.  The 
/// implementation in this class should be overwritten by implementing subclasses as it 
/// will abort with an error if ever called.

bool VisualObject::diagnosticHTMLSummary(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualObject::diagnosticHTMLSummary.\n");
}


// =======================================================================================
/// @brief Provides access to the diagnostic HTTP server for the diagnosticHTML page of 
/// any object via it's objIndex.
///
/// @returns True if the object was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server
/// @param path The path variable, which should just contain an object index.  If it is
/// something else, atoi will tend to turn it into zero and that object will be returned.


bool VisualObject::diagnosticHTMLSelection(HttpDebug* serv, char* path)
{
  unsigned index = atoi(path);
  staticLock.lock();
  if(allObjects.count(index))
   {
    VisualObject* V = allObjects[index];
    staticLock.unlock();
    return V->diagnosticHTML(serv);
   }
  else
   {
    staticLock.unlock();
    LogRequestErrors("Request for unknown visual object index: %s\n", path);
    return false;
   }
}


// =======================================================================================

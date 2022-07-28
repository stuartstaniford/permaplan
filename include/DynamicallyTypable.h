// Copyright Staniford Systems.  All Rights Reserved.  June 2021 -

#ifndef DYNAMICALLY_TYPABLE_H
#define DYNAMICALLY_TYPABLE_H


// =======================================================================================
/// @brief Enum representing the types that can inherit from DynamicallyTypable and 
/// be represented.
/// 
/// Note that any additions to this list must also be reflected in the
/// dynamicallyTypableName array in DynamicallyTypable.cpp

enum DynamicType
{
  TypeNull,                 // undefined pointers
  TypeDynamicallyTypable,   // bare instance of this class
  TypeBezierPatch,
  TypeLandSurfaceRegionPlanar,
  TypeRegionList,
  TypeSpecies,
  TypeSpeciesPath,
  TypeTree,
  TypeVisualElement,
  TypeVisualObject,
  TypeHWSDProfile
};


// =======================================================================================
/// @brief This class is for situations where we need to have pointers to a range of
/// different things, and resolve at run time which kind they are.  
/// 
/// This class provides means to do that.  The class should be subclassed for any new 
/// dynamic types, and a new case added to the enum DynamicType.  The method of most
/// consequence is getDynamicType, which should be overridden to return the 
/// class-specific value.  There's also a writeJson method which can be used to provide
/// json objects which a receiver can parse based on the dynamic type.


class DynamicallyTypable
{
public:
  
  // Instance variables - public

  // Member functions - public
  inline DynamicallyTypable(void) {};
  inline virtual ~DynamicallyTypable(void) {};
  
  /// @brief Interface for methods to return their dynamic type 
  /// @returns the DynamicType of this particular object.
  inline virtual DynamicType getDynamicType(void) {return TypeDynamicallyTypable;}
  virtual int writeJson(char* buf, unsigned bufSize);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  DynamicallyTypable(const DynamicallyTypable&);                 // Prevent copy-construction
  DynamicallyTypable& operator=(const DynamicallyTypable&);      // Prevent assignment

};


// =======================================================================================

#endif

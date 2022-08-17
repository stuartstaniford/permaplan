// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for situations where we need to have pointers to a range of
// different things, and resolve at run time which kind they are.  This class
// provides means to do that.

#include "DynamicallyTypable.h"
#include <stdio.h>


// =======================================================================================
// Static variables, etc.

/// Note that this must exactly match the enum DynamicType in DynamicallyTypable.h

char* dynamicallyTypableName[] = {
                            (char*)"TypeNull", 
                            (char*)"TypeDynamicallyTypable",
                            (char*)"TypeBezierPatch",
                            (char*)"TypeLandSurfaceRegionPlanar",
                            (char*)"TypeRegionList",
                            (char*)"TypeSpecies",
                            (char*)"TypeSpeciesPath",
                            (char*)"TypeTree",
                            (char*)"TypeVisualElement",
                            (char*)"TypeVisualObject",
                            (char*)"TypeSoilProfile",
                            (char*)"TypeHWSDProfile",
                            (char*)"TypeGroundLayer",
                            (char*)"TypeSoilHorizon"
};


// =======================================================================================
/// @brief Constructor.

DynamicallyTypable::DynamicallyTypable(void)
{
}


// =======================================================================================
/// @brief Destructor.

DynamicallyTypable::~DynamicallyTypable(void)
{
}


// =======================================================================================
/// @brief Interface for methods to return their dynamic type
/// @returns the DynamicType of this particular object.

DynamicType DynamicallyTypable::getDynamicType(void)
{
  return TypeDynamicallyTypable;
}


/// =======================================================================================
/// @brief Provide json objects which a receiver can parse based on the dynamic type.
/// @returns The number of bytes written.
/// @param buf The place in memory to output the JSON text to.
/// @param bufSize The size of the buffer that can safely be written to.

int DynamicallyTypable::writeJson(char* buf, unsigned bufSize)
{
  char* end = buf + bufSize;
  bufprintf("{\n");
  buf += writeJsonFields(buf, end-buf);
  bufprintf("\n}\n");
  return (bufSize - (int)(end-buf));
}


// =======================================================================================
/// @brief Provide json objects which a receiver can parse based on the dynamic type.
/// 
/// The convention is that each type of object first calls it's immediate parent
/// to get the parent (and grandparent etc) fields.  Then writes it's own fields.  So in
/// the end we get here which writes the type at the beginning (so objects in receipt
/// of this JSON object can decide how to parse it based on the dynamic type).
/// @returns The number of bytes written.
/// @param buf The place in memory to output the JSON text to.
/// @param bufSize The size of the buffer that can safely be written to.
/// @todo Deal with commas at end of list.

int DynamicallyTypable::writeJsonFields(char* buf, unsigned bufSize)
{
  char* end = buf + bufSize;
  bufprintf("\"dynamicType\": \"%s\",\n", dynamicallyTypableName[getDynamicType()]);
  return (bufSize - (int)(end-buf));
}


// =======================================================================================

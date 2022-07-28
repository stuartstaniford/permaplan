// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for situations where we need to have pointers to a range of
// different things, and resolve at run time which kind they are.  This class
// provides means to do that.

#include "DynamicallyTypable.h"


// =======================================================================================

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
                            (char*)"TypeHWSDProfile"
};


// =======================================================================================
/// @brief Provide json objects which a receiver can parse based on the dynamic type.
/// @param buf The place in memory to output the JSON text to.
/// @param bufSize The size of the buffer that can safely be written to.

int DynamicallyTypable::writeJson(char* buf, unsigned bufSize)
{
  return 0;
}


// =======================================================================================

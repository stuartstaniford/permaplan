// Copyright Staniford Systems.  All Rights Reserved.  October 2020 -
// This class holds the necessary information for a particular kind of material -
// carbon data, texture image for showing the surface of it, etc


#ifndef MATERIAL_H
#define MATERIAL_H

#include <cglm/cglm.h>
#include <vector>
#include "Texture.h"
#include "HttpDebug.h"

// =======================================================================================
// Class variable initialization


class Material
{
public:
  
  // Instance variables - public
  const char* name;
  float       carbonDensity;

  // Member functions - public
  Material(float carbonD, char* matName, char* tFileName);
  ~Material(void);
  
private:
  
  // Instance variables - private
  Texture texture;
  
  // Member functions - private
  Material(const Material&);                 // Prevent copy-construction
  Material& operator=(const Material&);      // Prevent assignment

};

class MaterialList: public std::vector<Material*>
{
private:
  
  static MaterialList* theMaterialList;

public:
  
  // Instance variables - public
  
  // Member functions - public
  MaterialList(void);
  ~MaterialList();
  bool diagnosticHTML(HttpDebug* serv);
  static const MaterialList& getMaterials(void) // Get the singleton instance
  {
   return *theMaterialList;
  }

private:

  // Instance variables - private
  
  // Member functions - private
  MaterialList(const MaterialList&);                 // Prevent copy-construction
  MaterialList& operator=(const MaterialList&);      // Prevent assignment
};

#endif





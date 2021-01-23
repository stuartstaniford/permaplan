// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is a container for the factors governing the evolution of a particular
// species (mainly extracted from an OTDL JSON object).

#ifndef SPECIES_H
#define SPECIES_H

#include "HttpDebug.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Class variable initialization


class Species
{
 public:
  
  // Member functions - public
  Species(void);
  virtual ~Species(void);
  void                readOTDLFromFile(char* fileName);
  bool                validateOTDL(rapidjson::Document& doc);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  Species(const Species&);                 // Prevent copy-construction
  Species& operator=(const Species&);      // Prevent assignment
};

#endif





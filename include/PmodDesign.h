// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main json file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.

#ifndef PMODDESIGN_H
#define PMODDESIGN_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include "PmodConfig.h"

// =======================================================================================
// Class variable initialization


class PmodDesign
{
  public:
  
  // Instance variables - public
  rapidjson::Document   doc;
  PmodConfig&           config;

  // Member functions - public
  PmodDesign(PmodConfig& config);
  ~PmodDesign(void);
};

#endif


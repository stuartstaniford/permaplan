// Copyright Staniford Systems.  All Rights Reserved.  April 2023 -

#ifndef PMOD_SERVER_H
#define PMOD_SERVER_H

#include "Global.h"


// =======================================================================================
// Forward declarations

class HttpServThread;


// =======================================================================================
/// @brief This class coordinates operations for the handling of PmodDesign related HTTP
/// operations in Permaserv.

class PmodServer
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  PmodServer(void);
  ~PmodServer(void);
  bool indexPageTable(HttpServThread* serv);

private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(PmodServer);
};


// =======================================================================================

#endif

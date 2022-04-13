// Copyright Staniford Systems.  All Rights Reserved.  March 2022 -

#ifndef MDB_FILE_H
#define MDB_FILE_H

#include "mdbtools.h"

// =======================================================================================
// Forward declarations


// =======================================================================================
/// @brief A wrapper for the mdbtools library libmdb.wrapper
/// 
/// MDBTools is a set of tools for reading ".mdb" files - Microsoft Access Database 
/// Files.  This abstracts the limited amount of functionality we have needed and creates
/// a cleaner C++ interface.  It was originally introduced into the code for reading
/// the MDB file from the Harmonized World Soil Database 

class MdbDatabase
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MdbDatabase(char* fileName);
  ~MdbDatabase(void);
  
private:
  
  // Instance variables - private
  MdbHandle*        mdb;
  MdbCatalogEntry*  entry;

  // Member functions - private
  /// @brief Prevent copy-construction.
  MdbDatabase(const MdbDatabase&);       
  /// @brief Prevent assignment.
  MdbDatabase& operator=(const MdbDatabase&);      
};


// =======================================================================================

#endif





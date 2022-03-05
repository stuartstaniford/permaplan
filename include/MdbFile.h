// Copyright Staniford Systems.  All Rights Reserved.  March 2022 -

#ifndef MDB_FILE_H
#define MDB_FILE_H


// =======================================================================================
// Forward declarations

class MdbHandle;
class MdbCatalogEntry;


// =======================================================================================
/// @brief A wrapper for the mdbtools library libmdb.wrapper
/// 
/// MDBTools is a set of tools for reading ".mdb" files - Microsoft Access Database 
/// Files.  This abstracts the limited amount of functionality we have needed and creates
/// a cleaner C++ interface.  It was originally introduced into the code for reading
/// the MDB file from the Harmonized World Soil Database 

class MdbFile
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MdbFile(char* fileName);
  ~MdbFile(void);
  
private:
  
  // Instance variables - private
  MdbHandle*        mdb;
  MdbCatalogEntry*  entry;

  // Member functions - private
  /// @brief Prevent copy-construction.
  MdbFile(const MdbFile&);       
  /// @brief Prevent assignment.
  MdbFile& operator=(const MdbFile&);      
};


// =======================================================================================

#endif





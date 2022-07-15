// Copyright Staniford Systems.  All Rights Reserved.  March 2022 -

#ifndef MDB_FILE_H
#define MDB_FILE_H

#include "mdbtools.h"
#include <vector>


// =======================================================================================
// Forward declarations

class SoilProfile;
class SoilDatabase;


// =======================================================================================
/// @brief The information required to extract a particular entry in one column from a 
/// table and insert it into a structure.
/// This is handled as byte offsets into a void*.  Note this operation is a fairly 
/// tricky and trouble prone thing to do, and mistakes will turn the structure into 
/// binary mush.  Pay careful attention to types.

class MdbTableSchemaEntry
{
public:
  MdbTableSchemaEntry(int col, int offs);
  
private:
  int         colIndex;
  //MdbColumn*  column;
  int         offset;
};


// =======================================================================================
/// @brief The information required to extract particular rows from a table and insert
/// them into a structure (which is handled as byte offsets into a void*).

class MdbTableSchema: public std::vector<MdbTableSchemaEntry*>
{
  
};


// =======================================================================================
/// @brief The information required to read a table and extract the fields into an 
/// array of big buffers

class MdbTableReader
{

 public:
  MdbTableReader(MdbHandle* mdb, char* table_name, unsigned bindSize);
  ~MdbTableReader(void);
  bool getNextRow(void);
  
 private:
  // Instance variables - private
  MdbTableDef*  table;
  char**        boundValues;
  int*          boundLens;
};


// =======================================================================================
/// @brief A wrapper for the mdbtools library libmdb.
/// 
/// MDBTools is a set of tools for reading ".mdb" files - Microsoft Access Database 
/// Files.  This abstracts the limited amount of functionality we have needed and creates
/// a cleaner C++ interface.  It was originally introduced into the code for reading
/// the MDB file from the Harmonized World Soil Database 

class MdbDatabase
{
  friend SoilDatabase;
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  MdbDatabase(char* fileName);
  void logCatalog(void);
  bool fetchNextRowToStructure(void* S);
  ~MdbDatabase(void);
  
private:
  
  // Instance variables - private
  MdbHandle*        mdb;
  MdbCatalogEntry*  entry;
  char*             dbName;

  // Member functions - private
  /// @brief Prevent copy-construction.
  MdbDatabase(const MdbDatabase&);       
  /// @brief Prevent assignment.
  MdbDatabase& operator=(const MdbDatabase&);      
};


// =======================================================================================

#endif





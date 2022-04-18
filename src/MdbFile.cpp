// Copyright Staniford Systems.  All Rights Reserved.  March 2022 -
// A wrapper for the mdbtools library libmdb.wrapper.  MDBTools is a set of tools 
// for reading ".mdb" files - Microsoft Access Database Files.  This abstracts the 
// limited amount of functionality we have needed and creates a cleaner C++ interface.  
// It was originally introduced into the code for reading the MDB file from the 
// Harmonized World Soil Database 

// MdbTools is not very well documented.  This file was developed by studying the files:
// https://github.com/mdbtools/mdbtools/blob/dev/src/util/mdb-tables.c

#include "MdbFile.h"
//#include "mdbver.h"
#include "Global.h"
#include <err.h>


// =======================================================================================
/// @brief Constructor

MdbDatabase::MdbDatabase(char* fileName):
                                dbName(fileName)
{
    unless((mdb = mdb_open(fileName, MDB_NOFLAGS))) 
      err(-1, "Couldn't open .mdb file %s.\n", fileName);  
  
  logCatalog();
}


// =======================================================================================
/// @brief Log the catalog of the database file

void MdbDatabase::logCatalog(void)
{
  int objtype = MDB_TABLE;
  // Read the catalog
  unless(mdb_read_catalog(mdb, MDB_ANY))
    err(-1, "File %s does not appear to be an Access database.\n", dbName);

  // loop over each entry in the catalog
  for (int i=0; i<mdb->num_catalog; i++) 
   {
    entry = (MdbCatalogEntry*)g_ptr_array_index(mdb->catalog, i);

    if (entry->object_type != objtype && objtype!=MDB_ANY)
      continue;

    printf("%d\t", entry->object_type);
    printf ("%s\n", entry->object_name);
   }
}


// =======================================================================================
/// @brief Destructor

MdbDatabase::~MdbDatabase(void)
{
  mdb_close(mdb);
}


// =======================================================================================

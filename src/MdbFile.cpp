// Copyright Staniford Systems.  All Rights Reserved.  March 2022 -
// A wrapper for the mdbtools library libmdb.wrapper.  MDBTools is a set of tools 
// for reading ".mdb" files - Microsoft Access Database Files.  This abstracts the 
// limited amount of functionality we have needed and creates a cleaner C++ interface.  
// It was originally introduced into the code for reading the MDB file from the 
// Harmonized World Soil Database 

// MdbTools is poorly documented.  This file was developed by studying the files:
// https://github.com/mdbtools/mdbtools/blob/dev/src/util/mdb-tables.c
// https://github.com/mdbtools/mdbtools/blob/dev/src/util/mdb-schema.c
// https://github.com/mdbtools/mdbtools/blob/dev/src/util/mdb-json.c

#include "MdbFile.h"
//#include "mdbver.h"
#include "SoilProfile.h"
#include "Global.h"
#include <err.h>


// =======================================================================================
/// @brief Constructor
/// @param mdb Pointer to the MdbHandle for the database (which should be open already).
/// @param table_name The (char*) name of the table we want to open and read from.
/// @param bindSize The size (in bytes) of the buffers to use for reading out values.

MdbTableReader::MdbTableReader(MdbHandle* mdb, char* table_name, unsigned bindSize)
{
  mdb_set_bind_size(mdb, bindSize);
  table = mdb_read_table_by_name(mdb, table_name, MDB_TABLE);
  unless(table) 
    err(-1, "Error: Table %s does not exist in this database.\n", table_name);

  /* read table */
  mdb_read_columns(table);
  mdb_rewind_table(table);

  boundValues  = (char**)calloc(table->num_cols, sizeof(char *));
  boundLens    = (int*)calloc(table->num_cols, sizeof(int));
  
  for(int i=0; i<table->num_cols; i++) 
   {
    /* bind columns */
    boundValues[i] = (char*)calloc(bindSize, sizeof(char));
    int ret = mdb_bind_column(table, i+1, boundValues[i], &boundLens[i]);
    if (ret == -1) 
      err(-1, "Failed to bind column %d\n", i + 1);
   }
}


// =======================================================================================
/// @brief Destructor

MdbTableReader::~MdbTableReader(void)
{
  for(int i=0; i<table->num_cols; i++)
    free(boundValues[i]);
  free(boundValues);
  free(boundLens);
  mdb_free_tabledef(table);
}


// =======================================================================================
/// @brief Read the next row into our buffer structure
/// @returns True if we successfully read another row, false on end of the table.

bool MdbTableReader::getNextRow(void)
{
  return mdb_fetch_row(table);
  
  ///@todo Sanity checking here
}


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
    printf("%s:\t", entry->object_name);
    MdbTableDef *table = mdb_read_table(entry);
    printf("%d rows\n", table->num_rows);
    mdb_print_schema(mdb, stdout, entry->object_name, NULL, MDB_SHEXP_DEFAULT);
   }
}


// =======================================================================================
/// @brief Fetch a particular soil profile.

bool MdbDatabase::fetchNextRowToStructure(void* S)
{
  return false;
}


// =======================================================================================
/// @brief Destructor

MdbDatabase::~MdbDatabase(void)
{
  mdb_close(mdb);
}


// =======================================================================================

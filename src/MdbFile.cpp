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

MdbDatabase::MdbDatabase(char* fileName)
{
    unless((mdb = mdb_open(fileName, MDB_NOFLAGS))) 
      err(-1, "Couldn't open .mdb file %s.\n", fileName);
}


// =======================================================================================
/// @brief Destructor

MdbDatabase::~MdbDatabase(void)
{
}


// =======================================================================================

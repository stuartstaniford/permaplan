// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// Code for reading Band-Interleaved-Line (BIL) files.  The main purpose of this at 
// the moment is reading data from the World Harmonized Soil Database.  For more, see
// https://www.fao.org/3/aq361e/aq361e.pdf

// Useful documentation/background on the BIL file format can be found here:
// https://webhelp.esri.com/arcgisdesktop/9.3/index.cfm?topicname=BIL,_BIP,_and_BSQ_raster_files

#include "BILFile.h"
#include "Global.h"
#include <string.h>
#include <err.h>


// =======================================================================================
/// @brief Constructor

BILFile::BILFile(void)
{
}


// =======================================================================================
/// @brief Destructor

BILFile::~BILFile(void)
{
}


// =======================================================================================
/// @brief Function to read the header file so that we have the basic parameters of the
/// raster data.

bool BILFile::readHdrFile(char* path, char* stub)
{
  char fileName[256];
  char line[128];
  FILE* file;
  
  // Figure out the file name and open the file.
  snprintf(fileName, 256, "%s/%s.hdr", path, stub);
  file = fopen(fileName, "r");
  unless(file)
    err(-1, "Couldn't open %s.\n", fileName);
  
  // Loop over the lines in the file
  while(fgets(line, 128, file))
   {
    
    // See https://developer.ibm.com/articles/au-endianc/ for a helpful discussion
    // of byte order
    if( (strncmp(line, "BYTEORDER", 9) == 0) || (strncmp(line, "byteorder", 9) == 0))
     {
      // I—Intel byte order (Silicon Graphics, DEC Alpha, PC); also known as little endian
      // M—Motorola byte order (Sun, HP, and others); also known as big endian
      if(index(line + 9, 'I'))
        byteOrder = LITTLE_ENDIAN;
      else if(index(line + 9, 'M'))
        byteOrder = BIG_ENDIAN;
      else
        err(-1, "Bad byteOrder in file %s.\n", fileName);
     }

    if( (strncmp(line, "LAYOUT", 6) == 0) || (strncmp(line, "layout", 6) == 0))
     {
      char buf[8];
      if( (sscanf(line+6, "%7s", buf) == 1) && strcmp(buf, "BIL") == 0)
       {
        // Ok, do nothing
       }
      else
        err(-1, "LAYOUT is not BIL in file %s.\n", fileName);
     }

    if( (strncmp(line, "NROWS", 5) == 0) || (strncmp(line, "nrows", 5) == 0))
     {
      
     }

    if( (strncmp(line, "NCOLS", 5) == 0) || (strncmp(line, "ncols", 5) == 0))
     {
      
     }

    if( (strncmp(line, "NBANDS", 6) == 0) || (strncmp(line, "nbands", 6) == 0))
     {
      
     }

    if( (strncmp(line, "NBITS", 5) == 0) || (strncmp(line, "nbits", 5) == 0))
     {
      
     }

    if( (strncmp(line, "BANDROWBYTES", 12) == 0) || (strncmp(line, "bandrowbytes", 12) == 0))
     {
      
     }

    if( (strncmp(line, "TOTALROWBYTES", 13) == 0) || (strncmp(line, "totalrowbytes", 13) == 0))
     {
      
     }

    if( (strncmp(line, "BANDGAPBYTES", 12) == 0) || (strncmp(line, "bandgapbytes", 12) == 0))
     {
      
     }
    
   }
  
  // Clean up and go home.
  fclose(file);
  return false;
}


// =======================================================================================

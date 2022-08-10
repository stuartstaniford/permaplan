// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -
// Code for reading Band-Interleaved-Line (BIL) files.  The main purpose of this at 
// the moment is reading data from the World Harmonized Soil Database.  For more, see
// https://www.fao.org/3/aq361e/aq361e.pdf

// Useful documentation/background on the BIL file format can be found here:
// https://webhelp.esri.com/arcgisdesktop/9.3/index.cfm?topicname=BIL,_BIP,_and_BSQ_raster_files

#include "BILFile.h"
#include "Global.h"
#include "SoilProfile.h"
#include "Logging.h"
#include <string.h>
#include <err.h>


// =======================================================================================
/// @brief Constructor
/// 
/// @param fileNameStub The path to the directory of the BIL file, together with the 
/// part of the filename prior to the .extension (such as .hdr)

BILFile::BILFile(char* fileNameStub)
{
  readHdrFile(fileNameStub);
  readBlwFile(fileNameStub);
  char fileName[256];
  snprintf(fileName, 256, "%s.bil", fileNameStub);
  dataFile = fopen(fileName, "r");
  unless(dataFile)
    err(-1, "Couldn't open %s.\n", fileName);
  else
    LogBilFileDetails("Opened Bilfile data file %s for reading.\n", fileName);
}


// =======================================================================================
/// @brief Destructor

BILFile::~BILFile(void)
{
}


// =======================================================================================
/// @brief Function to read the BLW file which describes the mapping between world coords
/// and raster coords.
/// 
/// @returns True if successfully read and parsed the BLW file, false otherwise
/// @param fileNameStub The path to the directory of the BIL file, together with the 
/// part of the filename prior to the .extension (such as .hdr).  The format is poorly
/// documented but see Annex 3 of https://www.fao.org/3/aq361e/aq361e.pdf

// Example file from Materials/Solar/HWSD_RASTER/hwsd.blw
//          0.00833333333333 
//          0.00000000000000 
//          0.00000000000000 
//          -0.00833333333333 
//          -179.99583333333334 
//          89.99583333326137 

bool BILFile::readBlwFile(char* fileNameStub)
{
  char fileName[256];
  char line[128];
  FILE* file;
  
  // Figure out the file name and open the file.
  snprintf(fileName, 256, "%s.blw", fileNameStub);
  file = fopen(fileName, "r");
  unless(file)
    err(-1, "Couldn't open %s.\n", fileName);
  else
    LogBilFileDetails("Opened Bilfile blw %s for reading.\n", fileName);
  
  // Work through the lines in the file

  // Long pixel delta
  fgets(line, 128, file);
  sscanf(line, " %lf", &longPixelDelta);
  LogBilFileDetails("longPixelDelta: %f\n", longPixelDelta);

  // Unknown
  fgets(line, 128, file);

  // Unknown
  fgets(line, 128, file);

  // Lat pixel delta
  fgets(line, 128, file);
  sscanf(line, " %lf", &latPixelDelta);
  LogBilFileDetails("latPixelDelta: %f\n", latPixelDelta);

  // Long pixel start
  fgets(line, 128, file);
  sscanf(line, " %lf", &longPixelStart);
  LogBilFileDetails("longPixelStart: %f\n", longPixelStart);

  // Lat pixel start
  fgets(line, 128, file);
  sscanf(line, " %lf", &latPixelStart);
  LogBilFileDetails("latPixelStart: %f\n", latPixelStart);
  
  // Clean up and go home.
  fclose(file);
  return true;
}


// =======================================================================================
/// @brief Function to read the header file so that we have the basic parameters of the
/// raster data.
/// 
/// @returns True if successfully read and parsed header file, false otherwise
/// @param fileNameStub The path to the directory of the BIL file, together with the 
/// part of the filename prior to the .extension (such as .hdr)

// Example file from Materials/Solar/HWSD_RASTER/hwsd.hdr
//BYTEORDER      I
//LAYOUT       BIL
//NROWS         21600
//NCOLS         43200
//NBANDS        1
//NBITS         16
//BANDROWBYTES         86400
//TOTALROWBYTES        86400
//BANDGAPBYTES         0

bool BILFile::readHdrFile(char* fileNameStub)
{
  char fileName[256];
  char line[128];
  FILE* file;
  
  // Figure out the file name and open the file.
  snprintf(fileName, 256, "%s.hdr", fileNameStub);
  file = fopen(fileName, "r");
  unless(file)
    err(-1, "Couldn't open %s.\n", fileName);
  else
    LogBilFileDetails("Opened Bilfile header %s for reading.\n", fileName);
  
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
       {
        byteOrder = LITTLE_ENDIAN;
        LogBilFileDetails("Bilfile header %s reports LITTLE_ENDIAN.\n", fileName);        
       }
      else if(index(line + 9, 'M'))
       {
        byteOrder = BIG_ENDIAN;
        LogBilFileDetails("Bilfile header %s reports BIG_ENDIAN.\n", fileName);        
       }
      else
        err(-1, "Bad byteOrder in file %s.\n", fileName);
     }

    if( (strncmp(line, "LAYOUT", 6) == 0) || (strncmp(line, "layout", 6) == 0))
     {
      char buf[8];
      if( (sscanf(line+6, "%7s", buf) == 1) && strcmp(buf, "BIL") == 0)
       {
        LogBilFileDetails("Bilfile header %s reports layout is BIL.\n", fileName);        
       }
      else
        err(-1, "LAYOUT is not BIL in file %s.\n", fileName);
     }

    if( (strncmp(line, "NROWS", 5) == 0) || (strncmp(line, "nrows", 5) == 0))
     {
      if(sscanf(line+5, "%d", &nRows) != 1)
        err(-1, "Could not get NROWS from file %s.\n", fileName);
      else
        LogBilFileDetails("Bilfile header %s reports nRows = %d.\n", fileName, nRows);        
     }

    if( (strncmp(line, "NCOLS", 5) == 0) || (strncmp(line, "ncols", 5) == 0))
     {
      if(sscanf(line+5, "%d", &nCols) != 1)
        err(-1, "Could not get NCOLS from file %s.\n", fileName);
      else
        LogBilFileDetails("Bilfile header %s reports nCols = %d.\n", fileName, nCols);        
     }

    if( (strncmp(line, "NBANDS", 6) == 0) || (strncmp(line, "nbands", 6) == 0))
     {
      if(sscanf(line+6, "%d", &nBands) != 1)
        err(-1, "Could not get NBANDS from file %s.\n", fileName);
      else
        LogBilFileDetails("Bilfile header %s reports nBands = %d.\n", fileName, nBands);        
      if(nBands != 1)
        err(-1, "NBANDS is %d not 1 in file %s.\n", nBands, fileName);
     }

    if( (strncmp(line, "NBITS", 5) == 0) || (strncmp(line, "nbits", 5) == 0))
     {
      if(sscanf(line+5, "%d", &nBits) != 1)
        err(-1, "Could not get NBITS from file %s.\n", fileName);
      else
        LogBilFileDetails("Bilfile header %s reports nBits = %d.\n", fileName, nBits);        
      if(nBits != 16)
        err(-1, "NBITS is %d not 16 in file %s.\n", nBits, fileName);      
     }

    if( (strncmp(line, "BANDROWBYTES", 12) == 0) || (strncmp(line, "bandrowbytes", 12) == 0))
     {
      continue;
     }

    if( (strncmp(line, "TOTALROWBYTES", 13) == 0) || (strncmp(line, "totalrowbytes", 13) == 0))
     {
      continue;
     }

    if( (strncmp(line, "BANDGAPBYTES", 12) == 0) || (strncmp(line, "bandgapbytes", 12) == 0))
     {
      continue;
     }
    
   }
  
  // Clean up and go home.
  fclose(file);
  return true;
}


// =======================================================================================
/// @brief Find the value at a particular point from the binary bilFile itself.
/// 
/// @returns The value in the bilFile at the given lat,longt.
/// @param lat The float latitude of the requested location.
/// @param longt The The float longtitude of the requested location.

unsigned short BILFile::valueAtPoint(float lat, float longt)
{  
  // Need to fseek in the bilfile.
  //int row = 
  return 0u;
}


// =======================================================================================

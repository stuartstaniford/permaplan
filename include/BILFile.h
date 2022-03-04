// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef BIL_FILE_H
#define BIL_FILE_H


// =======================================================================================
// Forward declarations, etc

class SoilProfile;


// =======================================================================================
/// @brief Code for reading Band-Interleaved-Line (BIL) files.
///
/// The main purpose of this at the moment is reading data from the World Harmonized 
/// Soil Database.  For more, see https://www.fao.org/3/aq361e/aq361e.pdf.

class BILFile
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BILFile(char* fileNameStub);
  ~BILFile(void);
  SoilProfile* newProfileAtPoint(float lat, float longt);

private:
  
  // Instance variables - private
  int byteOrder;
  int nRows;
  int nCols;
  int nBands;
  int nBits;
  int bandRowBytes;
  int totalRowBytes;
  int bandGapBytes;
  
  // Member functions - private
  bool readHdrFile(char* fileNameStub);
  
  /// @brief Prevent copy-construction.
  BILFile(const BILFile&);       
  /// @brief Prevent assignment.
  BILFile& operator=(const BILFile&);      
};


// =======================================================================================

#endif





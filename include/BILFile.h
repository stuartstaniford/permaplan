// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef BIL_FILE_H
#define BIL_FILE_H


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
  BILFile(void);
  ~BILFile(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  BILFile(const BILFile&);       
  /// @brief Prevent assignment.
  BILFile& operator=(const BILFile&);      
};


// =======================================================================================

#endif





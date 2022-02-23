// Copyright Staniford Systems.  All Rights Reserved.  February 2022 -

#ifndef BIL_FILE_H
#define BIL_FILE_H


// =======================================================================================
/// @brief Code for reading Band-Interleaved-Line (BIL) files.
///
/// The main purpose of this at the moment is reading data from the World Harmonized 
/// Soil Database.  For more, see https://www.fao.org/3/aq361e/aq361e.pdf.

class BIPFile
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BIPFile(void);
  ~BIPFile(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  BIPFile(const BIPFile&);       
  /// @brief Prevent assignment.
  BIPFile& operator=(const BIPFile&);      
};


// =======================================================================================

#endif





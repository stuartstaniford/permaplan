// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -

#ifndef CRYPTO_ALGORITHMS_H
#define CRYPTO_ALGORITHMS_H


// =======================================================================================
/// @brief Singleton class providing all crypto algorithm services to the rest of the 
/// permaplan and permaserv code.
///
/// Generally this tries to hide the details of what algorithms are being used so that
/// they can be migrated over time to newer ones without disturbing the rest of the code.
/// Currently Uses libssl.

class CryptoAlgorithms
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  CryptoAlgorithms(void);
  ~CryptoAlgorithms(void);
  static CryptoAlgorithms& getCryptoAlgorithms(void) // Get the singleton instance
   { return *theCryptoAlgorithms; }

private:
  
  // Private static (class) variables
  static CryptoAlgorithms* theCryptoAlgorithms;
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  CryptoAlgorithms(const CryptoAlgorithms&);       
  /// @brief Prevent assignment.
  CryptoAlgorithms& operator=(const CryptoAlgorithms&);      
};


// =======================================================================================

#endif





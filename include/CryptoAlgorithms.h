// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -

#ifndef CRYPTO_ALGORITHMS_H
#define CRYPTO_ALGORITHMS_H

#include "Global.h"
#include <stdio.h>

#define HASH_BYTES 16
#define SALT_BYTES 16

// =======================================================================================
/// @brief Class to encapsulate the concept of salt for use in hashing a password.

class PasswordSalt
{
public:
  
  // Member functions public
  bool outputToFile(FILE* file);
  
private:
  
  // Instance variables private
  unsigned char salt[SALT_BYTES];

  // Member functions - private
  PreventAssignAndCopyConstructor(PasswordSalt);
};


// =======================================================================================
/// @brief Class to encapsulate a password hash.

class PasswordHash
{
public:
  
  
private:
  
  // Instance variables private
  unsigned char passwordHash[HASH_BYTES];

  // Member functions - private
  PreventAssignAndCopyConstructor(PasswordHash);
};


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
  PreventAssignAndCopyConstructor(CryptoAlgorithms);
};


// =======================================================================================

#endif





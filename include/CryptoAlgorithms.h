// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -

#ifndef CRYPTO_ALGORITHMS_H
#define CRYPTO_ALGORITHMS_H

#include "Global.h"
#include <stdio.h>
#include <openssl/sha.h>

#define SALT_BYTES 16
#define APPSTRING_BYTES SALT_BYTES
#define HASH_INPUT_LENGTH SALT_BYTES*3
#define HASH_BYTES SHA256_DIGEST_LENGTH


// =======================================================================================
// Forward declarations.

class PasswordHash;
class UserRecord;


// =======================================================================================
/// @brief Class to encapsulate the concept of salt for use in hashing a password.

class PasswordSalt
{
  friend PasswordHash;
  
public:
  
  // Member functions public
  PasswordSalt(void);
  PasswordSalt(FILE* file);
  bool outputToFile(FILE* file);
  
  // Static member functions
  static inline int diskLength(void){return SALT_BYTES;}
  
private:
  
  // Instance variables private
  unsigned char salt[SALT_BYTES];
  bool          fileReadOk;   // constructors can't return NULL

  // Member functions - private
  PreventAssignAndCopyConstructor(PasswordSalt);
};


// =======================================================================================
/// @brief Class to encapsulate a password hash.

class PasswordHash
{
  friend UserRecord;
  
public:

  // Member functions public
  PasswordHash(char* pwd, PasswordSalt& salt);
  PasswordHash(FILE* file);
  bool outputToFile(FILE* file);
  bool checkMatch(char* pwd, PasswordSalt& salt);
  
  // Static member functions
  static inline int diskLength(void){return HASH_BYTES;}

private:
  
  // Instance variables private
  unsigned char hash[HASH_BYTES];
  bool          fileReadOk;   // constructors can't return NULL

  // Member functions - private
  
  void makeHash(unsigned char* buf, char* pwd, PasswordSalt& salt);
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





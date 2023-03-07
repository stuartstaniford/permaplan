// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -
// Singleton class providing all crypto algorithm services to the rest of the permaplan 
// and permaserv code.  Generally this tries to hide the details of what algorithms are 
// being used so that they can be migrated over time to newer ones without disturbing 
// the rest of the code.  Currently Uses libssl.

#include "CryptoAlgorithms.h"
#include "Logging.h"
#include <err.h>
#include <sys/random.h>
#include <string.h>


// =======================================================================================
// Static variables, etc.

CryptoAlgorithms* CryptoAlgorithms::theCryptoAlgorithms = NULL;
CryptoAlgorithms  CryptoAlgorithmInstance;
unsigned char appString[APPSTRING_BYTES] = { 
                                              0xa7, 0x18, 0x2a, 0xf9, 
                                              0x5a, 0xb4, 0xaa, 0xc8,
                                              0x29, 0x4b, 0x16, 0x7e,
                                              0x9d, 0xb8, 0x0a, 0xfe
                                            }; 


// =======================================================================================
/// @brief Constructor: initialize salt with random data

PasswordSalt::PasswordSalt(void)
{
  if(getentropy(salt, SALT_BYTES) != 0)
    err(-1, "Error return from getentropy.\n");
}


// =======================================================================================
/// @brief Constructor: read salt in from file
///
/// @param file A pointer to the stdio FILE structure for the file (open for reading).

PasswordSalt::PasswordSalt(FILE* file)
{
  if(fread(salt, SALT_BYTES, 1, file) != 1)
    fileReadOk = false;
  else
    fileReadOk = true;    
}


// =======================================================================================
/// @brief Output the salt to a file
///
/// @param file A pointer to the stdio FILE structure.
/// @returns True if written correctly, false otherwise

bool PasswordSalt::outputToFile(FILE* file)
{
  if(fwrite(salt, SALT_BYTES, 1, file) != 1)
    return false;
  return true;
}


// =======================================================================================
/// @brief Constructor: create the hash from password and salt
///
/// @param pwd A char* pointer to the new password.
/// @param salt A reference to the PasswordSalt to be used in creating the hash

PasswordHash::PasswordHash(char* pwd, PasswordSalt& salt)
{
  makeHash(hash, pwd, salt);
}


// =======================================================================================
/// @brief Constructor: read the hash in from a file
///
/// @param file A pointer to the open stdio FILE structure.

PasswordHash::PasswordHash(FILE* file)
{
  if(fread(hash, HASH_BYTES, 1, file) != 1)
    fileReadOk = false;
  else
    fileReadOk = true;    
}


// =======================================================================================
/// @brief Create the hash from password and salt
///
/// Used both in the constructor to make the hash in the first place, and also in 
/// password checking to create a temporary hash to compare to the stored one.
/// @param buf The location to store the constructed hash.  Needs to store HASH_BYTES.
/// @param pwd A char* pointer to the password.
/// @param salt A reference to the PasswordSalt to be used in creating the hash

void PasswordHash::makeHash(unsigned char* buf, char* pwd, PasswordSalt& salt)
{
  unsigned char input[HASH_INPUT_LENGTH];
  int pwdSize = strlen(pwd);

  // Build the hash input, interleaving salt, appstring, and password
  for(int i=0; i<SALT_BYTES; i++)
   {    
    input[3*i]    = appString[i];
    input[3*i+1]  = salt.salt[i];
    if(i < pwdSize)
      input[3*i+2] = pwd[i];
    else
      input[3*i+2] = appString[APPSTRING_BYTES-i-1];
   }
  
  // Do the crypto library hash function
  SHA256(input, HASH_INPUT_LENGTH, buf);
#ifdef LOG_CRYPTO_OPS
  char textBuf[256];
  hexdump(input, HASH_INPUT_LENGTH, textBuf);
  LogCryptoOps("Hash input:\n%s", textBuf);
  hexdump(hash, HASH_BYTES, textBuf);
  LogCryptoOps("Hash output:\n%s", textBuf);
#endif
  
  // Clean up memory of password data in the clear
  bzero(input, HASH_INPUT_LENGTH);
  bzero(pwd, pwdSize);
}


// =======================================================================================
/// @brief Check if a supplied password matches our stored hash.
///
/// The supplied password string will be zero'd out.
/// @param pwd A char* pointer to the password.
/// @param salt A reference to the PasswordSalt to be used in creating the hash

bool PasswordHash::checkMatch(char* pwd, PasswordSalt& salt)
{
  // Make the hash
  unsigned char buf[HASH_BYTES];
  makeHash(buf, pwd, salt);

  // Check if the newly made one matches our stored one
  for(int i=0; i<HASH_BYTES; i++)
    if(buf[i] != hash[i])
     {
      LogCryptoOps("Hash match failed at byte %d.\n", i);
      return false;
     }
  
  // Every byte matched, so call it good
  return true;
}


// =======================================================================================
/// @brief Output the hash to a file
///
/// @param file A pointer to the stdio FILE structure.
/// @returns True if written correctly, false otherwise

bool PasswordHash::outputToFile(FILE* file)
{
  if(fwrite(hash, HASH_BYTES, 1, file) != 1)
    return false;
  return true;
}


// =======================================================================================
/// @brief Constructor

CryptoAlgorithms::CryptoAlgorithms(void)
{
  if(theCryptoAlgorithms)
    return;
  else
    theCryptoAlgorithms = this;
}


// =======================================================================================
/// @brief Destructor

CryptoAlgorithms::~CryptoAlgorithms(void)
{
}


// =======================================================================================

// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -
// Singleton class providing all crypto algorithm services to the rest of the permaplan 
// and permaserv code.  Generally this tries to hide the details of what algorithms are 
// being used so that they can be migrated over time to newer ones without disturbing 
// the rest of the code.  Currently Uses libssl.

#include "CryptoAlgorithms.h"
#include "Logging.h"


// =======================================================================================
// Static variables, etc.

CryptoAlgorithms* CryptoAlgorithms::theCryptoAlgorithms = NULL;


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

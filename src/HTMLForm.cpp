// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -
// Represents the results of filling out an HTML form.  Derives from 
// std::unordered_map

#include "HTMLForm.h"
#include "Logging.h"

char* HTMLForm::allowedSymbols = (char*)"-_*^%$@!";

// =======================================================================================
/// @brief Constructor
/// @param formString A char* to the ApplicationXWWWFormUrlEncoded form body.  Note that
/// it will generally not be null-terminated (which is a pain).  
/// Eg: uname=foo&psw=bar
/// Note that the map contains pointers into the original string, so it cannot be used
/// after that is freed or goes out of scope.  For the last (unterminated) string we make
/// a copy and deallocate that in our destructor

HTMLForm::HTMLForm(char* formString, unsigned size)
{
  char*     first   = formString;
  unsigned  left    = size;
  char*     second;
  char*     end;
  
  lastToken = NULL;
  
  while(left > 0)  // loop over form field name/value pairs
   {
    unless( (end = (char*)memchr(first, '=', left)) )
     {
      // can't find "="
      LogRequestErrors("Can't find equals in form.\n"); 
      return;
     }
    unless(end > first)
     {
      // nothing before the "="
      LogRequestErrors("Empty name in form.\n"); 
      return;      
     }
    *end = '\0';
    unless(sanityCheck(first))
     {
      LogRequestErrors("Disallowed characters in form %s.\n", first); 
      return;
     }
    
    // At this point, we have a valid name, now need a value.
    left -= end-first+1;
    second = end + 1;
    unless(left > 0 && *second)
     {
      // Ran out of string with no value
      LogRequestErrors("Empty value in form for name %s.\n", first);
      return;
     }
    if( (end = (char*)memchr(second, '&', left)) )
     {
      // Not the last value
      *end = '\0';
      left -= end-second+1;
     }
    else
     {
      // The last value, where there is no space for null termination, so make a copy
      lastToken = new char[left+1];
      strncpy(lastToken, second, left);
      lastToken[left] = '\0';
      second = lastToken;
      left = 0u;
     }
    unless(sanityCheck(second))
     {
      LogRequestErrors("Disallowed characters in form value %s for name %s.\n", 
                                                                          second, first); 
      if(lastToken)
        delete[] lastToken;
      return;
     }

    // At this point, we have a valid name and value, so store the pair
    insert({std::string(first), second});
    LogRequestParsing("Added %s:%s to HTMForm object.\n", first, second);
    if(left > 0u)
      first = end + 1;
   }
}


// =======================================================================================
/// @brief Function to check that a symbol is legal to put into the form.  
/// 
/// This function is security critical as it is a gateway for what characters can get 
/// into forms in the rest of the system.
/// @returns True if all characters are legal, false otherwise.
/// @param symbol A char* pointer to a null-terminated name or value to check.

bool HTMLForm::sanityCheck(char* symbol)
{
  for(char* p = symbol; *p; p++)
   {
    if(isalnum(*p))
      continue;
    if(isspace(*p))
      continue;
    char* q;
    for(q = allowedSymbols; *q; q++)
     {
      if(*p == *q)
        break;  // out of loop over allowedSymbols
     }
    if(*p == *q)
      continue; // we satisfactorily matched on an allowed symbol, so go onto next char
    LogHTTPDetails("Form checking failing on character %c (%X)", *p, *p);
    return false;
   }
  
  return true;
}


// =======================================================================================
/// @brief Destructor

HTMLForm::~HTMLForm(void)
{
  delete[] lastToken;
}


// =======================================================================================

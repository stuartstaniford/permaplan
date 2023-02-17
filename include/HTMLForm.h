// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -

#ifndef HTML_FORM_H
#define HTML_FORM_H

#include <unordered_map>
#include <string>


// =======================================================================================
/// @brief Represents the results of filling out an HTML form.
///
/// Derives from std::unordered_map.  Note that the map contains pointers into the 
/// original string this is constructed from (typically an HTML body in a buffer, so 
/// it cannot be used after that is freed or goes out of scope

class HTMLForm: public std::unordered_map<std::string, char*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HTMLForm(char* formString, unsigned size);
  ~HTMLForm(void);
  
private:
  
  // Instance variables - private
  char* lastToken;
  
  // Member functions - private
  bool sanityCheck(char* symbol);
  /// @brief Prevent copy-construction.
  HTMLForm(const HTMLForm&);       
  /// @brief Prevent assignment.
  HTMLForm& operator=(const HTMLForm&);      
};


// =======================================================================================

#endif





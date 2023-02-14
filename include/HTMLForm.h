// Copyright Staniford Systems.  All Rights Reserved.  February 2023 -

#ifndef HTML_FORM_H
#define HTML_FORM_H

#include <unordered_map>
#include <string>


// =======================================================================================
/// @brief Represents the results of filling out an HTML form.
///
/// Derives from std::unordered_map.

class HTMLForm: public std::unordered_map<std::string, char*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HTMLForm(void);
  ~HTMLForm(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HTMLForm(const HTMLForm&);       
  /// @brief Prevent assignment.
  HTMLForm& operator=(const HTMLForm&);      
};


// =======================================================================================

#endif





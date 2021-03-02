// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class to model/manipulate OLDF, OTDL, etc version numbers.


#ifndef VERSION_H
#define VERSION_H

// =======================================================================================
// Class variable initialization


class Version
{
 public:
  
  // Instance variables - public
  unsigned char version[3];
  
  // Member functions - public
  Version(unsigned major, unsigned minor, unsigned increment);
  ~Version();


 private:
  
  // Instance variables - private
  
  // Member functions - private
};


#endif





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
  unsigned version[3];
  
  // Member functions - public
  inline Version() {};
  Version(unsigned major, unsigned minor, unsigned increment);
  ~Version();
  void set(unsigned major, unsigned minor, unsigned increment);
  char* printJSON(void);

  // Operators - public
  void operator=(const Version& inward);      // assignment
  unsigned operator[] (int i);               // subscript


 private:
  
  // Instance variables - private
  static char buf[64];
  
  // Member functions - private
};


#endif

#ifndef VERSION_IMPLEMENTATION
extern Version currentOLDFVersion;
extern Version currentOTDLVersion;
#endif

void setExternVersions(void);

// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Configuration object for the pmod program.  Encapsulates
// what are typically globally used variables supplied at
// program start.

#ifndef PMODCONFIG_H
#define PMODCONFIG_H

class PmodConfig
{
  public:
  
  // Instance variables - public
  char*           progName;
  bool            plotAxes;
  char*           designFileName;
  bool            plotGrid;
  float           gridSpacing;
  unsigned short  debugPort;
  bool            levelPlane;
  char*           bezReadFileName;
  char*           bezWriteFileName;
  
  private:
  
  static PmodConfig* config;

  public:
  
  // Member functions - public
  PmodConfig(int argc, char* argv[]);
  ~PmodConfig(void);
  void usage(void);
  bool isValid(void);
  static const PmodConfig& getConfig(void) // Get the singleton instance
   {
    return *config;
   }
};

#endif


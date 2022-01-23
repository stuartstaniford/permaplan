// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef PMODCONFIG_H
#define PMODCONFIG_H


// =======================================================================================
/// @brief Configuration object for the permaplan program.
/// 
/// This class encapsulates what are typically globally used variables supplied at
/// program start. (Pmod was an early name for permaplan which currently still survives
/// in the name of this class).  

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
  unsigned short  permaservPort;
  bool            levelPlane;
  char*           bezReadFileName;
  char*           bezWriteFileName;
  char*           writeDesignFileName;
  char*           speciesDirectory;
  unsigned        nSimThreads;

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

private:

  // Member functions - private
  PmodConfig(const PmodConfig&);                 // Prevent copy-construction
  PmodConfig& operator=(const PmodConfig&);      // Prevent assignment
};


// =======================================================================================

#endif


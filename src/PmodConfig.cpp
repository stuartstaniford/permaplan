// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Configuration object for the pmod program.  Encapsulates
// what are typically globally used variables supplied at
// program start.

#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include "PmodConfig.h"

PmodConfig* PmodConfig::config = NULL;

// =======================================================================================
// Educate the poor user

void PmodConfig::usage()
{
  printf("\nUsage:\n\n%s [options]\n\nOptions:\n\n", progName);
  
  printf("\t-A\tPlot X (red), Y (green), and Z (blue) axes.\n");
  printf("\t-b F\tRead Bezier patch state from file F.\n");
  printf("\t-B F\tWrite the Bezier patch state to file F.\n");
  printf("\t-d F\tRead OLDF design file F.\n");
  printf("\t-D F\tUse F as file to write out OLDF design.\n");
  printf("\t-g f\tAdd square gridlines every f units.\n");
  printf("\t-L\tLeave land surface as a plane.\n");
  printf("\t-p P\tRun debug server on port P .\n");
  printf("\t-P S\tUse S as plant species directory.\n");
  printf("\t-s N\tUse s simulation threads.\n");
  printf("\n");
  exit(0);
}


// =======================================================================================
// Constructor: process the command line - possibly exit if we don't like the taste

PmodConfig::PmodConfig(int argc, char* argv[])
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getConfig()
  if(config)
    return;
  else
    config = this;
  
  int optionChar;
  
  progName            = strdup(argv[0]);
  plotAxes            = false;
  designFileName      = NULL;
  debugPort           = 2080;
  levelPlane          = false;
  bezReadFileName     = NULL;
  bezWriteFileName    = NULL;
  writeDesignFileName = NULL;
  speciesDirectory    = NULL;
  nSimThreads         = 4;
  
  while( (optionChar = getopt(argc, argv, "Ab:B:d:D:g:Lp:P:s:")) != -1)
    switch (optionChar)
     {
      case 'A':
        plotAxes = true;
        break;

       case 'b':
         bezReadFileName = optarg;
         break;

       case 'B':
         bezWriteFileName = optarg;
         break;

       case 'd':
         designFileName = optarg;
         break;

       case 'D':
         writeDesignFileName = optarg;
         break;

       case 'g':
         plotGrid    = true;
         gridSpacing = atof(optarg);
         if(gridSpacing <= 0.0f)
            err(-1, "Bad gridspacing via -g: %s\n", optarg);
         break;

       case 'L':
         levelPlane = true;
         break;
      
       case 'p':
         debugPort = atoi(optarg);
         if(!debugPort)
           err(-1, "Bad port number via -p: %s\n", optarg);
         break;

       case 'P':
         speciesDirectory = optarg;
         break;

       case 's':
        nSimThreads = atoi(optarg);
         if(nSimThreads <= 0)
           err(-1, "Bad simulation thread number via -s: %s\n", optarg);
         break;

       default:
         usage();
      }
  if(!speciesDirectory)
    speciesDirectory = (char*)"./Materials/Trees";
}


// =======================================================================================
// Destructor (pro-forma)

PmodConfig::~PmodConfig()
{
  free(progName);
}


// =======================================================================================
// Double check that all constraints of the configuration are A-ok.

bool PmodConfig::isValid(void)
{
  if(!designFileName)
    return false;
  
  return true;
}


// =======================================================================================

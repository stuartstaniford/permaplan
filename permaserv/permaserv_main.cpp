// Main program for the permaserv HTTP server which exists to run at central HQ and
// supply permaplan instances with values required from very large databases that it
// wouldn't make sense to download in their entirety (eg solar DIF and DNI values,
// values from soil databases at particular locations, temperature or precipitation
// time series from climate model runs).

#include "HttpLBPermaserv.h"
#include "Logging.h"
#include <cstdio>
#include <stdexcept>
#include <pthread.h>
#include <err.h>
#include <unistd.h>


// =======================================================================================
// Global variables

unsigned short servPort = 2080u;

// =======================================================================================
// C function to launder C++ method into pthread_create

void* callProcessConn(void* arg)
{
  HttpLBPermaserv* hd = (HttpLBPermaserv*)arg;
  return hd->processConnections();
}


// =======================================================================================
/// @brief Educate the poor user on the command line options we support.

void printUsage(int argc, char* argv[])
{
  printf("\nUsage:\n\n%s [options]\n\nOptions:\n\n", argv[0]);
  
  printf("\t-h\tPrint this message.\n");
  printf("\t-p P\tRun server on port P .\n");
  printf("\n");
  exit(0);
}


// =======================================================================================
/// @brief Process the command line.
/// 
/// Uses C-style getopt() processing.  It may possibly exit if we don't like the taste.
/// @param argc Number of command line arguments from main()
/// @param argv Command line arguments from main().

void processCommandLine(int argc, char* argv[])
{  
  int optionChar;

  while( (optionChar = getopt(argc, argv, "hp:")) != -1)
    switch (optionChar)
     {
       case 'h':
         printUsage(argc, argv);
         exit(0);
      
       case 'p':
         servPort = atoi(optarg);
         if(!servPort)
           err(-1, "Bad port number via -p: %s\n", optarg);
         break;

       default:
        printUsage(argc, argv);
        exit(0);
      }
}


// =======================================================================================
// Main function

int main (int argc, char* argv[])
{
  // Initialize logging so we can record the rest of the start-up.
  LogInit();
    
  processCommandLine(argc, argv);
  
  // Start up the debugging http server
  HttpLBPermaserv httpServer(servPort);
  int             pthreadErr;
  pthread_t       httpThread;
  if((pthreadErr = pthread_create(&httpThread, NULL, callProcessConn, &httpServer)) != 0)
    err(-1, "Couldn't spawn HTTP server thread in %s.\n", argv[0]);
  
  sleep(1e8);
  
  return 0;
}


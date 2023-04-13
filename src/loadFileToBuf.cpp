// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility function for loading a file into a buffer.

#include "loadFileToBuf.h"
#include <err.h>
#include <sys/stat.h> // For POSIX systems (Linux, macOS)
#include <sys/time.h>
#include <sys/types.h> // For POSIX systems (Linux, macOS)
#include <cstring>
// #include <direct.h> // Uncomment this for Windows systems


// =======================================================================================
// Function to check that a file is present and is a plain old regular file

bool regularFileExists(char* fileName)
{
  struct stat params;
  if(stat(fileName, &params)<0)
    return false;
  if(!(params.st_mode&S_IFREG))
    return false;
  return true;
}


// =======================================================================================
// Function to return the time since a file was modified (in seconds).  Returns a 
// negative number on error

float getFileAge(char* fileName)
{
  struct stat params;
  if(stat(fileName, &params) < 0)
    return -1.0f;
  struct timeval now;
  gettimeofday(&now, NULL);
  
  return (now.tv_sec - params.st_mtimespec.tv_sec) 
                        + (now.tv_usec/1000.0f - params.st_mtimespec.tv_nsec/1000000.0f);
}


// =======================================================================================
// Function to check that a file is present and is a directory

bool directoryExists(char* dirName)
{
  struct stat params;
  if(stat(dirName, &params) < 0)
    return false;
  if(!(params.st_mode&S_IFDIR))
    return false;
  return true;
}


// =======================================================================================
// Function to create a directory

bool createDirectory(char* dirName)
{
  int result;
  
  // For POSIX systems (Linux, macOS)
  result = mkdir(dirName, 0755); // 0755 is the access mode (rwxr-xr-x)

  // For Windows systems
  // result = _mkdir(dirname);

  if(result == 0)
    return true;
  else 
    return false;
}


// =======================================================================================
// Takes a string (like "775" and generates an unsigned with the low nine bits reflecting
// the designated permission bits (so in this example it would be 
// 00000000000000000000000111111101).  See "man chmod" for the exact bit definitions.

int modeBitsFromString(const char* modeString)
{
  if(strlen(modeString) != 3)
    return -1;
  for(int j=0; j<3; j++)
    if(modeString[j] < '0' || modeString[j] > '7' )
      return -2;
  unsigned ownerPerms = modeString[0];  
  unsigned groupPerms = modeString[1];  
  unsigned worldPerms = modeString[2];

  return (ownerPerms<<6 | groupPerms<<3 | worldPerms);  
}


// =======================================================================================
// Check to see if a particular file/directory has the expected 9 bit permissions. 
// If not, returns false and fixes it.  It they are correct, returns true.

bool checkAndFixPermissions(const char* path, int mode)
{
  struct stat params;
  if(stat(path, &params) < 0)
    err(-1, "Couldn't stat %s.", path);
  if((params.st_mode & 0x00000fff) == mode)
    return true;
  chmod(path, mode);
  return false;
}


// =======================================================================================
// Load a file into a freshly allocated string.  Null terminate the string.
// Will pick up most file errors, but will silently load an empty file into string "\0"

char* loadFileToBuf(const char* fileName, unsigned* size)
{
  struct stat params;
  if(stat(fileName, &params) < 0)
    err(-1, "Couldn't stat fileName %s\n", fileName);
  if(!(params.st_mode & S_IFREG))
    err(-1, "File not regular file: %s\n", fileName);
  *size = params.st_size;
  char* buf = new char[*size+1];
  if(!buf)
    err(-1, "Couldn't create buffer of size %d\n", *size+1);
  FILE* file = fopen(fileName, "r");
  if(!file)
    err(-1, "Couldn't open file: %s\n", fileName);
  if(fread(buf, 1, *size, file) < *size)
    err(-1, "Couldn't read file: %s\n", fileName);
  
  fclose(file);
  buf[*size] = '\0';
  return buf;
}



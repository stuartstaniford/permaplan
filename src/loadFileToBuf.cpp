#include <err.h>
#include <sys/stat.h>
#include "loadFileToBuf.h"


// =======================================================================================
// Load a file into a freshly allocated string.  Null terminate the string.
// Will pick up most file errors, but will silently load an empty file into string "\0"

char* loadFileToBuf(const char* fileName, unsigned* size)
{
  struct stat params;
  if(stat(fileName, &params)<0)
  err(-1, "Couldn't stat fileName %s\n", fileName);
  if(!(params.st_mode&S_IFREG))
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



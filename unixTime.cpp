// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Utility program to print the current unix time in the form of a JSON
// array.

// Compilation
// clang++ -o unixTime -I include src/Timeval.o src/unixTime.cpp

#include <cstdio>
#include "Timeval.h"

int main(int argc, char* argv[])
{
  Timeval T;
  T.now();
  
  printf("[%ld, %d]\n", T.tv_sec, T.tv_usec);
}

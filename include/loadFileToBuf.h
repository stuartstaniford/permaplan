// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility function for loading a file into a buffer.

#include <stdio.h>

bool regularFileExists(char* fileName);

char* loadFileToBuf(const char* fileName, unsigned* size);

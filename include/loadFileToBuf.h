// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility function for loading a file into a buffer.

#include <stdio.h>

bool regularFileExists(char* fileName);
bool directoryExists(char* dirName);
bool createDirectory(char* dirName);
float getFileAge(char* fileName);

int modeBitsFromString(const char* modeString);
bool checkAndFixPermissions(const char* path, int mode);
char* loadFileToBuf(const char* fileName, unsigned* size);

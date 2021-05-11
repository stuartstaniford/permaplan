// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// The purpose of this class is to read from a socket, identify the boundaries of
// whole HTTP request messages, and return a pointer to the beginning of such
// messages.  Right now, it doesn't deal with requests with entity bodies (because
// it doesn't need to).  It null terminates the header so that higher layers that
// call on this one can treat the request header as a single string if they wish.
// We try to minimize extra copying, but there may be situations where some copying
// within our buffer is necessary.

#include "HttpRequestParser.h"

// =======================================================================================
// Constructor

HttpRequestParser::HttpRequestParser(void)
{
}


// =======================================================================================
// Destructor

HttpRequestParser::~HttpRequestParser(void)
{
}


// =======================================================================================

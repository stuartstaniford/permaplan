// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// The purpose of this class is to read from a socket, identify the boundaries of
// whole HTTP request messages, and return pointers to the beginnings of needed 
// fields.  Right now, it doesn't deal with requests with entity bodies (because
// it doesn't need to).  It also parses the header and stores offsets
// to key fields, and terminates each header line with a NULL so higher layers
// can parse individual headers to taste.  We try to minimize extra copying, but 
// there are corner-cases where some copying within our buffer is necessary.  

#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

// =======================================================================================
// Class variable initialization

class HttpRequestParser
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpRequestParser(unsigned size);
  ~HttpRequestParser(void);
  bool getNextRequest(void);
  inline void setNewConnection(int fd) {connfd = fd;}
  inline char* getMethod(void) {return buf;}
  inline char* getUrl(void) {return buf + urlOffset;}
  inline char* getHTTPVersion(void) {return buf + httpVerOffset;}
  
private:
  
  // Instance variables - private
  char*               buf;
  int                 connfd;
  unsigned            bufSize;
  unsigned            urlOffset;
  unsigned            httpVerOffset;
  
  // Member functions - private
  HttpRequestParser(const HttpRequestParser&);                 // Prevent copy-construction
  HttpRequestParser& operator=(const HttpRequestParser&);      // Prevent assignment

};

#endif





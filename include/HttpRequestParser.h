// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -

#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include "MimeTypeMaps.h"
#include <unordered_map>
#include <string>


// =======================================================================================
/// @brief An enum to encapsulate the different types of headers than can show up in HTTP.

enum HTTPHeaderType
{
  NoHeaderPresent,
  Connection,
  UserAgent,
  ContentLength,
  ContentType,
  TransferEncoding,
  Upgrade,
  Cookie,
};

// =======================================================================================
/// @brief An enum to encapsulate the different types of methods that we support in HTTP.

enum HTTPMethodType
{
  NoMethod,
  GET,
  POST,
};


// =======================================================================================
// Other forward declarations.

class DynamicallyTypable;

// =======================================================================================
/// @brief Parse one HTTP request.
/// 
/// The purpose of this class is to read from a socket, identify the boundaries of 
/// whole HTTP request messages, and return pointers to the beginnings of needed fields. 
/// Right now, it doesn't deal with requests with entity bodies (because it doesn't need 
/// to).  It also parses the header and stores offsets to key fields, and terminates 
/// each header line with a NULL so higher layers can parse individual headers to 
/// taste.  We try to minimize extra copying, but there are corner-cases where some 
/// copying within our buffer is necessary.  

class HttpRequestParser
{
public:
  
  // Instance variables - public
  bool                connectionWillClose;
  HTTPMethodType      requestMethod;
  DynamicallyTypable* parsedBody;

  // Member functions - public
  HttpRequestParser(unsigned size);
  void resetForNewRequest(void);
  void resetForNewConnection(void);
  ~HttpRequestParser(void);
  bool getNextRequest(void);
  inline void setNewConnection(int fd) {connfd = fd;}
  inline char* getUrl(void) {return buf + urlOffset;}
  inline char* getBodyString(void) {return headerEnd;} // Note, not null terminated
  inline char* getHTTPVersion(void) {return buf + httpVerOffset;}
    
private:
  
  // Instance variables - private
  std::unordered_map<std::string, HTTPHeaderType> headerMap; 
  char*               buf;
  char*               readPoint;
  char*               headerEnd;
  char*               cookieValue;
  unsigned            bufLeft;
  int                 connfd;
  unsigned            bufSize;
  unsigned            urlOffset;
  unsigned            httpVerOffset;
  bool                connectionDone;
  bool                bodyPresent;
  unsigned            bodySize;
  MimeType            contentType;
  
  // Member functions - private
  bool parseRequest(void);
  bool processBody(void);
  bool readAndCheck(int& nBytes);
  char* headerEndPresent(char* range, unsigned rangeSize);
  HttpRequestParser(const HttpRequestParser&);                 // Prevent copy-construction
  HttpRequestParser& operator=(const HttpRequestParser&);      // Prevent assignment

};


// =======================================================================================

#endif





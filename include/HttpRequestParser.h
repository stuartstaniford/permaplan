// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -

#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <unordered_map>
#include <string>

enum HTTPHeaderType
{
  NoHeaderPresent,
  Connection,
  UserAgent,
  ContentLength,
  ContentType,
  TransferEncoding,
  Upgrade,
};


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
  bool    connectionWillClose;
  
  // Member functions - public
  HttpRequestParser(unsigned size);
  void resetForReuse(void);
  ~HttpRequestParser(void);
  bool getNextRequest(void);
  inline void setNewConnection(int fd) {connfd = fd;}
  inline char* getMethod(void) {return buf;}
  inline char* getUrl(void) {return buf + urlOffset;}
  inline char* getHTTPVersion(void) {return buf + httpVerOffset;}
  
private:
  
  // Instance variables - private
  std::unordered_map<std::string, HTTPHeaderType> headerMap; 
  char*               buf;
  char*               readPoint;
  char*               headerEnd;
  unsigned            bufLeft;
  int                 connfd;
  unsigned            bufSize;
  unsigned            urlOffset;
  unsigned            httpVerOffset;
  bool                connectionDone;

  // Member functions - private
  bool parseRequest(void);
  char* headerEndPresent(char* range, unsigned rangeSize);
  HttpRequestParser(const HttpRequestParser&);                 // Prevent copy-construction
  HttpRequestParser& operator=(const HttpRequestParser&);      // Prevent assignment

};


// =======================================================================================

#endif





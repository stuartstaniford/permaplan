#include <cstdio>
#include <exception>

class pmodException: std::runtime_error
{
 public:
  pmodException(char* msg) : std::runtime_error(msg)
   {
    ;
   }
};

class pmodGLException: pmodException
{
 public:
  pmodGLException(char* msg) : pmodException(msg)
   {
    ;
   }

  /*const char* what()
   {
    return "OpenGL Error of some flavor or other.";
   }*/
};

class pmodGLFWException: pmodGLException
{
 public:
  pmodGLFWException(char* msg) : pmodGLException(msg)
   {
    ;
   }

 /* const char* what()
  {
   return ((std::runtime_error)this)->what();
  }*/
};


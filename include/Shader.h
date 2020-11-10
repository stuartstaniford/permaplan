// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for an openGL shader pipeline.

#ifndef SHADER_H
#define SHADER_H

#include <cstdio>
#include <cglm/cglm.h>

// =======================================================================================
// Utility function

int checkGLError(FILE* file, const char* preamble);

// =======================================================================================
// Class variable initialization


class Shader
{
 public:

  // Instance variables - public
 
 private:
  // Pointers to instances
  static Shader* mainShader;

 public:
  // Member functions - public
  Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
  ~Shader(void);
  void useProgram(void);
  void setUniform(const char* name, const bool value);
  void setUniform(const char* name, const unsigned value);
  void setUniform(const char* name, const float value);
  void setUniform(const char* name, const vec3& value);
  void setUniform(const char* name, const vec4& value);
  void setUniform(const char* name, const mat4& matrix);

  unsigned getUniformLocation(const char* name);

  void setUniform(const unsigned loc, const mat4& matrix);
  static Shader& getMainShader(void) // Get the singleton instance
   {
    return *mainShader;
   }

 private:


  // Instance variables - private
  unsigned  shaderProgram;

};

#endif



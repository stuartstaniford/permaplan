// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef SHADER_H
#define SHADER_H

#include <cstdio>
#include <cglm/cglm.h>


// =======================================================================================
// Utility function

int checkGLError(FILE* file, const char* preamble);


// =======================================================================================
/// @brief Class that provides an abstraction for an openGL shader pipeline.

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
  /// @brief Get the singleton instance of the shader
  static Shader& getMainShader(void) {return *mainShader;}
  static void validateShader(char* warning);  // test accesssibility for debugging purposes

 private:


  // Instance variables - private
  unsigned  shaderProgram;

  // member functions - private
  Shader(const Shader&);                 // Prevent copy-construction
  Shader& operator=(const Shader&);      // Prevent assignment
};

#endif

// =======================================================================================



// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for an openGL shader pipeline.

#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include "loadFileToBuf.h"
#include "Shader.h"


// =======================================================================================
// Utility function to check openGL glGetError and report

int checkGLError(FILE* file, const char* preamble)
{
  GLenum glError;
  int errCount = 0;
  while( (glError = glGetError()))
   {
    char* errString;
    switch(glError)
     {
      case GL_INVALID_ENUM:
        errString = (char*)"INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        errString = (char*)"INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        errString = (char*)"INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        errString = (char*)"STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        errString = (char*)"STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        errString = (char*)"OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        errString = (char*)"INVALID_FRAMEBUFFER_OPERATION";
        break;
      case GL_CONTEXT_LOST:
        errString = (char*)"CONTEXT_LOST";
        break;
      case GL_TABLE_TOO_LARGE:
        errString = (char*)"TABLE_TOO_LARGE";
        break;
      default:
        errString = (char*)"UNKNOWN GL ERROR";
     }
    fprintf(file, "checkGLerror: %s: %s\n", preamble, errString);
    errCount++;
   }
  return errCount;
}


// =======================================================================================
// Utility function to load a shader from a file and compile it

unsigned loadAndCompileShader(const char* fileName, GLenum shaderType)
{
  unsigned size;
  
  // Load the file and create the shader
  char* glslSource  = loadFileToBuf(fileName, &size);
  unsigned shader   = glCreateShader(shaderType);
  
  // Compile the shader and handle compilation errors
  glShaderSource(shader, 1, (const GLchar**)&glslSource, NULL);
  glCompileShader(shader);
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success)
   {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    err(-1, "Compilation of GLSL file %s failed: %s\n", fileName, infoLog);
   }
  
  // Free resources no longer required and go home
  delete glslSource; glslSource = NULL;
  return shader;
}


// =======================================================================================
// Build a shader program from files

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
  // Get and compile the shader source
  unsigned vertexShader   = loadAndCompileShader(vertexShaderFile, GL_VERTEX_SHADER);
  unsigned fragmentShader = loadAndCompileShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
  
  // Build the program from the compiled shaders
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  
  // Handle linking errors
  int success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success)
   {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    err(-1, "Linking of GLSL files %s and %s failed: %s\n", vertexShaderFile,
        fragmentShaderFile, infoLog);
   }
  
  // Free resources no longer required and go home
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  if(checkGLError(stderr, "Shader constructor"))
    exit(-1);
  useProgram();
}

// =======================================================================================
// Destructor

Shader::~Shader(void)
{
  glDeleteProgram(shaderProgram);
}

// =======================================================================================
// Make us the active program for openGL

void Shader::useProgram(void)
{
  glUseProgram(shaderProgram);
  if(checkGLError(stderr, "glUseProgram(shaderProgram)"))
    exit(-1);
}


// =======================================================================================
// Functions to set various values in the shader.  Basically using C++ overloading to
// hide the ugliness of the openGL API.  These ones lookup by name.

// First is setting bools
void Shader::setUniform(const char* name, const bool value)
{
  glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

// Then setting unsigned
void Shader::setUniform(const char* name, const unsigned value)
{
  glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

// Setting a signle float
void Shader::setUniform(const char* name, const float value)
{
  glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

// Setting a single vec3
void Shader::setUniform(const char* name, const vec3& vector)
{
  glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, vector);
}

// Setting a single vec4
void Shader::setUniform(const char* name, const vec4& vector)
{
  glUniform4fv(glGetUniformLocation(shaderProgram, name), 1, vector);
}

// Setting a single mat4
void Shader::setUniform(const char* name, const mat4& matrix)
{
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, (float*)matrix);
}


// =======================================================================================
// Function to get the locations of various values in the shader and return the index
// for later use.

unsigned Shader::getUniformLocation(const char* name)
{
  return glGetUniformLocation(shaderProgram, name);
}


// =======================================================================================
// Functions to set various values in the shader.  Basically using C++ overloading to
// hide the ugliness of the openGL API.  These ones lookup by indexed previously returned
// from getUniform.

void Shader::setUniform(const unsigned loc, const mat4& matrix)
{
  glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)matrix);
}

// =======================================================================================

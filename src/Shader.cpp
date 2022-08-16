// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for an openGL shader pipeline.

#include "Shader.h"
#include "loadFileToBuf.h"
#include "Logging.h"
#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include <assert.h>


// =======================================================================================
// Static variables

Shader* Shader::mainShader = NULL;


// =======================================================================================
/// @brief Utility function to check openGL glGetError and report

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
/// @brief Utility function to load a shader from a file and compile it

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
/// @brief Constructor: build a shader program from files.
/// @param vertexShaderFile C string with the file name for the GLSL vertex shader file.
/// @param fragmentShaderFile C string with the file name for the GLSL fragment shader file.

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getMainShader()
  if(mainShader)
    return;
  else
    mainShader = this;

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
/// @brief Destructor

Shader::~Shader(void)
{
  glDeleteProgram(shaderProgram);
}


// =======================================================================================
/// @brief Test accesssibility of shader for debugging purposes
/// @param warning A C-string to give of the warning to give if this particular call fails.

void Shader::validateShader(char* warning)
{
  assert(mainShader);
  glUseProgram(mainShader->shaderProgram);  
  if(checkGLError(stderr, warning))
    exit(-1);
}


// =======================================================================================
/// @brief Make this shader the active program for openGL. 
/// 
/// This will be more useful if/when there is more than one shader.  Right now it just
/// needs to be called once near the beginning of the program. 

void Shader::useProgram(void)
{
  glUseProgram(shaderProgram);
  if(checkGLError(stderr, "glUseProgram(shaderProgram)"))
    exit(-1);
}


// =======================================================================================
// Functions to set various values in the shader.  Basically using C++ overloading to
// hide the ugliness of the openGL API.  These ones lookup by name.

/// @brief Set the value of a boolean shader uniform
/// @param name A C-string with the name of the uniform to set
/// @param value The boolean value to set.
void Shader::setUniform(const char* name, const bool value)
{
  glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

/// @brief Set the value of an unsigned shader uniform
/// @param name A C-string with the name of the uniform to set
/// @param value The unsigned value to set.
void Shader::setUniform(const char* name, const unsigned value)
{
  glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

/// @brief Set the value of a single float shader uniform
/// @param name A C-string with the name of the uniform to set
/// @param value The float value to set.
void Shader::setUniform(const char* name, const float value)
{
  glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

/// @brief Set the value of a vec3 shader uniform
/// @param name A C-string with the name of the uniform to set
/// @param value A reference to the vec3 value to set.
void Shader::setUniform(const char* name, const vec3& vector)
{
  glUniform3fv(glGetUniformLocation(shaderProgram, name), 1, vector);
}

/// @brief Set the value of a vec4 shader uniform
/// @param name A C-string with the name of the uniform to set
/// @param value A reference to the vec4 value to set.
void Shader::setUniform(const char* name, const vec4& vector)
{
  glUniform4fv(glGetUniformLocation(shaderProgram, name), 1, vector);
}

/// @brief Set the value of a mat4 shader uniform
/// @param name A C-string with the name of the uniform to set
/// @param value A reference to the mat4 value to set.
void Shader::setUniform(const char* name, const mat4& matrix)
{
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, (float*)matrix);
}


// =======================================================================================
/// @brief Function to get the locations of various values in the shader and return the 
/// index for later use.

unsigned Shader::getUniformLocation(const char* name)
{
  return glGetUniformLocation(shaderProgram, name);
}


// =======================================================================================
// Functions to set various values in the shader.  Basically using C++ overloading to
// hide the ugliness of the openGL API.  These ones lookup by indexed previously returned
// from getUniform.

/// @brief Set the value of a mat4 shader uniform
/// @param loc An unsigned with the location of the uniform to set (from 
/// getUniformLocation()).
/// @param value A reference to the mat4 value to set.
void Shader::setUniform(const unsigned loc, const mat4& matrix)
{
  glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)matrix);
}

// =======================================================================================

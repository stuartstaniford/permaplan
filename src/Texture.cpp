// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for openGL textures.

#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// =======================================================================================
// Load a texture from a file.

Texture::Texture(const char* fileName):
                            data(NULL)
{
  //fprintf(stderr, "File name of size %lu\n", strlen(fileName));
  //fprintf(stderr, "File name is %s\n",fileName);
  
  textureFileName = new char[strlen(fileName)+1];
  strcpy(textureFileName, fileName);
  stbi_set_flip_vertically_on_load(1);
  data = stbi_load(fileName, &width, &height, &nrChannels, 0);
  if(!data)
    err(-1, "Couldn't load texture file %s", fileName);
  if(4==nrChannels)
    format = GL_RGBA;
  else if(3==nrChannels)
    format = GL_RGB;
  else
    err(-1, "Unsupported %d channels in file %s", nrChannels ,fileName);
  if(width <=0 || height <= 0)
    err(-1, "Bad width,height: (%d,%d) in file %s", width, height ,fileName);
}


// =======================================================================================
// Destructor

Texture::~Texture(void)
{
  if(data)
    stbi_image_free(data);
  else
    glDeleteTextures(1, &textureId);
  delete[] textureFileName;
}


// =======================================================================================
// Create a texture from data (and set up the Mipmap).  See this link for background
// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml

void Texture::sendToGpu(void)
{
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  checkGLError(stderr, "get2DWrapTextureFromData glBindTexture");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  checkGLError(stderr, "get2DWrapTextureFromData GL_TEXTURE_WRAP_S");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  checkGLError(stderr, "get2DWrapTextureFromData GL_TEXTURE_MIN_FILTER");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  if(checkGLError(stderr, "get2DWrapTextureFromData glTexImage2D"))
   {
    int maxTexSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    fprintf(stderr, "Width: %d, Height:%d, GL_MAX_TEXTURE_SIZE: %d\n",
          width, height, maxTexSize);
    exit(-1);
   }
  glGenerateMipmap(GL_TEXTURE_2D);
  if(checkGLError(stderr, "get2DWrapTextureFromData glGenerateMipmap"))
    exit(-1);
  stbi_image_free(data);
  data = NULL;
}


// =======================================================================================
// Bind a particular texture to a given active texture unit and name in the shader

void Texture::bind(unsigned textureUnit, const char* name)
{
  glActiveTexture(GL_TEXTURE0+textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureId);
  if(checkGLError(stderr, "Texture::bind:glBindTexture"))
    exit(-1);
  Shader& shader = Shader::getMainShader();
  shader.setUniform(name, textureUnit);
  if(checkGLError(stderr, "Texture::bind:setUniform"))
    exit(-1);
}


// =======================================================================================

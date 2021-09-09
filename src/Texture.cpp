// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for openGL textures.

#include "Texture.h"
#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// =======================================================================================
/// @brief Constructor that loads a single texture from a file.
///
/// This function loads the texture into CPU memory (pointed to by the data pointer) but
/// does not send it to the GPU.  Uses the 
/// (STB Image library)[https://github.com/nothings/stb/blob/master/stb_image.h]
/// to do the actual loading. 
/// @param fileName A C string with the file name to load an image from.
/// @param flip A boolean for whether to flip the image vertically on loading.

Texture::Texture(const char* fileName, bool flip):
                            data(NULL)
{
  //fprintf(stderr, "File name of size %lu\n", strlen(fileName));
  //fprintf(stderr, "File name is %s\n",fileName);
  
  strncpy(textureFileName, fileName, TexPathLimit);
  stbi_set_flip_vertically_on_load((int)flip);
  data = stbi_load(fileName, (int*)&width, (int*)&height, &nrChannels, 0);
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
/// @brief Alternate constructor used for creating an empty texture (eg used by 
/// TextureAtlas)

Texture::Texture(void):
                    width(0u),
                    height(0u),
                    nrChannels(4),
                    format(GL_RGBA),
                    data(NULL)
{
}


// =======================================================================================
/// @brief Destructor.  
/// 
/// Deallocates the data buffer if it's in use, or else glDeletes the texture

Texture::~Texture(void)
{
  if(data)
    stbi_image_free(data);
  else
    glDeleteTextures(1, &textureId);
}


// =======================================================================================
/// @brief Create a texture from data (with mipmap) and send it to the GPU.
///
/// This will deallocate the data buffer (so we no longer have the data in CPU memory
/// once we have it in GPU memory.  See (this link) 
/// [https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml]
/// for background.

void Texture::sendToGpu(void)
{
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  checkGLError(stderr, "get2DWrapTextureFromData glBindTexture");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  checkGLError(stderr, "get2DWrapTextureFromData GL_TEXTURE_WRAP_S");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
/// @brief Bind a particular texture to a given active texture unit and name in the shader
/// @param textureUnit
/// @param name

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

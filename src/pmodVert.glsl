//VERTEX SHADER
#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec2  texCoord;
out vec3  normal;
out vec4  color;
out vec3  fragPosition; // passing world space coords to frag shader

uniform mat4  model;
uniform mat4  view;
uniform mat4  projection;

void main()
{
  vec4 worldPos = model * vec4(aPos, 1.0);
  gl_Position   = projection*view*worldPos;
  fragPosition  = vec3(worldPos);
  texCoord      = aTexCoord;
  normal        = aNormal;
  color         = aColor;
}


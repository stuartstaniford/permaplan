//FRAGMENT SHADER
#version 410 core

// Our output to the screen window
out vec4  FragColor;

// Stuff coming from the vertex shader
in vec2   texCoord;
in vec3   normal;
in vec4   color;
in vec3   fragPosition;

// The main earth texture
uniform sampler2D earthTexture;


// Used for things like axes, grid lines, etc
uniform bool      fixedColor;
uniform bool      noTexColor;
uniform vec4      theColor;
uniform vec4      accentColor;

// Stuff related to the lighting model
uniform float     ambientStrength;
uniform vec3      sunPosition;
uniform vec3      sunColor;

void main()
{
  vec4 ambient      = vec4(ambientStrength * sunColor, 1.0);
  vec3 norm         = normalize(normal);
  vec4 objectColor;
  vec3 sunDir       = normalize(sunPosition - fragPosition);
  float diff        = max(dot(norm, sunDir), 0.0);
  vec4 diffuse      = vec4(diff * sunColor, 1.0);

  if(fixedColor)
   {
    // Just used a fixed color for everything here
    objectColor = theColor;
   }
  else
   {
    if(noTexColor)
     {
      objectColor = color;
     }
    else
     {
      // Earth texture, modified by lighting model and accentColor.  This is the
      // branch for actual scenery/objects in the scene.
      objectColor           = texture(earthTexture, texCoord);
     }
    FragColor         = (ambient+diffuse) * objectColor;
   }
}

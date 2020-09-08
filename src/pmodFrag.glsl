//FRAGMENT SHADER
#version 410 core

// Our output to the screen window
out vec4  FragColor;

// Stuff coming from the vertex shader
in vec2   texCoord;
in float  accent;

// The main earth texture
uniform sampler2D earthTexture;


// Used for things like axes, grid lines, etc
uniform bool      fixedColor;
uniform bool      noTexColor;
uniform vec4      theColor;
uniform vec4      accentColor;

void main()
{
  if(fixedColor)
   {
    // Just used a fixed color for everything here
    FragColor = theColor;
   }
  else
   {
    if(noTexColor)
     {
      // We use the tex coordinate and accent as a color vector.
      FragColor = vec4(texCoord.x, texCoord.y, accent, 1.0);
     }
    else
     {
      // Earth texture, modified by accentColor
      FragColor = (1.0f-accent)*texture(earthTexture, texCoord) + accent*accentColor;
     }
   }
}

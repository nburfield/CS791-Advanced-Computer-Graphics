#version 330

in vec2 teture0;
uniform sampler2D gColorMap;

out vec4 fragColor;

void main(void)
{
   fragColor = texture(gColorMap, teture0);

    if (fragColor.r >= 0.9 && fragColor.g >= 0.9 && fragColor.b >= 0.9)
    {           
      discard;                                                                    
    }       
}

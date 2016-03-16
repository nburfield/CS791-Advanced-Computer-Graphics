#version 330


uniform sampler2D gColorMap;

in vec2 TexCoord;
in vec3 Color;
out vec4 FragColor;
                                                                                    
void main()                                                                         
{
    vec4 c = texture(gColorMap, TexCoord);

    if (c.r >= 0.9 && c.g >= 0.9 && c.b >= 0.9) 
    {
      discard;
    }
    else
    {
      FragColor = c * vec4(Color, 0.0f);
    }
}

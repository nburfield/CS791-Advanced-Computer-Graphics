#version 330                                                                        
                                                                                    
uniform sampler2D gColorMap;                                                        

in vec4 pos;                                                                   
in vec2 TexCoord;                                                                   
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{
    vec4 normPos = pos;
    vec4 tempf = texture(gColorMap, TexCoord);                                     
                                                                                    
    if (tempf.r >= 0.9 && tempf.g >= 0.9 && tempf.b >= 0.9)
    {           
      discard;                                                                    
    }
    else
    {
      FragColor = tempf * normPos;
    }
}

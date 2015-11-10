#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;                                                       
                                                                                    
uniform mat4 gVP;                                                                   
uniform vec3 gCameraPos;                                                            
uniform float gBillboardSize;  
uniform float gDT;
uniform float gTime;

out vec2 TexCoord;                                                                  
                                                                                    
void main()                                                                         
{
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(gCameraPos - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) * gBillboardSize;                    
                                                                                    
    Pos -= right;
    Pos.x = Pos.x - gDT + gTime;
    gl_Position = gVP * vec4(Pos.x + gDT - gTime, Pos.y, Pos.z, 1.0);
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += gBillboardSize;                                                        
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y -= gBillboardSize;                                                        
    Pos += right;                                                                   
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += gBillboardSize;                                                        
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();                                                                 
}                                                                                   

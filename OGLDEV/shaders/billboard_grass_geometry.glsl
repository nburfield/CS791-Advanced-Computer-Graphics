#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;                                                       
                                                                                    
uniform mat4 gVP;                                                                   
uniform vec3 gCameraPos;                                                            
uniform float gBillboardSize;
uniform float gDT;
uniform float gTime;

out float one;
out vec2 TexCoord;                                                                  

void main()                                                                         
{
    float gTimeVal = gTime / (2 * 3.14);
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(gCameraPos - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) * gBillboardSize;
    vec3 original = Pos;                  
                                                                                    
    Pos -= right;                                                                   
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += gBillboardSize;
    Pos.z += abs(cos(gTimeVal - gDT + sin(original.x) + sin(original.z)));
    Pos.x += abs(sin(gTimeVal + cos(original.z)));
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();

    Pos.y -= gBillboardSize;
    Pos.z -= abs(5 * cos(gTimeVal + sin(original.x) + sin(original.z)));
    Pos.x -= abs(2 * sin(gTimeVal + cos(original.z)));
    Pos += right;                                                                   
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += gBillboardSize;
    Pos.z += abs(5 * cos(gTimeVal + sin(original.x) + sin(original.z)));
    Pos.x += abs(2 * sin(gTimeVal + cos(original.z))); 
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();                                                                 
}                                                                                   

#version 410 core
layout(location=0) in vec2 inPos; // NDC fullscreen quad
layout(location=1) in vec2 inUV; // [0,1]
out vec2 vUV;
void main()
{ 
    vUV = inUV; 
    gl_Position = vec4(inPos,0.0,1.0); 
}
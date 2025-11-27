#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inUV;
out vec2 vUV;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    vUV = inUV;
    gl_Position = projection * view * vec4(position, 1.0);
}
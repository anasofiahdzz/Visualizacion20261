
#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex;
out vec3 fragColor;
out vec2 coordTex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    coordTex = tex;
    fragColor = color;
    gl_Position = projection * view * model * vec4(position, 1.0);
}

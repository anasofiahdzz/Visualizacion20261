#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aTex;

out VS_OUT { 
    vec3 texCoord;
} vs_out;

uniform mat4 uMVP;

void main() {
    vs_out.texCoord = aTex;
    gl_Position = uMVP * vec4(aPos, 1.0);
}


// -------------------------------
// shaders/advect.frag
// -------------------------------
#version 410 core
in vec2 vUV; 
out vec2 outVel;
uniform sampler2D uVel; // RG16F current
uniform float uDt; // seconds
uniform vec2 uTexel; // 1/size


void main(){
vec2 v = texture(uVel, vUV).xy;
// backtrace
vec2 prevUV = vUV - uDt * v; // domain is [0,1]
prevUV = clamp(prevUV, 0.0, 1.0);
vec2 adv = texture(uVel, prevUV).xy;
outVel = adv;
}
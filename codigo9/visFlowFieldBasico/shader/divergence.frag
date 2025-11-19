// -------------------------------
// shaders/divergence.frag
// -------------------------------
#version 410 core
in vec2 vUV; out float outDiv;
uniform sampler2D uVel; // RG16F
uniform vec2 uTexel; // 1/size


void main(){
vec2 L = texture(uVel, vUV - vec2(uTexel.x,0)).xy;
vec2 R = texture(uVel, vUV + vec2(uTexel.x,0)).xy;
vec2 B = texture(uVel, vUV - vec2(0,uTexel.y)).xy;
vec2 T = texture(uVel, vUV + vec2(0,uTexel.y)).xy;
float div = 0.5*(R.x - L.x)/uTexel.x + 0.5*(T.y - B.y)/uTexel.y;
outDiv = div;
}
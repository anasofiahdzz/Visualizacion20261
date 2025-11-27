

// -------------------------------
// shaders/jacobi.frag
// -------------------------------
#version 410 core
in vec2 vUV; 
out float outPressure;
uniform sampler2D uPressure; // previous pressure
uniform sampler2D uDivergence; // divergence
uniform vec2 uTexel; // 1/size
uniform float alpha; // -h^2
uniform float rBeta; // 1/4 (inv beta)


void main(){
    float L = texture(uPressure, vUV - vec2(uTexel.x,0)).r;
    float R = texture(uPressure, vUV + vec2(uTexel.x,0)).r;
    float B = texture(uPressure, vUV - vec2(0,uTexel.y)).r;
    float T = texture(uPressure, vUV + vec2(0,uTexel.y)).r;
    float C = texture(uDivergence, vUV).r;
outPressure = (L + R + B + T + alpha * C) * rBeta;
}
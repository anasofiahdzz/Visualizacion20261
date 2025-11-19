#version 410 core
in vec2 vUV; 
out vec2 outVel;
uniform sampler2D uVel;
uniform sampler2D uPressure;
uniform vec2 uTexel;


void main()
{
    float L = texture(uPressure, vUV - vec2(uTexel.x,0)).r;
    float R = texture(uPressure, vUV + vec2(uTexel.x,0)).r;
    float B = texture(uPressure, vUV - vec2(0,uTexel.y)).r;
    float T = texture(uPressure, vUV + vec2(0,uTexel.y)).r;
    vec2 v = texture(uVel, vUV).xy;
    // subtract grad p
    v -= 0.5 * vec2(R - L, T - B) / uTexel; // scaled by grid spacing
    // optional damping
    v *= 0.999;
    outVel = v;
}

// -------------------------------
// shaders/render.frag
// -------------------------------
#version 410 core
in vec2 vUV; 
out vec4 FragColor;
uniform sampler2D uVel;


vec3 hsv2rgb(vec3 c){
vec3 p = abs(fract(c.xxx + vec3(0,1,2)/3.0)*6.0 - 3.0);
vec3 rgb = clamp(p - 1.0, 0.0, 1.0);
return c.z * mix(vec3(1.0), rgb, c.y);
}


void main(){
vec2 v = texture(uVel, vUV).xy;
float mag = length(v);
float ang = atan(v.y, v.x); // [-pi,pi]
float hue = (ang / 6.2831853) + 0.5; // [0,1]
vec3 col = hsv2rgb(vec3(hue, 0.8, clamp(mag*3.0, 0.0, 1.0)));
FragColor = vec4(col,1.0);


//FragColor = texture(uVel, vUV);
}
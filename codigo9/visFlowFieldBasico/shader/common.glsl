// -------------------------------
// shaders/common.glsl
// -------------------------------
// Value-noise 3D + curl noise in 2D (from 3D field)
#version 410 core


float rand3(vec3 p){ return fract(sin(dot(p, vec3(12.9898,78.233,45.164))) * 43758.5453); }


float noise3(vec3 p){
vec3 ip=floor(p); vec3 u=fract(p); u=u*u*(3.0-2.0*u);
float a=rand3(ip);
float b=rand3(ip+vec3(1,0,0));
float c=rand3(ip+vec3(0,1,0));
float d=rand3(ip+vec3(1,1,0));
float e=rand3(ip+vec3(0,0,1));
float f=rand3(ip+vec3(1,0,1));
float g=rand3(ip+vec3(0,1,1));
float h=rand3(ip+vec3(1,1,1));
float xy = mix(mix(a,b,u.x), mix(c,d,u.x), u.y);
float zw = mix(mix(e,f,u.x), mix(g,h,u.x), u.y);
return mix(xy, zw, u.z);
}


// Fractional Brownian Motion (fbm)
float fbm3(vec3 p){
float v=0.0; float amp=0.5; float freq=1.0;
for(int i=0;i<5;i++){ v += amp*noise3(p*freq); amp*=0.5; freq*=2.0; }
return v;
}


// Curl of 3D scalar field N packed as F=(Ny, Nz, Nx) to get a pseudo 2D incompressible field.
// Simpler: build a 3D vector field from three fbm channels and compute curl.
vec3 vecField(vec3 p){
return vec3( fbm3(p+vec3(13.1,0.0,0.0)),
fbm3(p+vec3(0.0,27.2,0.0)),
fbm3(p+vec3(0.0,0.0,39.3)) );
}


vec2 curlNoise2D(vec2 uv, float t, float scale, float eps){
vec3 p = vec3(uv*scale, t*0.1);
// finite differences for curl(F) = (dFz/dy - dFy/dz, dFx/dz - dFz/dx, dFy/dx - dFx/dy)
vec3 F = vecField(p);
vec3 Fdx = vecField(p+vec3(eps,0,0));
vec3 Fdy = vecField(p+vec3(0,eps,0));
vec3 Fdz = vecField(p+vec3(0,0,eps));
float dFzdy = (Fdy.z - F.z)/eps;
float dFydz = (Fdz.y - F.y)/eps;
float dFxdz = (Fdz.x - F.x)/eps;
float dFzdx = (Fdx.z - F.z)/eps;
// use x,y components of curl for a 2D field
return vec2(dFzdy - dFydz, dFxdz - dFzdx);
}


vec2 enforceBorders(vec2 vel, vec2 uv){
// Zero normal at borders (simple)
if(uv.x<=0.001) vel.x = max(vel.x, 0.0);
if(uv.x>=0.999) vel.x = min(vel.x, 0.0);
if(uv.y<=0.001) vel.y = max(vel.y, 0.0);
if(uv.y>=0.999) vel.y = min(vel.y, 0.0);
return vel;
}
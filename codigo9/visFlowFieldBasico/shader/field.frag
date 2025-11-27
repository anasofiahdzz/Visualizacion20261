//Vamos a crear un campo vectorial y lo guardaremos en una textura.
#version 410 core
in vec2 vUV;
out vec4 FragColor;

uniform float uTime;
uniform int uMode;
uniform float uScale;
uniform float uStrength;

vec3 hsvrgb(vec3 c){
    vec3 p = abs(fract(c.xxx + vec3 (0,1,2)/3.0)*6.0 - 3.0);
    vec3 rgb = clamp(p - 1.0, 0.0, 1.0);
    return c.z * mix (vec3(1.0), rgb, c.y);
}

float rand3(vec3 p){
    return fract(sin(dot(p,vec3(12.9898,78.233,45.164))) * 43758.5453);
}

float noise(vec3 p){
    vec3 ip = floor(p), u=fract(p);
    u = u * u *(3.0- 2.0 * u);
    float a = rand3(ip);
    float b = rand3(ip + vec3(1,0,0));
    float c = rand3(ip + vec3(0,1,0));
    float d = rand3(ip + vec3(1,1,0));
    float e = rand3(ip + vec3(0,0,1));
    float f = rand3(ip + vec3(1,0,1));
    float g = rand3(ip + vec3(0,1,1));
    float h = rand3(ip + vec3(1,1,1));

    float xy = mix(mix(a,b,u.x), mix(c,d,u.x), u.y);
    float zw = mix(mix(e,f,u.x), mix(g,h,u.x), u.y);

    return mix(xy,zw,u.z);
}

float fbm3(vec3 p){
    float v=0.0, amp=0.5, freq=1.0;

    for(int i = 0; i < 5; i++)
    {
        v += amp*noise(p*freq);
        amp*=0.5;
        freq*=2.0;
    }
    return v;
}

vec3 vecField(vec3 p){
    return vec3(
        fbm3(p + vec3(13.1,0.0,0.0)),
        fbm3(p + vec3(0.0,27.2,0.0)),
        fbm3(p + vec3(0.0,0.0,39.3))
    );
}

vec2 curlNoise2D(vec2 uv, float t, float scale, float eps){

    vec3 p = vec3(uv*scale, t*0.1);
    vec3 F = vecField(p);
    vec3 Fdx = vecField(p+vec3(eps, 0, 0));
    vec3 Fdy = vecField(p+vec3(0, eps, 0));
    vec3 Fdz = vecField(p+vec3(0, 0, eps));

    float dFzdy = (Fdy.z - F.z)/eps;
    float dFydz = (Fdy.y - F.y)/eps;
    float dFxdz = (Fdy.x - F.x)/eps;
    float dFzdx = (Fdy.z - F.z)/eps;

    return vec2(dFzdy - dFydz, dFxdz -dFzdx);
}

vec2 swirl(vec2 uv){
    vec2 c = uv - vec2(0.5);
    float r = length(c) + 1e-6;
    vec2 t = vec2(-c.y, c.x) / r;
    return t * exp(-3.0*r);
}

vec2 field(vec2 uv, float t){
    if(uMode == 0){
        return uStrength * swirl(uv);
    }else{
        vec2 f = curlNoise2D(uv, t, max(uScale, 0.1), 0.01);
        return uStrength * f;
    }
}

void main(){
    vec2 V = field(vUV, uTime);
    float mag = length(V);
    float ang = atan(V.y, V.x);
    float hue = ang / 6.2831853 + 0.5;
    vec3 rgb = hsvrgb(vec3(hue, 0.85, clamp(mag*3.0,0.0,1.0)));
    FragColor = vec4(rgb,1.0);
}
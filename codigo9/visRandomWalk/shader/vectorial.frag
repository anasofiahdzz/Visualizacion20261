#version 410 core
uniform float aTime;
uniform vec2  inicio;     // semilla/offset
in vec2  coordTex;        // UV en [0,1]
out vec4 FragColor;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 n){
    vec2 ip = floor(n);
    vec2 u  = fract(n);
    u = u*u*(3.0 - 2.0*u);
    float res = mix(
        mix(rand(ip),              rand(ip + vec2(1.0, 0.0)), u.x),
        mix(rand(ip + vec2(0.0,1.0)), rand(ip + vec2(1.0,1.0)), u.x),
        u.y);
    return res*res;
}

void main() {
    // Posición “aleatoria suave” distinta para X e Y (cambia con el tiempo)
    // Nota: usamos offsets distintos (constantes) para evitar correlación X=Y.
    
    
    float t = aTime * 0.6; // factor de velocidad
    float px = noise(inicio + vec2(t, 0.123)); // en [0,1]
    float py = noise(inicio + vec2(0.456, t)); // en [0,1]
    vec2 pos = vec2(px, py);

    // Opción A: píxel casi-exacto con tolerancia (epsilon en UV)
    float epsilon = 0.003; // ajusta para hacerlo más/menos estricto
    bool hit = (abs(coordTex.x - pos.x) < epsilon) &&
               (abs(coordTex.y - pos.y) < epsilon);

    // Opción B (recomendada): punto visible como “disco” con radio
    float radius = 0.01; // tamaño del punto
    float d = distance(coordTex, pos);
    float disk = 1.0 - smoothstep(radius, radius * 0.6, d); // borde suave

    // Elige una de las dos salidas:
    // 1) Exacto por epsilon:
    // vec3 col = hit ? vec3(1.0) : vec3(0.0);

    // 2) Punto/disco visible:
    vec3 col = vec3(disk);

    FragColor = vec4(col, 1.0);
}
#version 410 core

in VS_OUT {
    vec3 texCoord;  // entrada del rayo
} fs_in;

out vec4 FragColor;

uniform sampler3D uVolume;
uniform sampler2D uBackfaceTex;
uniform vec2 uViewportSize;

// Parámetros de raymarch
uniform float uStepSize;    // ej. 0.01
uniform float uDensity;     // factor de opacidad (ej. 0.1)

void main() {
    // Coordenadas de pantalla [0,1] para leer la textura de backfaces
    vec2 uv = gl_FragCoord.xy / uViewportSize;
    vec3 entry = fs_in.texCoord;
    vec3 exitPos = texture(uBackfaceTex, uv).rgb;

    // Si por alguna razón la salida no es válida (no se dibujó backface), descartamos
    if (exitPos == entry) {
        discard;
    }

    vec3 dir = exitPos - entry;
    float rayLength = length(dir);
    vec3 rayDir = normalize(dir);

    float t = 0.0;
    float maxT = rayLength;

    vec3 accumColor = vec3(0.0);
    float accumAlpha = 0.0;

    // Máximo número de pasos (constante para que el compilador no sufra)
    const int MAX_STEPS = 512;

    for (int i = 0; i < MAX_STEPS; ++i) {
        if (t > maxT || accumAlpha > 0.99)
            break;

        vec3 samplePos = entry + rayDir * t;

        // Por seguridad (aunque no debería salirse de [0,1])
        if (any(lessThan(samplePos, vec3(0.0))) ||
            any(greaterThan(samplePos, vec3(1.0)))) {
            break;
        }

        float density = texture(uVolume, samplePos).r; // 0..1

        if (density > 0.001) {
            // Color simple azulado en función de la densidad
            vec3 color = vec3(density * 0.2, density * 0.5, density);

            float alpha = density * uDensity;

            // Composición front-to-back
            float oneMinusAlpha = 1.0 - accumAlpha;
            accumColor += color * alpha * oneMinusAlpha;
            accumAlpha += alpha * oneMinusAlpha;
        }

        t += uStepSize;
    }

    FragColor = vec4(accumColor, accumAlpha);
}

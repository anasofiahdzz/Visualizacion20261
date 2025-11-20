#version 410 core

in VS_OUT {
    vec3 texCoord;
} fs_in;

out vec4 FragColor;

void main() {
    // Guardamos la posición de salida del rayo en el volumen (texCoord en [0,1]^3)
    FragColor = vec4(fs_in.texCoord, 1.0);
}

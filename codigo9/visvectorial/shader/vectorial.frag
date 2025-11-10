#version 410 core
in vec2 coordTex;
out vec4 FragColor;

float func(float x, float y){

    return exp( -x*x - y*y);

}   

vec2 gradianteFunc(float x,float y){

    return vec2(-2.0*x*exp(-x*x - y*y),-2.0*y*exp(-x*x - y*y));
}

vec3 arcoiris(float val){

    float dx = 0.8;
    float f = clamp(val,0,1);
    float g = (6.0 - 2.0*dx) * f + dx;
    vec3 color = vec3(1.0);
    color.r = max(0,(3.0- abs(g-4.0) - abs(g-5.0)) / 2.0);
    color.g = max(0,(4.0- abs(g-2.0) - abs(g-4.0)) / 2.0);
    color.b = max(0,(3.0- abs(g-1.0) - abs(g-2.0)) / 2.0);
    return color;
}

void main()
{
        // Función e^(-x*x-y*y)
        // Dominio Reales
        // Rango [0,1]

        //Definimos el dominio utilizando coordenadas de textura 
        //
        //1. (0,0) x (1,1) a (-1,-1) x (1,1)
        float x = coordTex.x;
        float y = coordTex.y;
        float x1 = 2.0 * x - 1.0;
        float y1 = 2.0 * y - 1.0;

        // Las coordenadas x1 y y1 están entre [-1,-1] y [1,1]
        
        //float val = func(x1, y1);
        //2. Obtenemos el gradiente de la función
        vec2 vector = gradianteFunc(x1,y1);

        //3. Calculamus su magnitud 
        float val = length(vector);

        //4. Pintamos la magnitud
        // Nos indica donde hay cambios Rojo mucho cambio Azul poco o nada
        vec3 col = arcoiris(val);
   
   FragColor = vec4(col, 1.0);
}
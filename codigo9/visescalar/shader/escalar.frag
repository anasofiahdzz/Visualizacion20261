#version 410 core
in vec2 coordTex;
out vec4 FragColor;


    int mandelbrot(vec2 c, float max_iter){

        int i = 0;

        vec2 z = vec2(0.0);

        while( length(z) <= 2.0 && (i < max_iter))
        {

          float a = z.x * z.x - z.y * z.y ;
          float b = 2.0 * z.x * z.y;

          z = vec2(a,b) + c;
          i++;
        }

        return i;
    }

float func(float x, float y){

    return exp( -x*x - y*y);

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
            // Conjunto de MandelBrot
        //1. (0,0) x (1,1) a (-2,-2) x (2,2)

        //float max_iterations = atime;
        float max_iterations = 100;
        float x = coordTex.x;
        float y = coordTex.y;

        // (0,1) -> (-2,2)
        float x1 = 2.0 * x - 1.0;
        float y1 = 2.0 * y - 1.0;

        
        //float iter = mandelbrot(vec2(x1,y1), max_iterations);
        float val = func(x1, y1);

        vec3 col = arcoiris(val);
    
   
   FragColor = vec4(col, 1.0);
}
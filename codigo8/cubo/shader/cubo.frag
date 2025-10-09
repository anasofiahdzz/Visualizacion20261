
#version 410 core
in vec3 fragColor;
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

void main()
{
            // Conjunto de MandelBrot
        //1. (0,0) x (1,1) a (-2,-2) x (2,2)

        //float max_iterations = atime;
        float max_iterations = 10;
        float x = coordTex.x;
        float y = coordTex.y;

        // (0,1) -> (-2,2)
        float x1 = (x - 0.5) * 4.0;
        float y1 = (y - 0.5) * 4.0;

        
        float iter = mandelbrot(vec2(x1,y1), max_iterations);

/*
 if(iter < max_iterations){
            iter = iter / max_iterations;
            FragColor = vec4(iter,0.0,0.0,1.0);
        }else

            FragColor = vec4(0.0,0.0,1.0,1.0);

   // color = vec4(coordTex, 0.0, 1.0);
*/
FragColor = vec4(coordTex, 0.0, 1.0);
}


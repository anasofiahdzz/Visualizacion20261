#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../header/model.h"
#include <string>
#include <vector>

class Ventana{
    public:
        GLuint width, height;
        GLFWwindow* window;
        float timeVal;
        glm::mat4 view, projection, modelmat;
        GLuint velPing, velPong, pressurePing, pressurePong, divergenceTex ;
        
        const int W=512,H=512;
        
        std::vector<GLhalf> zero ;
        GLuint fbo; 
        Shader *shader[7];
        Model *model;

        Ventana();
        Ventana(GLuint w, GLuint h);
        void initGLFW();
        void initGLEW();
        void initModels(Model* m);
        GLuint makeTex(int w,int h, GLint ifmt, GLenum fmt, GLenum type);
        void attach(GLuint fbo, GLuint tex);
        
        
        void initViewProyection();
        void render();
        void update();
        void idel();
        void finish();
};
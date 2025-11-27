#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../header/shader.h"

class Model{
public:

    Shader* shader;
    GLfloat vertices[20];
    GLuint indices[6];
    float angle, timeVal;
    glm::mat4 modelmat;
    GLuint VAO, VBO, EBO;

    Model();
    //Model(GLfloat* vertices, GLuint indices);

    void initModel();
    void renderModel(glm::mat4 view, glm::mat4 projection);
    void updateModel(float timeValue);
    void finish();


};
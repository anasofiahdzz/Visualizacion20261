#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#define PI 3.141592


// Vertex Shader source code (GLSL 4.10)
const char* vertexShaderSource = R"(
    #version 410 core
    layout (location = 0) in vec3 aPos;
    uniform float atime;
    out vec3 vertexColor;

    void main()
    {
          gl_Position = vec4(aPos.x,aPos.y,aPos.z, 1.0);
   //     gl_Position = vec4(
   //                     aPos.x * cos(atime) + aPos.y * sin(atime), 
   //                     aPos.y * cos(atime) - aPos.x * sin(atime), 
   //                     (1.0) * sin(atime) * 2.0, 
   //                     1.0);
        vertexColor = (aPos + 1.0) * 0.5;
    }
)";

// Fragment Shader source code (GLSL 4.10)
const char* fragmentShaderSource = R"(
    #version 410 core
    out vec4 FragColor;
    in vec3 vertexColor;
    uniform float atime;

    void main()
    {
        FragColor = vec4(vertexColor.r ,vertexColor.g,vertexColor.b, 1.0);
    }
)";


void crearCirculo(int tam, float radius, float* vertices)
{
    //centro del ciculo
    vertices[0] = 0.0f; //x
    vertices[1] = 0.0f; //y
    vertices[2] = 0.0f; //z

    for(int i = 0; i < tam + 1; i++)
    {
        float theta = (2.0f * PI)/tam;
        vertices[(i*3) + 0] = radius * cosf((theta) * i); //x
        vertices[(i*3) + 1] = radius * sinf((theta) * i); //y
        vertices[(i*3) + 2] = 0.0f; //z
    }
}


std::vector<float> crearCirculoVector(int tam, float radius)
{
    std::vector<float> vertices;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for(int i = 1; i < tam + 1; i++)
    {

        vertices.push_back(radius *cosf(((2.0f * PI)/tam) * i)); //x
        vertices.push_back(radius *sinf(((2.0f * PI)/tam) * i)); //y
        vertices.push_back(0.0f); //z

    }

    return vertices;
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version to 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Circulo", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Build and compile the vertex shader
    // vertexShader es un ID asociado al shader de vertices 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Build and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders to a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers and configure vertex attributes

    int tam = 11;
    float radio = 0.5;

    //CIRCULO
    float* vertices = (float*)malloc(sizeof(float) * 3 * (tam + 1)); // void*
    
    crearCirculo(tam,radio, &vertices[0]);
    //std::vector<float> vertices = crearCirculoVector(tam,radio);
    GLuint VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Definir el tamaño de los datos a enviar al buffer
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (tam + 1) * 3, &vertices[0], GL_STATIC_DRAW);

    // Definir la distribución de los datos que se enivarán
    // variable shader   layout (location = 0) in vec3 aPos;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);

    float time = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render

        //El fondo de nuestro visualizador
        glClearColor(0.0f, 0.3f, 0.8f, 1.0f);
        //Pintar del color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the model
        glUseProgram(shaderProgram);

        //Como podemos enviar información sin necesidad de utilizar un buffer
        GLuint uniformTime = glGetUniformLocation(shaderProgram, "atime");
        glUniform1f(uniformTime,time);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, tam+1);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    free( vertices);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

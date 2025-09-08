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
    // layout (location = 1) in vec3 aColor;
    uniform float atime;
    out vec3 vertexColor;

    void main()
    {
//        gl_Position = vec4(aPos.x + sin(atime),aPos.y,aPos.z, 1.0);
        gl_Position = vec4(
                        aPos.x , //* cos(atime) + aPos.y * sin(atime), 
                        aPos.y , //* cos(atime) - aPos.x * sin(atime), 
                        aPos.z, 
                        1.0);
        vertexColor = (aPos * cos(atime) + 1.0)*0.5;
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
        //FragColor = vec4(vertexColor.r ,vertexColor.g,vertexColor.b, 1.0);

        FragColor = vec4(gl_FragCoord.x / 800 ,gl_FragCoord.y / 600 ,0.0,1.0);
    }
)";


void crearCirculo(int tam, float radius, float* vertices)
{


    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;

    for(int i = 1; i < tam + 1; i++)
    {

    vertices[(i*tam)+0] = radius *cosf(((2.0f * PI)/tam) * i); //x
    vertices[(i*tam)+1] = radius *sinf(((2.0f * PI)/tam) * i); //y
    vertices[(i*tam)+2] = 0.0f; //z


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

std::vector<float> crearCuadrado(){

    
    std::vector<float> vertices;
    // v0
    vertices.push_back(1.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    // v1
    vertices.push_back(1.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);
    // v2
    vertices.push_back(-1.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    // v3
    vertices.push_back(-1.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
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
    /*float vertices[] = {
        -1.5f, -1.5f, 0.0f,
         1.5f, -1.5f, 0.0f,
         0.0f,  1.5f, 0.0f
    };*/

    /*float vertices[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         
         1.0f, -1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
         0.0f, -1.0f, 0.0f
    };*/

    float colors[] = {
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         
         1.0f, 1.0f, 0.0f,
         1.0f, 0.0f, 1.0f,
         0.0f, 1.0f, 1.0f
    };
    int tam = 1;
    float radio = 0.5;
    //float* vertices = new float[(tam + 1)*3];
    
    //crearCirculo(tam,radio,vertices);
    //std::vector<float> vertices = crearCirculoVector(tam,radio);
    std::vector<float> vertices = crearCuadrado();
    GLuint VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);


    GLuint VBOcolor, VAOcolor;
    
    glGenVertexArrays(1, &VAOcolor);
    glGenBuffers(1, &VBOcolor);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOcolor);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

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
        glClearColor(0.0f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);

        GLuint uniformTime = glGetUniformLocation(shaderProgram, "atime");
        glUniform1f(uniformTime,time);

       // glBindVertexArray(VAOcolor);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

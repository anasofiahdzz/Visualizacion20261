// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "shader.h"

const int WIDTH  = 800;
const int HEIGHT = 600;

// Carga volumen 3D RAW de uint8 (un canal)
bool loadVolume(const std::string& path,
                int dimX, int dimY, int dimZ,
                std::vector<unsigned char>& data) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir volumen: " << path << std::endl;
        return false;
    }
    data.resize(dimX * dimY * dimZ);
    file.read(reinterpret_cast<char*>(data.data()), data.size());
    if (!file) {
        std::cerr << "Error leyendo volumen (tamaño incorrecto?)" << std::endl;
        return false;
    }
    return true;
}

GLuint createVolumeTexture3D(const std::vector<unsigned char>& data,
                             int dimX, int dimY, int dimZ) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_3D, tex);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage3D(GL_TEXTURE_3D,
                 0,
                 GL_R8,            // formato interno
                 dimX, dimY, dimZ,
                 0,
                 GL_RED,           // formato de origen
                 GL_UNSIGNED_BYTE,
                 data.data());

    glBindTexture(GL_TEXTURE_3D, 0);
    return tex;
}

// Crea un cubo en [0,1]^3 con posiciones y texcoords iguales
void createCube(GLuint& vao, GLuint& vbo, GLuint& ebo) {
    // pos(x,y,z), tex(x,y,z)
    float vertices[] = {
        // x, y, z,   s, t, r
        // Frente (z = 1)
        0.f, 0.f, 1.f,  0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,  1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,  1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,  0.f, 1.f, 1.f,
        // Atrás (z = 0)
        0.f, 0.f, 0.f,  0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,  1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,  1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,  0.f, 1.f, 0.f,
    };

    unsigned int indices[] = {
        // Frente
        0, 1, 2,
        2, 3, 0,
        // Atrás
        5, 4, 7,
        7, 6, 5,
        // Izquierda
        4, 0, 3,
        3, 7, 4,
        // Derecha
        1, 5, 6,
        6, 2, 1,
        // Arriba
        3, 2, 6,
        6, 7, 3,
        // Abajo
        4, 5, 1,
        1, 0, 4
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // aTex
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

bool createBackfaceFBO(GLuint& fbo, GLuint& tex, GLuint& depthRbo, int width, int height) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
                 GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, tex, 0);

    glGenRenderbuffers(1, &depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthRbo);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FBO de backfaces no está completo!" << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Raycasting Volumetrico", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error creando ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error inicializando GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Cargar shaders
    GLuint backfaceProgram = createProgramFromFiles(
        "shaders/raycast.vert",
        "shaders/backface.frag"
    );

    GLuint raycastProgram = createProgramFromFiles(
        "shaders/raycast.vert",
        "shaders/raycast.frag"
    );

    // Crear cubo
    GLuint cubeVAO, cubeVBO, cubeEBO;
    createCube(cubeVAO, cubeVBO, cubeEBO);

    // Cargar volumen
    int dimX = 32, dimY = 32, dimZ = 32;
    std::vector<unsigned char> volumeData;
    if (!loadVolume("data/shell32_uint8.raw", dimX, dimY, dimZ, volumeData)) {
        std::cerr << "No se pudo cargar el volumen 3D" << std::endl;
        return -1;
    }

    GLuint volumeTex = createVolumeTexture3D(volumeData, dimX, dimY, dimZ);

    // FBO para backfaces
    GLuint backfaceFBO, backfaceTex, depthRbo;
    if (!createBackfaceFBO(backfaceFBO, backfaceTex, depthRbo, WIDTH, HEIGHT)) {
        return -1;
    }

    // Matrices de cámara
    glm::vec3 camPos   = glm::vec3(1.5f, 1.5f, 2.0f);
    glm::vec3 camTarget= glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 camUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                      (float)WIDTH / (float)HEIGHT,
                                      0.1f, 10.0f);

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        angle += 0.3f; // rotación lenta

        glm::mat4 view = glm::lookAt(camPos, camTarget, camUp);

        glm::mat4 model = glm::mat4(1.0f);
        // Rotamos el volumen alrededor de su centro (0.5,0.5,0.5)
        model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.5f, -0.5f, -0.5f));

        glm::mat4 mvp = proj * view * model;

        // ---------- PASO 1: render de backfaces al FBO ----------
        glBindFramebuffer(GL_FRAMEBUFFER, backfaceFBO);
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  // dibujamos las caras traseras
        glEnable(GL_DEPTH_TEST);

        glUseProgram(backfaceProgram);
        GLint locMVP = glGetUniformLocation(backfaceProgram, "uMVP");
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // ---------- PASO 2: raycasting sobre las caras frontales ----------
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glCullFace(GL_BACK);  // ahora caras frontales
        glEnable(GL_DEPTH_TEST);

        glUseProgram(raycastProgram);

        locMVP = glGetUniformLocation(raycastProgram, "uMVP");
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));

        // Volume texture (unit 0)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, volumeTex);
        glUniform1i(glGetUniformLocation(raycastProgram, "uVolume"), 0);

        // Backface texture (unit 1)
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, backfaceTex);
        glUniform1i(glGetUniformLocation(raycastProgram, "uBackfaceTex"), 1);

        // Otros uniforms
        glUniform2f(glGetUniformLocation(raycastProgram, "uViewportSize"),
                    (float)WIDTH, (float)HEIGHT);
        glUniform1f(glGetUniformLocation(raycastProgram, "uStepSize"), 0.01f);
        glUniform1f(glGetUniformLocation(raycastProgram, "uDensity"), 0.1f);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // Limpieza
    glDeleteTextures(1, &volumeTex);
    glDeleteTextures(1, &backfaceTex);
    glDeleteRenderbuffers(1, &depthRbo);
    glDeleteFramebuffers(1, &backfaceFBO);

    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteVertexArrays(1, &cubeVAO);

    glDeleteProgram(backfaceProgram);
    glDeleteProgram(raycastProgram);

    glfwTerminate();
    return 0;
}

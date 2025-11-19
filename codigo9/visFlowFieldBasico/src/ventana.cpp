#include "../header/ventana.h"
#include <iostream>
Ventana::Ventana(){
    width = 800;
    height = 600;
}

Ventana::Ventana(GLuint w, GLuint h){
    width = w;
    height = h;
    zero.assign(2*W*H, 0);
}


void Ventana::initGLFW()
{
        if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return ;
    }

    // Crear una ventana
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Plano", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);


}

    // Inicializar GLEW
void Ventana::initGLEW(){
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        return;
    }
    std::cout << "Inicialización Correcta de GLEW y GLFW" << std::endl;
    // Habilitar depth testing y face culling
    glEnable(GL_DEPTH_TEST);  // Prueba de profundidad
    glEnable(GL_CULL_FACE);   // Habilitar culling de caras
    glCullFace(GL_BACK);      // Culling de caras traseras
    glFrontFace(GL_CCW);      // Las caras frontales son las que tienen vértices en sentido antihorario

}

void Ventana::initModels(Model* m)
{
    model = m;
    model->initModel();
    shader[0] = new Shader("./shader/vectorial.vert","./shader/advect.frag");
    shader[1] = new Shader("./shader/vectorial.vert","./shader/add_forces.frag");
    shader[2] = new Shader("./shader/vectorial.vert","./shader/divergence.frag");
    shader[3] = new Shader("./shader/vectorial.vert","./shader/jacobi.frag");
    shader[4] = new Shader("./shader/vectorial.vert","./shader/project.frag");
    shader[5] = new Shader("./shader/vectorial.vert","./shader/render.frag");
    shader[6] = new Shader("./shader/vectorial.vert","./shader/field.frag");
    glGenFramebuffers(1,&fbo);
    velPing = makeTex(W,H, GL_RG16F, GL_RG, GL_HALF_FLOAT);
    velPong = makeTex(W,H, GL_RG16F, GL_RG, GL_HALF_FLOAT);
    pressurePing = makeTex(W,H, GL_R16F, GL_RED, GL_HALF_FLOAT);
    pressurePong = makeTex(W,H, GL_R16F, GL_RED, GL_HALF_FLOAT);
    divergenceTex= makeTex(W,H, GL_R16F, GL_RED, GL_HALF_FLOAT);
 
    glBindTexture(GL_TEXTURE_2D, velPing); 
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,W,H,GL_RG,GL_HALF_FLOAT, zero.data()); 
    glBindTexture(GL_TEXTURE_2D, velPong); 
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,W,H,GL_RG,GL_HALF_FLOAT, zero.data());
}

void Ventana::initViewProyection(){
     // Configurar matrices de transformación (model, view, projection)
    
    view = glm::lookAt(glm::vec3(0.0f,0.0f,1.5f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

}

GLuint Ventana::makeTex(int w,int h, GLint ifmt, GLenum fmt, GLenum type){
    GLuint t; 
    glGenTextures(1,&t); 
    glBindTexture(GL_TEXTURE_2D,t); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE); 
    glTexImage2D(GL_TEXTURE_2D,0,ifmt,w,h,0,fmt,type,nullptr); 
    return t; 
}

void Ventana::attach(GLuint fbo, GLuint tex){ 
    glBindFramebuffer(GL_FRAMEBUFFER,fbo); 
    glFramebufferTexture2D(GL_FRAMEBUFFER, 
        GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0); 
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
    { std::cerr<<"FBO incomplete"; exit(1);} 
}

void Ventana::render(){
double last = glfwGetTime();
glDisable(GL_DEPTH_TEST);
std::cout << W << "," << H << std::endl;
while (!glfwWindowShouldClose(window)) {
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

    double now = glfwGetTime(); 
    float dt = (float)(now-last); 
    last=now; 
    dt = std::min(dt, 1.0f/30.0f);

    glViewport(0,0,W,H);
    glBindFramebuffer(GL_FRAMEBUFFER,0); 
    int fbw,fbh; 
    glfwGetFramebufferSize(window,&fbw,&fbh); 
    glViewport(0,0,fbw,fbh); 
    glClear(GL_COLOR_BUFFER_BIT);
    shader[6]->use();
    glUniform1f(glGetUniformLocation(shader[6]->ID,"uTime"), (float)glfwGetTime());
    glUniform1i(glGetUniformLocation(shader[6]->ID,"uMode"), 1);
    glUniform1f(glGetUniformLocation(shader[6]->ID,"uScale"), 2.0f);
    glUniform1f(glGetUniformLocation(shader[6]->ID,"uStrength"), 1.0f);
    model->renderModel(shader[6]->ID,view,projection);
    glfwSwapBuffers(window); 
    glfwPollEvents();
        update();
    }


}
void Ventana::update(){
    // Calcular la rotación del cubo
    timeVal = glfwGetTime();
    model->updateModel(timeVal);
}
void Ventana::idel(){}
void Ventana::finish(){

    
        
    shader[0]->terminate();
    shader[1]->terminate();
    shader[2]->terminate();
    shader[3]->terminate();
    shader[4]->terminate();    
    shader[5]->terminate();   
    shader[6]->terminate();
    delete(shader[0]);
    delete(shader[1]);
    delete(shader[2]);
    delete(shader[3]);
    delete(shader[4]);
    delete(shader[5]);
    delete(shader[6]);
    model->finish();
}
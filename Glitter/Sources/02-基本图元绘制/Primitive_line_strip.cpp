

#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

static int SRC_W = 1280;
static int SRC_H = 800;

void initLib() {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow *createWindow() {
    // createWindow
    GLFWwindow *window = glfwCreateWindow(SRC_W, SRC_H, "基本图元绘制", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "创建窗口失败...\n");
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    return window;
}

void bindFunctionAddress() {
    if ( !gladLoadGL()) {
        fprintf(stderr, "函数地址绑定失败...\n");
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int VAO,VBO;

void preData() {
    // 顶点数据
    float vertices[] = {
        
        // 点
        0.0f,0.0f,0.0f,

        // 线带
        -0.9f, 0.2f, 0.0f,
        -0.6f, 0.8f, 0.0f,
        -0.3f,  -0.1f, 0.0f,
        0.0f, 0.1f, 0.0f,
        0.3f, 0.1f, 0.0f,
        0.5f, 0.9f, 0.0f,
        
        // 四边形
        -0.4,0.2,0.0,
        0.2,0.2,0,
        0.2,-0.3,0,
        -0.4,-0.3,0.0,
        
        // 线
        0.0f,0.0f,0.0f,
        0.5f,0.0f,0.0,
        0.5f,0.3f,0.0f,
        0.0f,0.3f,0.0f,
        
    };
    
    unsigned int VBO;
    glGenBuffers(1,&VBO);// 创建一个索引
    
    glGenVertexArrays(1,&VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW); // 将顶点数据放到GPU内存

    glBindVertexArray(VAO);
    //告诉OpenGL如何解析上面的内存
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    
}

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 inColor;\n"
"void main()\n"
"{\n"
"   FragColor = inColor;\n"
"}\n\0";

int shaderProgram;
void createShader() {
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void renderScreen() {
    
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    
    int location = glGetUniformLocation(shaderProgram,"inColor");
    
    // 画线
    glUniform4f(location,0.6f,0.3f,0.3f,1.0f);
    glDrawArrays(GL_LINES,11,4);
    
    
    // 画线带
    glUniform4f(location,1.0f,0.0f,0.0f,1.0f);
    glDrawArrays(GL_LINE_STRIP,1,6);

    // 画点 改变点的大小请看 14-几何着色器/Geometry.cpp
    glPointSize(80.0f);
    glUniform4f(location,0.0f,1.0f,0.0f,1.0f);
    glDrawArrays(GL_POINTS,0,1);
    
    // 画矩形线框
    glUniform4f(location,0.0f,0.0f,1.0f,1.0f);
    glDrawArrays(GL_LINE_LOOP,7,4);
    

    
}

int main(int argc, char * argv[]) {
    
    initLib();
    
    GLFWwindow *window = createWindow();
    if (window == nullptr) return -1;
    
    bindFunctionAddress();
    
    preData();
    createShader();
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    while (glfwWindowShouldClose(window) == false) {
        processInput(window);
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        renderScreen();
        
        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    
    return 0;
}



#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "filesystem.hpp"
#include "shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static int SCR_W = 1280;
static int SCR_H = 800;

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
    GLFWwindow *window = glfwCreateWindow(SCR_W, SCR_H, "通过GL_InstanceArray绘制", nullptr, nullptr);
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

void initBuffersData() {
    
    float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
        0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };

    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2 ) {
        for (int x = -10; x < 10; x += 2) {
            glm::vec2 translation;
            translation.x =  (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    GLuint instanceVBO;
    glGenBuffers(1,&instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(translations) ,translations,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2 * sizeof(float),(void *)0);
//    glVertexAttribDivisor(2, 1);

}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
int main(int argc, char * argv[]) {
#pragma clang diagnostic pop
    // 初始化第三方库
    initLib();
    
    // 创建窗口
    GLFWwindow *window = createWindow();
    if (window == nullptr) return -1;
    
    // 绑定OpenGL函数地址
    bindFunctionAddress();
    
    // 初始化顶点数据和buffer
    initBuffersData();
    
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // 创建着色器程序
    Shader shader(FileSystem::getGLSLPath("15-实例化/instanceArray/instance_array.vs").c_str(),
                  FileSystem::getGLSLPath("15-实例化/instanceArray/instance_array.fs").c_str());
    
    shader.use();
 
    while (glfwWindowShouldClose(window) == false) {
        
        processInput(window);
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT );
        
        shader.use();
        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES,0,6,100);
        glBindVertexArray(0);
        
        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    
    return 0;
}


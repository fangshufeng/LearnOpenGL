
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "filesystem.hpp"
#include "shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static int SCR_W = 1280;
static int SCR_H = 800;

unsigned int loadTexture(const std::string &path);

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
    GLFWwindow *window = glfwCreateWindow(SCR_W, SCR_H, "基本图元绘制", nullptr, nullptr);
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

unsigned int VAO,VBO, EBO;
void initBuffersData() {
    // 顶点数据
    float vertices[] = {
        // positions          // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
    };
    
//    float vertices[] = {
//        // positions          // texture coords
//        1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
//        1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
//        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left
//    };
    
    
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenBuffers(1,&VBO);// 创建一个索引
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW); // 将顶点数据放到GPU内存
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //告诉OpenGL如何解析上面的内存
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    
    
}

void renderScreen(GLFWwindow *window, Shader &shader,unsigned int texture0) {
    
    shader.use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture0);
    
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
    
//    projection = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f,0.0f),
//                           glm::vec4(0.0f,1.0f, 0.0f,0.0f),
//                           glm::vec4(0.0f, 0.0f,-0.02f,0.0f),
//                           glm::vec4(0.0f, 0.0f, -1.0f,1.0f));
    
//   projection =  glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    
//    template <typename T>
//    GLM_FUNC_QUALIFIER tmat4x4<T, defaultp> orthoRH
//    (
//     T left, T right,
//     T bottom, T top,
//     T zNear, T zFar
//     )
//    {
//        tmat4x4<T, defaultp> Result(1);
//        Result[0][0] = static_cast<T>(2) / (right - left);
//        Result[1][1] = static_cast<T>(2) / (top - bottom);
//        Result[3][0] = - (right + left) / (right - left);
//        Result[3][1] = - (top + bottom) / (top - bottom);
//
//#        if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
//        Result[2][2] = - static_cast<T>(1) / (zFar - zNear);
//        Result[3][2] = - zNear / (zFar - zNear);
//#        else
//        Result[2][2] = - static_cast<T>(2) / (zFar - zNear);
//        Result[3][2] = - (zFar + zNear) / (zFar - zNear);
//#        endif
//
//        return Result;
//    }

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    // Flip Buffers and Draw
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main(int argc, char * argv[]) {
    
    // 初始化第三方库
    initLib();
    
    // 创建窗口
    GLFWwindow *window = createWindow();
    if (window == nullptr) return -1;
    
    // 绑定OpenGL函数地址
    bindFunctionAddress();
    
    // 初始化顶点数据和buffer
    initBuffersData();
    
    // 创建着色器程序
    Shader ourShader(FileSystem::getGLSLPath("05-坐标系统/coordinate.vs").c_str(), FileSystem::getGLSLPath("05-坐标系统/coordinate.fs").c_str());
    
    // 加载纹理
    unsigned int awesonfaceTexture =  loadTexture(FileSystem::getTexturePath("awesomeface.png"));
    
    ourShader.use();
    ourShader.setInt("texture1", 0);
    
    while (glfwWindowShouldClose(window) == false) {
        processInput(window);
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        renderScreen(window,ourShader,awesonfaceTexture);
        
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    
    return 0;
}

unsigned int loadTexture(const std::string &path) {
    
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture); // 2d纹理
    
    // 设置纹理的环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    
    // 设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    // 读取图片到内存中
    
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 5.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
    
    
    int width,height,nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    
    // d绑定到buffer上
    if (data != nullptr) {
        GLenum format = GL_RGBA ;
        if (nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    
    stbi_image_free(data);
    
    return texture;
}


#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "filesystem.hpp"
#include "shader.hpp"
#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static int SCR_W = 1280;
static int SCR_H = 800;

float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

float lastX = SCR_W / 2.0f;
float lastY = SCR_H / 2.0f;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

unsigned int loadTexture(const std::string &path);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
    GLFWwindow *window = glfwCreateWindow(SCR_W, SCR_H, "渲染天空盒", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "创建窗口失败...\n");
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
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
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    
}

unsigned int VAO,VBO;

void initBuffersData() {
    
    float positions[] = {

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    
    glBufferData(GL_ARRAY_BUFFER,sizeof(positions) ,NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(positions),&positions);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
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
    Shader redShader(FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_red.vs").c_str(), FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_red.fs").c_str());
    Shader greenShader(FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_g.vs").c_str(), FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_g.fs").c_str());
    Shader blueShader(FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_b.vs").c_str(), FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_b.fs").c_str());
    Shader yellowShader(FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_y.vs").c_str(), FileSystem::getGLSLPath("13-高级数据和GLSL/uniform缓冲对象/uniform_y.fs").c_str());

    // 获取Uniform块索引
    GLuint rIndex =  glGetUniformBlockIndex(redShader.ID,"Matrices");
    GLuint gIndex =  glGetUniformBlockIndex(greenShader.ID,"Matrices");
    GLuint bIndex =  glGetUniformBlockIndex(blueShader.ID,"Matrices");
    GLuint yIndex =  glGetUniformBlockIndex(yellowShader.ID,"Matrices");
    
    // 着色器绑定点
    glUniformBlockBinding(redShader.ID,rIndex,0);
    glUniformBlockBinding(greenShader.ID,gIndex,0);
    glUniformBlockBinding(blueShader.ID,bIndex,0);
    glUniformBlockBinding(yellowShader.ID,yIndex,0);
    
    // 创建Uniform缓冲对象
    GLuint uboMatrices;
    glGenBuffers(1,&uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof( glm::mat4), NULL,GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    
    //绑定对象绑定到对应的点上
    glBindBufferRange(GL_UNIFORM_BUFFER,0,uboMatrices,0,2 * sizeof(glm::mat4));
    
    while (glfwWindowShouldClose(window) == false) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT );
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        
        glBindBuffer(GL_UNIFORM_BUFFER,uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(projection));
        glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER,0);
        
        redShader.use();
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
        redShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        
        // GREEN
        greenShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
        greenShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // YELLOW
        yellowShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
        yellowShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // BLUE
        blueShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
        blueShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
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


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
#pragma clang diagnostic pop
    
    glViewport(0, 0, width, height);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
#pragma clang diagnostic pop
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
#pragma clang diagnostic pop
    camera.ProcessMouseScroll(yoffset);
}

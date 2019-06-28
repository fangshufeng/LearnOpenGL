

//#include "glitter.hpp"

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
#include "model.h"
#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static int SCR_W = 1280;
static int SCR_H = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_W / 2.0;
float lastY = (float)SCR_H / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path, bool gammaCorrection);

void initLib() {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);
}

GLFWwindow *createWindow() {
    // createWindow
    GLFWwindow *window = glfwCreateWindow(SCR_W, SCR_H, "gamma", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "创建窗口失败...\n");
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    return window;
}

void bindFunctionAddress() {
    if ( !gladLoadGL()) {
        fprintf(stderr, "函数地址绑定失败...\n");
    }
}

bool gammaEnabled = false;
bool gammaKeyPressed = false;


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
    
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
    {
        gammaEnabled = !gammaEnabled;
        gammaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        gammaKeyPressed = false;
    }
    
}


unsigned int planeVAO, planeVBO;

void initBuffersData() {
    
    float planeVertices[] = {
        // positions            // normals         // texcoords
        10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

        10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };


    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
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
    
    glEnable(GL_DEPTH_TEST);

    Shader planeShader(FileSystem::getGLSLPath("19-gamma/gamma.vs").c_str(),
                      FileSystem::getGLSLPath("19-gamma/gamma.fs").c_str());
    
    GLuint gammaFloorTexture = loadTexture(FileSystem::getTexturePath("WX20190628-143516.png").c_str(),true);
    GLuint floorTexture = loadTexture(FileSystem::getTexturePath("WX20190628-143516.png").c_str(),false);
    
    planeShader.use();
    planeShader.setInt("material.diffuse", 0);
    
    glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);
    planeShader.setVec3("light.position", glm::vec3(0.0f,0.0f,1.0f));
    planeShader.setVec3("light.ambient", lightColor * glm::vec3(0.2));
    planeShader.setVec3("light.diffuse", lightColor * glm::vec3(0.5));
    planeShader.setVec3("light.specular", lightColor);
    
    planeShader.setVec3("material.specular", glm::vec3(0.5f));
    planeShader.setFloat("material.shiness", 1.0f);
    
    planeShader.setVec3("lightPos", glm::vec3(0.0f,0.0f,1.0f));
    
    while (glfwWindowShouldClose(window) == false) {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        planeShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        planeShader.setMat4("projection", projection);
        planeShader.setMat4("view", camera.GetViewMatrix());
        planeShader.setMat4("model", glm::mat4(1.0f));
        planeShader.setVec3("viewPos", camera.Position);
        planeShader.setInt("gamma", gammaEnabled);
        
        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE, gammaEnabled ? gammaFloorTexture : floorTexture);
        glBindTexture(GL_TEXTURE,  floorTexture);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);
        
        std::cout << (gammaEnabled ? "Gamma enabled" : "Gamma disabled") << std::endl;
        
        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    
    glfwTerminate();
    
    return 0;
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

unsigned int loadTexture(char const * path, bool gammaCorrection)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}

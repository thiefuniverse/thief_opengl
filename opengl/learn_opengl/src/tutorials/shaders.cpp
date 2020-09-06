#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    //   float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
    float vertices[] = {
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    // create a VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //  create a VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // copy vertices to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void * )0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void * )(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // create vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // define vertex shader src
    const char *vertex_shader_src = "#version 330 core \n\
    layout(location = 0) in vec3 aPos;\
    layout(location = 1) in vec3 aColor;\
    out vec3 ourColor;\
    void main() {\
        gl_Position = vec4(aPos, 1.0);\
        ourColor = aColor;\
    }";
    // compile vertex shader
    glShaderSource(vertexShader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertexShader);
    int suc;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &suc);
    if (!suc) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "error shader compile failed: " << infoLog << std::endl;
        return 1;
    }
    // define fragment shader src
    const char *fragment_shader_src = "#version 330 core \n\
    out vec4 FragColor;\
    in vec3 ourColor;\
    void main() {\
        FragColor = vec4(ourColor, 1.0);\
    }\
                                ";
    // create fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //  compile fragment shader
    glShaderSource(fragmentShader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &suc);
    if (!suc) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "error shader compile failed: " << infoLog << std::endl;
        return 1;
    }

    // create shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &suc);
    if (!suc) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "error shader link failed: " << infoLog << std::endl;
        return 1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // set wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int nrAttributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "nrAttributes: " << nrAttributes << std::endl;
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

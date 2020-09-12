#include "shader_helper.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

glm::mat4 view;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, ShaderHelper &shader) {
    static float cur_ratio = 0.2;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::cout << "up now" << std::endl;
        cur_ratio += 0.1;
        if (cur_ratio > 1.0) {
            cur_ratio = 1.0;
        }
        shader.setValue("ratio", cur_ratio);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::cout << "down now" << std::endl;
        cur_ratio -= 0.1;
        if (cur_ratio < 0) {
            cur_ratio = 0;
        }
        shader.setValue("ratio", cur_ratio);
    }
    float cameraSpeed = 2.5f * deltaTime;  // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // glfwWaitEventsTimeout(50);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

int main() {
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

    // load texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("res/image/container.jpg", &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        std::cout << "stbi load image error" << std::endl;
        return 1;
    }
    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // release image
    stbi_image_free(data);

    unsigned char *data2 = stbi_load("res/image/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data2 == nullptr) {
        std::cout << "stbi load image error" << std::endl;
        return 1;
    }
    unsigned int texture2;
    glGenTextures(1, &texture2);

    std::cout << "texture 1: " << texture2 << std::endl;
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    // release image
    stbi_image_free(data2);

    // float vertices[] = {0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.5f,  0.5f, 0.5f, -0.5f, 0.0f,
    //                     0.0f, 1.0f, 0.0f,  0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,
    //                     0.0f, 0.0f, -0.5f, 0.5f, 0.0f, 1.0f,  1.0f,  1.0f, 0.0f, 0.5f};
    float texture_offset = 0.0;
    float vertices[] = {0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1 - texture_offset, 1 - texture_offset,
                        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1 - texture_offset, texture_offset,
                        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, texture_offset,     texture_offset,
                        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, texture_offset,     1 - texture_offset};

    unsigned int indices[] = {

        // 注意索引从0开始!
        0,
        1,
        3,  // 第一个三角形
        1,
        2,
        3  // 第二个三角形
    };

    // create a VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    //  create a VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // create a EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // copy vertices to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), ( void * )0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), ( void * )(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), ( void * )(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    ShaderHelper sha;
    std::cout << "before shader" << std::endl;
    sha.addShaderByPath("shader/vertex3.glsl", VERTEX_SHADER);
    std::cout << "after vertex shader" << std::endl;
    sha.addShaderByPath("shader/fragment2.glsl", FRAGMENT_SHADER);
    std::cout << "after fragment shader" << std::endl;
    sha.linkShader();

    sha.use();
    sha.setValue("texture1", 0);
    sha.setValue("texture2", 1);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processInput(window, sha);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        sha.use();

        // set camera
        // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // glm::vec3 cameraRight = glm::normalize(glm::cross(up - cameraDirection));
        // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
        glBindVertexArray(VAO);
        // set mvp
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        float radius = 5.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        // glm::mat4 view = glm::mat4(1.0f);
        //// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
        int modelLoc = glGetUniformLocation(sha.getID(), "model");
        //        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        sha.setValue("model", model);

        int viewLoc = glGetUniformLocation(sha.getID(), "view");
        sha.setValue("view", view);
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectLoc = glGetUniformLocation(sha.getID(), "projection");
        sha.setValue("projection", projection);
        // glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

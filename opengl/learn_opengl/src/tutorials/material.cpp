#include "material.hpp"
#include "camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "shader_helper.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to
                     // the right so we initially rotate a bit to the left.
float pitch = 0.0f;

glm::mat4 view;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;

float ambientStrength = 0.3;
float specularStrength = 0.5;
int specularNum = 32;

Camera globalCamera(cameraPos, cameraUp, yaw, pitch);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    globalCamera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    globalCamera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window) {
    static float cur_ratio = 0.2;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 2.5f * deltaTime;  // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        globalCamera.ProcessKeyboard(Camera::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        globalCamera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        globalCamera.ProcessKeyboard(Camera::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        globalCamera.ProcessKeyboard(Camera::RIGHT, deltaTime);
    }
    // control light argument
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (ambientStrength < 1.0) {
            ambientStrength += 0.1;
        } else {
            ambientStrength = 0.9;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (ambientStrength > 0.0) {
            ambientStrength -= 0.1;
        } else {
            ambientStrength = 0.0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (specularStrength < 1.0) {
            specularStrength += 0.1;
        } else {
            specularStrength = 0.9;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        if (specularStrength > 0.0) {
            specularStrength -= 0.1;
        } else {
            specularStrength = 0.0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (specularNum <= 1024) {
            specularNum *= 2;
        } else {
            specularNum = 1024;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        if (specularNum > 1) {
            specularNum /= 2;
        } else {
            specularNum = 1;
        }
    }
    std::cout << "amb: " << ambientStrength << " specular: " << specularStrength << " specularNum: " << specularNum
              << std::endl;
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
    float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
                        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
                        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

                        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
                        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

                        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
                        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
                        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

                        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
                        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
                        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
                        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

                        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
                        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};
    ShaderHelper sha;
    sha.addShaderByPath("shader/vertex2_phong_light.glsl", VERTEX_SHADER);
    sha.addShaderByPath("shader/fragment1_material.glsl", FRAGMENT_SHADER);
    sha.linkShader();

    // lightShader
    ShaderHelper lightShader;
    lightShader.addShaderByPath("shader/vertex1_light.glsl", VERTEX_SHADER);
    lightShader.addShaderByPath("shader/fragment2_light.glsl", FRAGMENT_SHADER);
    lightShader.linkShader();

    // normal VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // normal VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void * )0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void * )(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // light VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void * )0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ( void * )(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // normal shader
        sha.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = globalCamera.GetViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), 640.0f / 480.0f, 0.1f, 100.0f);
        sha.setValue("model", model);
        sha.setValue("view", view);
        sha.setValue("projection", projection);
        glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
        sha.setValue("objectColor", objectColor);

        // light shader
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightModel = glm::rotate(lightModel, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // move light position
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        float timeValue = glfwGetTime();
        lightColor = glm::vec3(2.0f * cos(0.2 * timeValue), 5.0f * sin(0.5 * timeValue), 3.0f * sin(0.2 * timeValue));
        sha.setValue("lightColor", lightColor);
        // std::cout << "view: " << glm::to_string(view) << std::endl;
        // std::cout << "lightpos : " << glm::to_string(lightPos) << std::endl;
        // std::cout << "view * lightPos" << glm::to_string(view * glm::vec4(lightPos, 1.0)) << std::endl;
        // std::cout << "vec3" << glm::to_string(glm::vec3(view * glm::vec4(lightPos, 1.0))) << std::endl;
        // sha.setValue("lightPos", glm::vec3(view * glm::vec4(lightPos, 1.0)));
        sha.setValue("lightPos", lightPos);
        sha.setValue("viewPos", globalCamera.position_);
        sha.setMaterial("frag", ruby_material);
        sha.setLight("light", default_light);
        // sha.setValue("specularStrength", specularStrength);
        // sha.setValue("specularNum", specularNum);
        // sha.setValue("ambientStrength", ambientStrength);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.use();
        lightShader.setValue("model", lightModel);
        lightShader.setValue("view", view);
        lightShader.setValue("projection", projection);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

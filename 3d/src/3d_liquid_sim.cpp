#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "camera.h"
#include "shader.h"

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float dt = 0.0f;
float lastFrame = 0.0f;

void framebuffer_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

const float L =1.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D_FLUID_SIMULATION", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize Glad" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    float boxVertices[] = {
        // Bottom face
        -L, -L, -L,          L, -L, -L,
        L, -L, -L,           L, -L, L,
        L, -L, L,            -L, -L, L,
        -L, -L, L,           -L, -L, -L,
        // Top Face 
        -L, L, -L,           L, L, -L,
        L, L, -L,            L, L, L,
        L, L, L,             -L, L, L,
        -L, L, L,            -L, L, -L,
        // Vertical 
        -L, -L, -L,           -L, L, -L,
        L, -L, -L,            L, L, -L,
        L, -L, L,             L, L, L,
        -L, -L, L,            -L, L, L
    };

    Shader boxShader("shader.vert", "shader.frag");

    unsigned int boxVAO, boxVBO;
    glGenVertexArrays(1, &boxVAO);
    glGenBuffers(1, &boxVBO);

    glBindVertexArray(boxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float>(glfwGetTime());
        dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        boxShader.use();
        boxShader.setMat4("projection", projection);
        boxShader.setMat4("view", view);

        glBindVertexArray(boxVAO);
        glDrawArrays(GL_LINES, 0, 24);
        glBindVertexArray(0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FOWARD, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, dt);
    }
}

void framebuffer_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
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

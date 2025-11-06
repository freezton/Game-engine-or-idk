#include <iostream>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <math.h>

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define RELATIVE_RESOURCE_PATH "assets/"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FOVY 45.0f

Camera camera(
    glm::vec3(0.0f, 0.0f, 3.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    -90.0f,
    0.0f
);

glm::mat4 projection;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;

float lastX = WINDOW_WIDTH / 2;
float lastY = WINDOW_HEIGHT / 2;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(FOVY), (float)width / height, 0.1f, 100.0f);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

void drawGrid()
{
    
}

int main(void)
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "game", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    stbi_set_flip_vertically_on_load(true);

    Shader modelShader(RELATIVE_RESOURCE_PATH"shaders/vertex.glsl", RELATIVE_RESOURCE_PATH"shaders/fragment.glsl");
    Shader gridShader(RELATIVE_RESOURCE_PATH"shaders/grid_vertex.glsl", RELATIVE_RESOURCE_PATH"shaders/grid_fragment.glsl");

    // // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // // some textures
    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // int width, height, nrChannels;
    // unsigned char* data = stbi_load(RELATIVE_RESOURCE_PATH"textures/image.jpg", &width, &height, &nrChannels, 0);

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // align image rows to 1 byte (4 default)
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(data);
    
    Model cube(RELATIVE_RESOURCE_PATH"scenes/cube.obj");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glm::radians(-55.0), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(FOVY), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec3 lightPos = glm::vec3(5.0f, 4.0f, 5.0f);

    // ///////////////////////////////////////////////////////////////////

    // float lineVertices[] = {
    //      100, 0, 0,
    //     -100, 0, 0,
    //      100, 0, -1,
    //     -100, 0, -1,
    //      100, 0, 1,
    //     -100, 0, 1,
    // };
    // GLuint gVAO, gVBO;

    // glGenVertexArrays(1, &gVAO);
    // glBindVertexArray(gVAO);

    // glGenBuffers(1, &gVBO);
    // glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // glBindVertexArray(0); 
    // ///////////////////////////////////////////////////////////////////

    drawGrid();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        processInput(window); // add resize window callback

        ////////////////// render //////////////////
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "cColor");
        // glBindTexture(GL_TEXTURE_2D, texture);
        // glUniform3f(vertexColorLocation, r, g, b);

        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime()/3, glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        view = camera.GetViewMatrix();
        drawGrid(gridShader, model, view, projection, camera.Position);

        glm::mat4 transformModel = glm::translate(model, glm::vec3(0.0f, sin(glfwGetTime()) * 5, 0.0f));
        modelShader.use();

        modelShader.setMat4("model", glm::scale(transformModel, glm::vec3(1.5f)));
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);

        // GLuint lightLoc = glGetUniformLocation(shaderProgram.ID, "lightPos");
        // glUniform3fv(lightLoc, 1, glm::value_ptr(lightPos));

        modelShader.setVec3("viewPos", camera.Position);
        modelShader.setVec3("lightPos", lightPos);

        cube.Draw(modelShader);
        glBindVertexArray(0);
        ////////////////////////////////////////////

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
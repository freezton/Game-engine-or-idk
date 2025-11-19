#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

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
#include "utils/grid.h"

#include "coordinator.h"
#include "systems/render_system.h"
#include <memory>
#include <unordered_map>
#include <vector>

#include "components/components.h"

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

int getRandomNum(int min, int max)
{
    return std::rand() % (max - min) + min;
}

Coordinator coordinator;

int main(void)
{
    std::srand(std::time({}));
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

    ShaderProgram modelShader;
    modelShader.attachShader(GL_VERTEX_SHADER, "model.vert");
    modelShader.attachShader(GL_FRAGMENT_SHADER, "model.frag");
    modelShader.link();
    
    ShaderProgram gridShader;
    gridShader.attachShader(GL_VERTEX_SHADER, "grid.vert");
    gridShader.attachShader(GL_FRAGMENT_SHADER, "grid.frag");
    gridShader.link();

    glm::vec3 lightPos = glm::vec3(5.0f, 4.0f, 5.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    coordinator.Init();
    coordinator.RegisterComponent<Transform>();
    coordinator.RegisterComponent<ModelComponent>();
    coordinator.RegisterComponent<UniformCallback>();

    auto renderSystem = coordinator.RegisterSystem<RenderSystem>();

    std::vector<Entity> entities;
    std::unordered_map<Entity, glm::vec3> positions;
    int count = getRandomNum(10, 100);
    for (int i = 0; i < count; i++)
    {
        Entity e = coordinator.CreateEntity();
        int d = 30;
        float x = (float)(getRandomNum(0, d)) - d/2.0f;
        float y = (float)(getRandomNum(0, d));
        float z = (float)(getRandomNum(0, d)) - d/2.0f;
        positions[e] = glm::vec3(x,y,z);

        coordinator.AddComponent<Transform>(e, Transform{ 
            glm::vec3(x, y, z), glm::vec3(0), glm::vec3(1.0f) 
        });
        coordinator.AddComponent<ModelComponent>(e, ModelComponent{ 
            std::make_shared<Model>(RELATIVE_RESOURCE_PATH"scenes/cube.obj") 
        });
        coordinator.AddComponent<UniformCallback>(e, UniformCallback{
            [&](ShaderProgram& shader) {
                shader.setMat4("view", view);
                shader.setMat4("projection", projection);
                shader.setVec3("viewPos", camera.Position);
                shader.setVec3("lightPos", lightPos);
            }
        });
        entities.push_back(e);
    }

    // Entity e = coordinator.CreateEntity();
    // coordinator.AddComponent<Transform>(e, Transform{ glm::vec3(0), glm::vec3(0), glm::vec3(0.1f) });
    // coordinator.AddComponent<ModelComponent>(e, ModelComponent{ std::make_shared<Model>(RELATIVE_RESOURCE_PATH"scenes/alien.obj") });
    // coordinator.AddComponent<UniformCallback>(e, UniformCallback{
    //     [&](ShaderProgram& shader) {
    //         shader.setMat4("view", view);
    //         shader.setMat4("projection", projection);
    //         shader.setVec3("viewPos", camera.Position);
    //         shader.setVec3("lightPos", lightPos);
    //     }
    // });

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



    // Model cube(RELATIVE_RESOURCE_PATH"scenes/alien.obj");

    // model = glm::rotate(model, (float)glm::radians(-55.0), glm::vec3(1.0f, 0.0f, 0.0f));


    projection = glm::perspective(glm::radians(FOVY), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////

    Grid grid;

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

        // glBindTexture(GL_TEXTURE_2D, texture);

        view = camera.GetViewMatrix();
        grid.Draw(gridShader, model, view, projection, camera.Position);

        modelShader.use();

        // modelShader.setMat4("model", glm::scale(model, glm::vec3(0.1f)));
        // modelShader.setMat4("view", view);
        // modelShader.setMat4("projection", projection);
        // modelShader.setVec3("viewPos", camera.Position);
        // modelShader.setVec3("lightPos", lightPos);

        // cube.Draw(modelShader);
        for (auto e: entities)
        {
            Transform& t = coordinator.GetComponent<Transform>(e);
            t.position.x = sin(glfwGetTime()) * positions[e].x;
        }
        
        renderSystem->Update(modelShader);

        glBindVertexArray(0);
        ////////////////////////////////////////////

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
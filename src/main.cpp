#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Shader.hpp"
#include "Graphics/Sprite.hpp"

#include "Managers/AssetManager.hpp"
#include "Managers/SpriteManager.hpp"

#include "World/TileMap.hpp"

using Clock = std::chrono::high_resolution_clock;
using TimeStamp = std::chrono::time_point<Clock>;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
bool IsKeyPressed(GLFWwindow* window, const int key);

glm::ivec2 screen_size = glm::ivec2(800, 600);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, "Platformer2D", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback( [](GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
        {
        fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                    type, severity, message );
        }, 0 );
#endif

    AssetManager al;
    SpriteManager sm;

    sm.createLinearAnimaton("Explosion", AssetManager::get<Texture2D>("Explosion.png"), 48, 30);
    sm.unloadOnGPU();

    Sprite sprite;
    sprite.setTexture(AssetManager::get<Texture2D>("Explosion.png")->texture);
    sprite.setFrame(0);

    Shader* shader = AssetManager::get<Shader>("TileMapShader", "tilemap.vert", "tilemap.frag");
    Shader::bind(shader);

    TileMap tm;
    tm.loadFromFile("TestMap.tmx");

    glm::mat4 projection(1.0f);
    projection = glm::ortho(0.0f, (float)screen_size.x, (float)screen_size.y, 0.0f, -1.0f, 1.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view,glm::vec3(0, 0, 0) );

    GLint projLoc = shader->getUniformLocation("projection");
    GLint viewLoc = shader->getUniformLocation("view");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int counter = 0;
    int frameNum = 0;

    TimeStamp timestamp = Clock::now();

    while (!glfwWindowShouldClose(window))
    {
        const auto dt = Clock::now() - timestamp;

        if (dt < std::chrono::milliseconds(10)) 
        { 
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        timestamp = Clock::now();

        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(IsKeyPressed(window, GLFW_KEY_A))
            view = glm::translate(view,glm::vec3(3, 0, 0) );

        if(IsKeyPressed(window, GLFW_KEY_D))
            view = glm::translate(view,glm::vec3(-3, 0, 0) );

        if(IsKeyPressed(window, GLFW_KEY_W))
            view = glm::translate(view,glm::vec3(0, 3, 0) );

        if(IsKeyPressed(window, GLFW_KEY_S))
            view = glm::translate(view,glm::vec3(0, -3, 0) );

        Shader::bind(shader);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        tm.draw();

        if(++counter > 2)
        {
            counter  = 0;
            frameNum++;

            if(frameNum >= 48)
                frameNum = 0;

            sprite.setFrame(frameNum);
        }

        sm.bind();
        sprite.draw();
        sm.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();      
    }

    glfwTerminate();
    return 0;
}

bool IsKeyPressed(GLFWwindow* window, const int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS) ? true : false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    screen_size = glm::ivec2(width, height);
    glViewport(0, 0, width, height);
}

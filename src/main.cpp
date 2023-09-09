#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Primitives/Sprite.hpp"

#include "Managers/AssetManager.hpp"
#include "Managers/SpriteManager.hpp"
#include "Managers/TileMapManager.hpp"

#include "Graphics/Transform2D.hpp"

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

    Renderer renderer;

    AssetManager al;
    SpriteManager sm;
    TileMapManager tm;

    sm.createLinearAnimaton("Explosion", AssetManager::get<Texture2D>("Explosion.png"), 48, 30);
    sm.unloadOnGPU();

    auto tmp = tm.loadFromFile("TestMap.tmx");

    Sprite sprite;
    sprite.setTexture(AssetManager::get<Texture2D>("Explosion.png")->texture);
    sprite.setFrame(0);

    Shader* tilemapShader = AssetManager::get<Shader>("TileMap", "TileMap.vert", "TileMap.frag");
    Shader::bind(tilemapShader);
    
    int ViewProjection = tilemapShader->getUniformLocation("ViewProjection");

    glm::mat4 projection = glm::ortho(0.0f, (float)screen_size.x, (float)screen_size.y, 0.0f, -1.0f, 1.0f);
    glm::mat4 view(glm::identity<glm::mat4>());

    glUniformMatrix4fv(ViewProjection, 1, GL_FALSE, glm::value_ptr(projection * view));

    Shader* spriteShader = AssetManager::get<Shader>("SpriteShader", "Sprite.vert", "Sprite.frag");
    Shader::bind(spriteShader);

    int ModelViewProjectionLoc = spriteShader->getUniformLocation("ModelViewProjection");

    int counter = 0;
    int frameNum = 0;
    int angle = 0;

    TimeStamp timestamp = Clock::now();

    Transform2D model;

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

        const glm::mat4 viewProjMat { projection * view }; 

        Shader::bind(tilemapShader);
        glUniformMatrix4fv(ViewProjection, 1, GL_FALSE, glm::value_ptr(viewProjMat));
        tm.draw();

        if(++counter > 2)
        {
            counter  = 0;
            frameNum++;

            if(frameNum >= 48)
                frameNum = 0;

            sprite.setFrame(frameNum);
        }

        //model.setRotation(angle++);
        //model.move(1, 0);

        sm.bind();
        Shader::bind(spriteShader);
        glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(viewProjMat * model.getMatrix()));
        renderer.draw(sprite);
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

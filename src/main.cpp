#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Shader.hpp"
#include "graphics/2D/Transform2D.hpp"
#include "graphics/2D/Sprite2D.hpp"
#include "graphics/2D/Animator2D.hpp"

#include "managers/AssetManager.hpp"
#include "managers/Animation2DManager.hpp"
#include "managers/TileMapManager.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
bool IsKeyPressed(GLFWwindow* window, const int key);

glm::ivec2 screen_size = glm::ivec2(800, 600);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, "Renderer", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

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
    Animation2DManager sm;
    TileMapManager tm;
    Animator2D anim;

    sm.createLinearAnimaton("Explosion", AssetManager::get<Texture2D>("Explosion.png"), 48, 30);
    sm.unloadOnGPU();

    auto tmp = tm.loadFromFile("TestMap.tmx");

    Sprite2D sprite;
    sprite.setTexture(AssetManager::get<Texture2D>("Explosion.png")->texture);
    sprite.setFrame(0);

    anim.setSprite(&sprite);
    anim.setAnimation(sm.getAnimation("Explosion"));
    anim.loop(true);
    anim.reverse(false);
    anim.play();

    Shader* tilemapShader = AssetManager::get<Shader>("TileMap", "tilemap.vert", "tilemap.frag");
    Shader::bind(tilemapShader);
    
    int ViewProjection = tilemapShader->getUniformLocation("ViewProjection");

    glm::mat4 projection = glm::ortho(0.0f, (float)screen_size.x, (float)screen_size.y, 0.0f, -1.0f, 1.0f);
    Transform2D view;

    glUniformMatrix4fv(ViewProjection, 1, GL_FALSE, glm::value_ptr(projection * view.getMatrix()));

    Shader* spriteShader = AssetManager::get<Shader>("SpriteShader", "sprite.vert", "sprite.frag");
    Shader::bind(spriteShader);

    int ModelViewProjectionLoc = spriteShader->getUniformLocation("ModelViewProjection");

    int counter = 0;
    int frameNum = 0;
    int angle = 0;

    sprite.setOrigin(128, 128);
    sprite.setPosition(200, 50);
    //sprite.setScale(-1, 1);

    int posX = 0;
    int posY = 0;

    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();  

        float currentTime = static_cast<float>(glfwGetTime());
        float dt = currentTime - lastTime;
        lastTime = currentTime; 

        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(IsKeyPressed(window, GLFW_KEY_A))
            view.move(10, 0);

        if(IsKeyPressed(window, GLFW_KEY_D))
            view.move(-10, 0);

        if(IsKeyPressed(window, GLFW_KEY_W))
            view.move(0 , 10);

        if(IsKeyPressed(window, GLFW_KEY_S))
            view.move(0, -10);

        glm::mat4 viewProjMat { projection * view.getMatrix() }; 

        Shader::bind(tilemapShader);
        glUniformMatrix4fv(ViewProjection, 1, GL_FALSE, glm::value_ptr(viewProjMat));

        for(const auto& layer : tmp->getTileLayers())
            tmp->draw(layer);

        Shader::bind(nullptr);

        anim.update(dt);

        sm.bind();
        Shader::bind(spriteShader);


        glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(viewProjMat * sprite.getMatrix()));
        sprite.draw();

        Shader::bind(nullptr);
        sm.unbind();

        glfwSwapBuffers(window);    
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

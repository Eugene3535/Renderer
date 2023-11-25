#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Shader.hpp"
#include "graphics/Transform2D.hpp"
#include "graphics/Sprite2D.hpp"
#include "graphics/TiledMap.hpp"
#include "controllers/Animator.hpp"

#include "managers/AssetManager.hpp"
#include "managers/SpriteManager.hpp"
#include "managers/TiledMapManager.hpp"

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

    std::cout << glGetString(GL_VERSION) << '\n';

    AssetManager al;
    SpriteManager sm;
    TiledMapManager tm;
    Animator anim;

    AssetManager::get<Texture2D>("Explosion.png");

    sm.createLinearAnimaton("Explosion", AssetManager::get<Texture2D>("Explosion.png"), 48, 1000 / 30);
    sm.unloadOnGPU();

    auto tmp = tm.loadFromFile("TestMap.tmx");

    if(!tmp)
        return -1;

    if( ! anim.addAnimation("Explosion", sm.get<Animation>("Explosion"))) return -1;
    if( ! anim.setAnimation("Explosion"))return -1;
    anim.loop(true);
    anim.reverse(true);
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

    Transform2D trans;

    trans.setOrigin(128, 128);
    trans.setPosition(200, 50);
    //sprite.setScale(-1, 1);

    int posX = 0;
    int posY = 0;

    float ugol = 0.f;

    float lastTime = static_cast<float>(glfwGetTime());
    int dt = 0;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();  

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime; 

        std::cout << (int)currentTime << '\n';

        dt = static_cast<int>(deltaTime * 1000);

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

        for(const auto& layer : tmp->m_layers)
            tm.draw(layer);

        Shader::bind(nullptr);

        trans.setRotation(ugol);
        ugol += 2.5f;

        anim.update(dt);

        sm.bind(true);
        Shader::bind(spriteShader);

        glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(viewProjMat * trans.getMatrix()));
        sm.draw(*anim.getCurrentFrame());

        Shader::bind(nullptr);
        sm.bind(false);

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

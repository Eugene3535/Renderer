#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Loaders/AssetLoader.hpp"
#include "Loaders/TileMap.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

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

    Shader shader;
    auto res = shader.compile("res/shaders/shader.vert", "res/shaders/shader.frag");
    shader.bind();

    AssetLoader al;

    TileMap tm;
    tm.loadFromFile("Map1");

    glm::mat4 projection(1.0f);
    projection = glm::ortho(0.0f, (float)screen_size.x, (float)screen_size.y, 0.0f, -1.0f, 1.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view,glm::vec3(0, 0, 0) );

    shader.setUniform("projection", projection);
    shader.setUniform("view", view);



    while (!glfwWindowShouldClose(window))
    {
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

        shader.bind();

        shader.setUniform("view", view);

        tm.draw(shader.m_handle);

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

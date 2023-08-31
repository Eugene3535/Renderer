#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{
public:
    struct States
    {
        unsigned* pIndices = nullptr;

        unsigned texture = 0u;
        unsigned mode = 0u;

        int firstVertex = 0;
        int count = 0;
    };
    

public:
    Renderer() noexcept;
    ~Renderer();

    template<class Drawable>
    void draw(Drawable& object) noexcept
    {
        object.draw(this);
    }

    void drawStates(const States& states) noexcept;
};


#endif
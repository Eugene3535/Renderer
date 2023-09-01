#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer
{
public:
    Renderer() noexcept;
    ~Renderer();

    template<class Drawable>
    void draw(Drawable& object) noexcept
    {
        object.draw(this);
    }

    void drawPrimitives(unsigned texture, unsigned mode, int firstVertex, int count) const noexcept;
    void drawPrimitives(unsigned texture, unsigned mode, int count, int type, const unsigned* indices) const noexcept;
};


#endif
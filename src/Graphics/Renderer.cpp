#include <glad/glad.h>

#include "Graphics/Renderer.hpp"

Renderer::Renderer() noexcept
{
}

Renderer::~Renderer()
{
}

void Renderer::drawStates(const States& states) noexcept
{
    if (states.texture)
    {
        glBindTexture(GL_TEXTURE_2D, states.texture);

        if (states.pIndices)
        {
            glDrawElements(states.mode, states.count, GL_UNSIGNED_INT, states.pIndices);
        }
        else
        {
            glDrawArrays(states.mode, states.firstVertex, states.count);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
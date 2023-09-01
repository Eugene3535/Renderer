#include <glad/glad.h>

#include "Graphics/Renderer.hpp"

Renderer::Renderer() noexcept
{
}

Renderer::~Renderer()
{
}

void Renderer::drawPrimitives(unsigned texture, unsigned mode, int firstVertex, int count) const noexcept
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(mode, firstVertex, count);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::drawPrimitives(unsigned texture, unsigned mode, int count, int type, const unsigned* indices) const noexcept
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(mode, count, type, indices);
    glBindTexture(GL_TEXTURE_2D, 0);
}
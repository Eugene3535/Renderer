#ifndef SPRITE2D_HPP
#define SPRITE2D_HPP

/// A sprite is a lightweight class representing a minimal graphical element for rendering a rectangular shape with a texture in a game. 
/// It contains only identifiers of the texture and the rectangle from it. 
/// Sprite size, frame list are defined in structures of Animation2D type and stored in the animation manager. 
/// Mirroring mode, frame change, sprite size and so on are controlled by the Animator2D class.
/// Inheritance from the Transform2D class allows matrix transformations such as translation, rotation, and scaling to be applied to the sprite
///
/// Usage example:
/// \code
/// // Declare and load a texture
/// Texture2D* pTexture = AssetManager::get<Texture2D>("Sprite.png")
/// 
/// // Create a sprite
/// Sprite sprite;
/// sprite.setTexture(pTexture->texture);
/// sprite.setFrame(0);
/// sprite.setPosition(100, 25);
///
/// // Draw it
/// sprite.draw();
/// \endcode
///
/// \see AssetManager, Texture2D, Transform2D, Animation2DManager, Animation2D, Animator2D

#include "Graphics/2D/Transform2D.hpp"

class Sprite2D:
	public Transform2D
{
public:
	Sprite2D() noexcept;
	Sprite2D(glm::uint32_t texture) noexcept;
	Sprite2D(glm::uint32_t texture, glm::uint32_t frameNum) noexcept;
	~Sprite2D() = default;

	void setTexture(glm::uint32_t texture) noexcept;
	void setFrame(glm::uint32_t frameNum)  noexcept;

	glm::uint32_t getTexture()  const noexcept;
	glm::uint32_t getFrameNum() const noexcept;

	void draw() const noexcept;

private:
	glm::uint32_t m_texture;
	glm::uint32_t m_frame;
};

#endif 

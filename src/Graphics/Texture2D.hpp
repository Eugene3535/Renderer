#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include "system/NonCopyable.hpp"
#include "graphics/Image.hpp"

class Texture2D:
	private NonCopyable
{
public:
	Texture2D() noexcept;
	~Texture2D();

	bool loadFromFile(const std::string& filepath) noexcept;
	bool loadFromImage(const Image& image)         noexcept;

	void setSmooth(bool smooth)    noexcept;
	void setRepeated(bool repeate) noexcept;

	bool isSmooth()   const noexcept;
	bool isRepeated() const noexcept;

	unsigned getNativeHandle()    const noexcept;
	const glm::uvec2& getSize() const noexcept;

	static void bind(const Texture2D* texture) noexcept;

private:
	glm::uvec2 m_size;
	unsigned   m_texture;

	bool m_isSmooth;
	bool m_isRepeated;
};

#endif // !TEXTURE2D_HPP
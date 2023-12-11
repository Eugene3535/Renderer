#include <memory>

#include <glad/glad.h>

#include "rapidxml_ext.h"
#include "rapidxml_utils.hpp"

#include "system/FileProvider.hpp"
#include "graphics/Texture2D.hpp"
#include "graphics/Sprite2D.hpp"
#include "managers/SpriteManager.hpp"

SpriteManager::SpriteManager() noexcept:
	m_vao(0u), m_vbo(0u)
{
}

SpriteManager::~SpriteManager()
{
	reset();
}

bool SpriteManager::createFrame(const std::string& name, const Texture2D* texture, const glm::ivec4& frame) noexcept
{
	if (!texture)
		return false;

	auto it = m_animations.try_emplace(name);

	if(it.second == false)
		return false; // Already in the container

	auto& anim    = it.first->second;
	auto& sprites = m_sprites.emplace_back();
	auto  ratio   = 1.0f / glm::vec2(texture->getSize());
	anim.duration = 1;

	createSpriteFromFrame(frame, ratio, sprites, texture->getNativeHandle());
	anim.sprites = sprites.data();

	return true;
}

bool SpriteManager::createLinearAnimaton(const std::string& name, const Texture2D* texture, int duration, int delay) noexcept
{
	if (!texture)
		return false;

	auto it = m_animations.try_emplace(name);

	if(it.second == false)
		return false; // Already in the container

	auto& anim    = it.first->second;
	auto& sprites = m_sprites.emplace_back();
	anim.delay    = delay;
	anim.duration = duration;

	sprites.reserve(static_cast<std::size_t>(duration));

	auto size      = glm::vec2(texture->getSize());
	auto ratio     = 1.0f / size;
	int frameWidth = size.x / duration;

	for (int i = 0; i < duration; ++i)
		createSpriteFromFrame(glm::ivec4(i * frameWidth, 0, frameWidth, size.y), ratio, sprites, texture->getNativeHandle());

	anim.sprites = sprites.data();
	
    return true;
}

bool SpriteManager::createGridAnimaton(const std::string& name, const Texture2D* texture, int columns, int rows, int delay) noexcept
{
	if (!texture)
		return false;

	auto it = m_animations.try_emplace(name);

	if(it.second == false)
		return false; // Already in the container

	auto& anim    = it.first->second;
	auto& sprites = m_sprites.emplace_back();
	anim.delay    = delay;
	anim.duration = columns * rows;

	sprites.reserve(static_cast<std::size_t>(columns * rows));

	auto size       = glm::vec2(texture->getSize());
	auto ratio      = 1.0f / size;
	int frameWidth  = size.x / columns;
	int frameHeight = size.y / rows;

	for (int y = 0; y < rows; ++y)
		for (int x = 0; x < columns; ++x)	
			createSpriteFromFrame(glm::ivec4(x * frameWidth, y * frameHeight, frameWidth, frameHeight), ratio, sprites, texture->getNativeHandle());

	anim.sprites = sprites.data();

    return true;
}

bool SpriteManager::loadSpriteSheet(const std::string& filename, const Texture2D* texture) noexcept
{
	if (!texture)
		return false;

	if(auto found = m_spriteSheets.find(filename); found != m_spriteSheets.end())
		return true; // Already loaded

	const std::string filepath = FileProvider::getPathToFile(filename);

	if(filepath.empty())
		return false;

	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filepath.c_str());
	document->parse<0>(xmlFile.data());
	const auto spriteNode = document->first_node("sprites");

	auto ssIt = m_spriteSheets.try_emplace(filename);

	if( ! ssIt.second )
		return false;

	auto& spriteSheet = ssIt.first->second;

	auto ratio = 1.0f / glm::vec2(texture->getSize());

	for(auto animNode = spriteNode->first_node("animation");
		     animNode != nullptr;
		     animNode = animNode->next_sibling("animation"))
	{
		std::string title;

		if(auto ptr = animNode->first_attribute("title"); ptr != nullptr)
			title = ptr->value();

		if(title.empty())
			continue;

		if(auto it = m_animations.try_emplace(title); it.second)
		{
			auto delay = animNode->first_attribute("delay");

			auto& anim    = it.first->second;
			auto& sprites = m_sprites.emplace_back();
			anim.delay    = delay ? std::atoi(delay->value()): 0; 

			auto cutNode = animNode->first_node("cut");

			while (cutNode)
			{
				auto pX = cutNode->first_attribute("x");
				auto pY = cutNode->first_attribute("y");
				auto pW = cutNode->first_attribute("w");
				auto pH = cutNode->first_attribute("h");

				int x = pX ? atoi(pX->value()) : 0;
				int y = pY ? atoi(pY->value()) : 0;
				int w = pW ? atoi(pW->value()) : 0;
				int h = pH ? atoi(pH->value()) : 0;

				createSpriteFromFrame(glm::ivec4(x, y, w, h), ratio, sprites, texture->getNativeHandle());

				cutNode = cutNode->next_sibling();
			}

			anim.sprites = sprites.data();
			spriteSheet.emplace(title, anim);
		}
	}

    return true;
}

void SpriteManager::unloadOnGPU() noexcept
{
	if(m_vertexBuffer.empty())
		return;

	reset();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * m_vertexBuffer.size(), m_vertexBuffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_vertexBuffer.clear();
}

void SpriteManager::bind(bool on) noexcept
{
	glBindVertexArray(on ? m_vao : 0);
}

void SpriteManager::reset() noexcept
{
	if(m_vao)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0u;
	}

	if(m_vbo)
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0u;
	}
}

void SpriteManager::draw(const Sprite2D& sprite) const noexcept
{
	if(sprite.texture)
	{
		glBindTexture(GL_TEXTURE_2D, sprite.texture);
		glDrawArrays(GL_TRIANGLE_FAN, sprite.frame, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SpriteManager::createSpriteFromFrame(const glm::ivec4& frame, const glm::vec2& ratio, std::vector<Sprite2D>& sprites, unsigned texture) noexcept
{
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.emplace_back();

	auto number    = static_cast<unsigned>(m_vertexBuffer.size() - 4);
	auto& sprite   = sprites.emplace_back();
	sprite.frame   = number; // offset to first vertex of the quad
	sprite.texture = texture;
	sprite.width   = static_cast<unsigned>(frame.z);
	sprite.height  = static_cast<unsigned>(frame.w);

	Vertex2D* quad     = &m_vertexBuffer[number];
	quad[1].position.x = static_cast<float>(frame.z);
	quad[2].position.x = static_cast<float>(frame.z);
	quad[2].position.y = static_cast<float>(frame.w);
	quad[3].position.y = static_cast<float>(frame.w);

	float left   = frame.x * ratio.x;
	float top    = frame.y * ratio.y;
	float right  = (frame.x + frame.z) * ratio.x;
	float bottom = (frame.y + frame.w) * ratio.y;

	quad[0].texCoords.x = left;
	quad[0].texCoords.y = top;

	quad[1].texCoords.x = right;
	quad[1].texCoords.y = top;

	quad[2].texCoords.x = right;
	quad[2].texCoords.y = bottom;

	quad[3].texCoords.x = left;
	quad[3].texCoords.y = bottom;
}
#include <memory>

#include <glad/glad.h>

#include "rapidxml_ext.h"
#include "rapidxml_utils.hpp"

#include "utils/Files.hpp"
#include "graphics/2D/Texture2D.hpp"
#include "managers/Animation2DManager.hpp"

Animation2DManager::Animation2DManager() noexcept:
	m_vao(0u), 
	m_vbo(0u)
{
}

Animation2DManager::~Animation2DManager()
{
	release();
}

bool Animation2DManager::createFrame(const std::string& name, const Texture2D *pTexture, const glm::ivec4 &frame) noexcept
{
	if (!pTexture)
		return false;

	const auto pAnim = getAnimation(name);

	if (pAnim != nullptr)
		return true;

	auto it = m_animations.try_emplace(name);

	if( ! it.second )
		return false;

	auto ratio = 1.0f / glm::vec2(pTexture->size);

	auto& anim       = it.first->second;
	anim.pTexture    = pTexture;
	anim.pFrameSizes = &m_frameSizes;
	anim.startFrame  = m_vertexBuffer.size() >> 2; 
	anim.duration    = 1;

	createVerticesFromFrame(frame, ratio);

	return true;
}

bool Animation2DManager::createLinearAnimaton(const std::string &name, const Texture2D *pTexture, int duration, int fps, float delay) noexcept
{
	if (!pTexture)
		return false;

	const auto pAnim = getAnimation(name);

	if (pAnim)
		return true;

	auto it = m_animations.try_emplace(name);

	if (!it.second)
		return false;

	auto& anim       = it.first->second;
	anim.pTexture    = pTexture;
	anim.pFrameSizes = &m_frameSizes;
	anim.startFrame  = m_vertexBuffer.size() >> 2;
	anim.duration    = duration;
	anim.fps         = fps;
	anim.delay       = delay;

	int frameWidth = pTexture->size.x / duration;
	auto ratio = 1.0f / glm::vec2(pTexture->size);

	for (int i = 0; i < duration; ++i)
		createVerticesFromFrame(glm::ivec4(i * frameWidth, 0, frameWidth, pTexture->size.y), ratio);
	
    return true;
}

bool Animation2DManager::createGridAnimaton(const std::string &name, const Texture2D *pTexture, int columns, int rows, int fps, float delay) noexcept
{
	if(!pTexture)
		return false;

	const auto pAnim = getAnimation(name);

	if (pAnim)
		return true;

	auto it = m_animations.try_emplace(name);

	if (!it.second)
		return false;

	auto& anim       = it.first->second;
	anim.pTexture    = pTexture;
	anim.pFrameSizes = &m_frameSizes;
	anim.startFrame  = m_vertexBuffer.size() >> 2;
	anim.duration    = columns * rows;
	anim.fps         = fps;
	anim.delay       = delay;

	int  frameWidth  = pTexture->size.x / columns;
	int  frameHeight = pTexture->size.y / rows;
	auto ratio = 1.0f / glm::vec2(pTexture->size);

	for (int y = 0; y < rows; ++y)
		for (int x = 0; x < columns; ++x)	
			createVerticesFromFrame(glm::ivec4(x * frameWidth, y * frameHeight, frameWidth, frameHeight), ratio);

    return true;
}

bool Animation2DManager::loadSpriteSheet(const std::string &filename, const Texture2D *pTexture) noexcept
{
	if (!pTexture)
		return false;

	const auto already_loaded = getSpriteSheet(filename);

	if (already_loaded)
		return true;

	const std::string filepath = FileUtils::getPathToFile(filename);

	if(filepath.empty())
		return false;

	auto pDocument = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filepath.c_str());
	pDocument->parse<0>(xmlFile.data());
	const auto pSpritesNode = pDocument->first_node("sprites");

	auto ssIt = m_spriteSheets.try_emplace(filename);

	if( ! ssIt.second )
		return false;

	auto pSpriteSheet = &ssIt.first->second;

	auto ratio = 1.0f / glm::vec2(pTexture->size);

	for(auto pAnimNode = pSpritesNode->first_node("animation");
		     pAnimNode != nullptr;
		     pAnimNode = pAnimNode->next_sibling("animation"))
	{
		auto pTitle = pAnimNode->first_attribute("title");

		if(!pTitle)
			continue;

		std::string title = pTitle->value();

		auto it = m_animations.try_emplace(title);

		if( ! it.second )
			continue;

		auto pAnim  = &it.first->second;
		auto pDelay = pAnimNode->first_attribute("delay");

		pAnim->pTexture    = pTexture;
		pAnim->pFrameSizes = &m_frameSizes;
		pAnim->startFrame  = m_vertexBuffer.size() >> 2;
		pAnim->delay       = pDelay ? std::atoi(pDelay->value()) * 0.001f : 1.0f; // Convert milliseconds to seconds
		pAnim->fps         = 1.0f / pAnim->delay; // The number of frames shown per second

		auto pCutNode = pAnimNode->first_node("cut");

		while (pCutNode)
		{
			auto pX = pCutNode->first_attribute("x");
			auto pY = pCutNode->first_attribute("y");
			auto pW = pCutNode->first_attribute("w");
			auto pH = pCutNode->first_attribute("h");

			int x = pX ? atoi(pX->value()) : 0;
			int y = pY ? atoi(pY->value()) : 0;
			int w = pW ? atoi(pW->value()) : 0;
			int h = pH ? atoi(pH->value()) : 0;

			createVerticesFromFrame(glm::ivec4(x, y, w, h), ratio);
			pAnim->duration++;

			pCutNode = pCutNode->next_sibling();
		}

		pSpriteSheet->emplace(title, pAnim);
	}

    return true;
}

const Animation2DManager::Animation2D* Animation2DManager::getAnimation(const std::string &name) const noexcept
{
    auto found = m_animations.find(name);

    return (found != m_animations.end()) ? &found->second : nullptr;
}

const Animation2DManager::SpriteSheet* Animation2DManager::getSpriteSheet(const std::string &name) const noexcept
{
    auto found = m_spriteSheets.find(name);

    return (found != m_spriteSheets.end()) ? &found->second : nullptr;
}

void Animation2DManager::unloadOnGPU() noexcept
{
	if(m_vertexBuffer.empty())
		return;

	release();

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

void Animation2DManager::bind() noexcept
{
	if(m_vao)
		glBindVertexArray(m_vao);
}

void Animation2DManager::unbind() noexcept
{
	glBindVertexArray(0);
}

void Animation2DManager::release() noexcept
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

void Animation2DManager::createVerticesFromFrame(const glm::ivec4 &frame, const glm::vec2 &ratio) noexcept
{
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.emplace_back();
	m_vertexBuffer.emplace_back();
	m_frameSizes.emplace_back(frame.z, frame.w);

	Vertex2D* quad = &m_vertexBuffer[m_vertexBuffer.size() - 4];

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
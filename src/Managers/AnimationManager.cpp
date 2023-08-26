#include <memory>

#include <glad/glad.h>
#include "rapidxml_ext.h"
#include "rapidxml_utils.hpp"

#include "Managers/AnimationManager.hpp"
#include "Utils/Files.hpp"
#include "Graphics/Texture2D.hpp"
#include "Geometry/Vertex2D.hpp"

AnimationManager* AnimationManager::m_pInstance;

AnimationManager::AnimationManager() noexcept
{
	if (!m_pInstance)
		m_pInstance = this;
}

AnimationManager::~AnimationManager()
{
	for (const auto& [name, anim] : m_pInstance->m_animations)
	{
		glDeleteBuffers(1, &anim.vbo);
		glDeleteVertexArrays(1, &anim.vao);
	}

	for (const auto& [name, sprite_sheet] : m_pInstance->m_spriteSheets)
	{
		for (const auto& [name, anim] : sprite_sheet)
		{
			glDeleteBuffers(1, &anim->vbo);
			glDeleteVertexArrays(1, &anim->vao);
		}
	}

	m_pInstance->m_animations.clear();
    m_pInstance->m_spriteSheets.clear();
}

const Animation* AnimationManager::create(const char* name, const Texture2D* pTexture, const IntRect& rect) noexcept
{
	if (!m_pInstance || !pTexture)
		return nullptr;

	const auto pAnim = m_pInstance->getAnimation(name);

	if (pAnim)
		return pAnim;

	auto it = m_pInstance->m_animations.try_emplace(name);

	if( ! it.second )
		return nullptr;

	auto& anim = it.first->second;

	Vertex2D vertices[4]{};

	vertices[1].position.x = static_cast<float>(rect.width);
	vertices[2].position.x = static_cast<float>(rect.width);
	vertices[2].position.y = static_cast<float>(rect.height);
	vertices[3].position.y = static_cast<float>(rect.height);

	const auto& texSize = pTexture->getSize();
	float ratioX = 1.0f / texSize.x;
	float ratioY = 1.0f / texSize.y;

	float left   = rect.left * ratioX;
	float top    = rect.top * ratioY;
	float right  = (rect.left + rect.width) * ratioX;
	float bottom = (rect.top + rect.height) * ratioY;

	vertices[0].texCoords.x = left;
	vertices[0].texCoords.y = top;

	vertices[1].texCoords.x = right;
	vertices[1].texCoords.y = top;

	vertices[2].texCoords.x = right;
	vertices[2].texCoords.y = bottom;

	vertices[3].texCoords.x = left;
	vertices[3].texCoords.y = bottom;

	anim.pTexture = pTexture;
	anim.duration = 1;

	m_pInstance->unloadOnGPU(vertices, anim);

	return &anim;
}

const Animation* AnimationManager::create(const char* name, const Texture2D* pTexture, int duration, float fps, float delay) noexcept
{
	if (!m_pInstance || !pTexture)
		return nullptr;

	const auto pAnim = m_pInstance->getAnimation(name);

	if (pAnim)
		return pAnim;

	auto it = m_pInstance->m_animations.try_emplace(name);

	if (!it.second)
		return nullptr;

	auto& anim = it.first->second;

	std::vector<Vertex2D> vertices(static_cast<std::size_t>(duration * 4));

	const auto& texSize = pTexture->getSize();
	int frameWidth = texSize.x / duration;
	float ratioX = 1.0f / texSize.x;
	float ratioY = 1.0f / texSize.y;

	for (std::size_t i = 0; i < vertices.size(); i += 4)
	{
		Vertex2D* quad = &vertices[i];

		quad[1].position.x = static_cast<float>(frameWidth);
		quad[2].position.x = static_cast<float>(frameWidth);
		quad[2].position.y = static_cast<float>(texSize.y);
		quad[3].position.y = static_cast<float>(texSize.y);

		float left   = i * frameWidth * ratioX;
		float top    = 0.0f;
		float right  = (left + static_cast<float>(frameWidth)) * ratioX;
		float bottom = 1.0f;

		quad[0].texCoords.x = left;
		quad[0].texCoords.y = top;

		quad[1].texCoords.x = right;
		quad[1].texCoords.y = top;

		quad[2].texCoords.x = right;
		quad[2].texCoords.y = bottom;

		quad[3].texCoords.x = left;
		quad[3].texCoords.y = bottom;
	}

	anim.pTexture = pTexture;
	anim.duration = duration;
	anim.rate     = fps;
	anim.delay    = delay;

	m_pInstance->unloadOnGPU(vertices.data(), anim);

	return &anim;
}

const Animation* AnimationManager::create(const char* name, const Texture2D* pTexture, int columns, int rows, float fps, float delay) noexcept
{
	if (!m_pInstance || !pTexture)
		return nullptr;

	const auto pAnim = m_pInstance->getAnimation(name);

	if (pAnim)
		return pAnim;

	auto it = m_pInstance->m_animations.try_emplace(name);

	if (!it.second)
		return nullptr;

	auto& anim = it.first->second;

	std::vector<Vertex2D> vertices(static_cast<std::size_t>(columns * rows * 4));

	const auto& texSize = pTexture->getSize();

	int frameWidth  = texSize.x / columns;
	int frameHeight = texSize.y / rows;
	float ratioX = 1.0f / texSize.x;
	float ratioY = 1.0f / texSize.y;

	for (int y = 0; y < rows; ++y)
		for (int x = 0; x < columns; ++x)
		{
			Vertex2D* quad = &vertices[static_cast<std::size_t>(y * columns + x) * 4];

			quad[1].position.x = static_cast<float>(frameWidth);
			quad[2].position.x = static_cast<float>(frameWidth);
			quad[2].position.y = static_cast<float>(frameHeight);
			quad[3].position.y = static_cast<float>(frameHeight);

			float left   = x * frameWidth * ratioX;
			float top    = y * frameHeight * ratioY;
			float right  = (left + frameWidth) * ratioX;
			float bottom = (top + frameHeight) * ratioY;

			quad[0].texCoords.x = left;
			quad[0].texCoords.y = top;

			quad[1].texCoords.x = right;
			quad[1].texCoords.y = top;

			quad[2].texCoords.x = right;
			quad[2].texCoords.y = bottom;

			quad[3].texCoords.x = left;
			quad[3].texCoords.y = bottom;
		}

	anim.pTexture = pTexture;
	anim.duration = columns * rows;
	anim.rate     = fps;
	anim.delay    = delay;

	m_pInstance->unloadOnGPU(vertices.data(), anim);

	return &anim;
}

const AnimationManager::SpriteSheet* AnimationManager::loadSpriteSheet(const std::string& filename, const Texture2D* pTexture) noexcept
{
	if (!m_pInstance || !pTexture)
		return nullptr;

	const auto already_loaded = m_pInstance->getSpriteSheet(filename);

	if (already_loaded)
		return already_loaded;

	const std::string filepath = FileUtils::getPathToFile(filename);

	if(filepath.empty())
		return nullptr;

	auto pDocument = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filepath.c_str());
	pDocument->parse<0>(xmlFile.data());
	const auto pSpritesNode = pDocument->first_node("sprites");

	auto ssIt = m_pInstance->m_spriteSheets.try_emplace(filename);

	if( ! ssIt.second )
		return nullptr;

	auto pSpriteSheet = &ssIt.first->second;

	for(auto pAnimNode = pSpritesNode->first_node("animation");
		     pAnimNode != nullptr;
		     pAnimNode = pAnimNode->next_sibling("animation"))
	{
		auto pTitle = pAnimNode->first_attribute("title");

		if(!pTitle)
			continue;

		std::string title = pTitle->value();

		auto it = m_pInstance->m_animations.try_emplace(title);

		if( ! it.second )
			continue;

		auto pAnim  = &it.first->second;
		auto pDelay = pAnimNode->first_attribute("delay");

		pAnim->pTexture = pTexture;
		pAnim->delay    = pDelay ? std::atoi(pDelay->value()) * 0.001f : 1.0f; // Convert milliseconds to seconds
		pAnim->rate     = 1.0f / pAnim->delay; // The number of frames shown per second

		auto pCutNode = pAnimNode->first_node("cut");

		std::vector<IntRect> frames;

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

			frames.emplace_back(x, y, w, h);
			pAnim->duration++;

			pCutNode = pCutNode->next_sibling();
		}

		if (frames.empty())
			continue;

		std::vector<Vertex2D> vertices(frames.size() * 4);

		float ratioX = 1.0f / pTexture->getSize().x;
		float ratioY = 1.0f / pTexture->getSize().y;

		auto createVerticesFromFrame = [ratioX, ratioY, &vertices](const IntRect& frame, std::size_t stride)
		{
			Vertex2D* quad = &vertices[stride * 4];

			quad[1].position.x = static_cast<float>(frame.width);
			quad[2].position.x = static_cast<float>(frame.width);
			quad[2].position.y = static_cast<float>(frame.height);
			quad[3].position.y = static_cast<float>(frame.height);

			float left   = frame.left * ratioX;
			float top    = frame.top * ratioY;
			float right  = (frame.left + frame.width) * ratioX;
			float bottom = (frame.top + frame.height) * ratioY;

			quad[0].texCoords.x = left;
			quad[0].texCoords.y = top;

			quad[1].texCoords.x = right;
			quad[1].texCoords.y = top;

			quad[2].texCoords.x = right;
			quad[2].texCoords.y = bottom;

			quad[3].texCoords.x = left;
			quad[3].texCoords.y = bottom;
		};

		for (std::size_t i = 0; i < frames.size(); ++i)	
			createVerticesFromFrame(frames[i], i);
		
		unloadOnGPU(vertices.data(), *pAnim);

		pSpriteSheet->emplace(title, pAnim);
	}

	return pSpriteSheet;
}

const Animation* AnimationManager::getAnimation(const std::string& name) noexcept
{
    auto found = m_pInstance->m_animations.find(name);

    return (found != m_pInstance->m_animations.end()) ? &found->second : nullptr;
}

const AnimationManager::SpriteSheet* AnimationManager::getSpriteSheet(const std::string& name) noexcept
{
	auto found = m_pInstance->m_spriteSheets.find(name);

    return (found != m_pInstance->m_spriteSheets.end()) ? &found->second : nullptr;
}

void AnimationManager::unloadOnGPU(const Vertex2D* vertices, Animation& anim) noexcept
{
	glGenVertexArrays(1, &anim.vao);
	glGenBuffers(1, &anim.vbo);

	glBindVertexArray(anim.vao);

	glBindBuffer(GL_ARRAY_BUFFER, anim.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * anim.duration * 4, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, texCoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

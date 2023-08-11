#include "Managers/AnimationManager.hpp"
#include "Utils/Files.hpp"
#include "Graphics/Texture2D.hpp"

#include <glad/glad.h>

#include "rapidxml_ext.h"
#include "rapidxml_utils.hpp"

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

const Animation* AnimationManager::create(const char* name, const Texture2D* pTexture, const glm::ivec4& frame) noexcept
{
	if (!m_pInstance)
		return nullptr;

	const auto pAnim = m_pInstance->getAnimation(name);

	if (pAnim)
		return pAnim;

	auto it = m_pInstance->m_animations.try_emplace(name);

	if( ! it.second )
		return nullptr;

	auto& anim = it.first->second;
	auto& frames = m_frameList.emplace_back();
	frames.emplace_back(frame);

	anim.pTexture = pTexture;
	anim.pFrames  = frames.data();
	anim.duration = frames.size();

	return true;
}

const Animation* AnimationManager::create(const char* name, const Texture2D* pTexture, const glm::ivec4& startFrame, int duration, float fps, float delay) noexcept
{
	if (!m_pInstance)
		return nullptr;

	const auto pAnim = m_pInstance->getAnimation(name);

	if (pAnim)
		return pAnim;

	auto it = m_animMap.try_emplace(name);

	if( ! it.second )
		return false;

	auto& anim = it.first->second;
	auto& frames = m_frameList.emplace_back();
	frames.reserve(duration);

	for (int i = 0; i < duration; ++i)
		frames.emplace_back();

	anim.pTexture = pTexture;
	anim.pFrames  = frames.data();
	anim.duration = frames.size();
	anim.rate     = fps;
	anim.delay    = delay;

	return true;
}

const Animation* AnimationManager::create(const char* name, const Texture2D* pTexture, int duration, float fps, float delay) noexcept
{
	if (!m_pInstance)
		return nullptr;

	const auto pAnim = m_pInstance->getAnimation(name);

	if (pAnim)
		return pAnim;

	auto it = m_animMap.try_emplace(name);

	if( ! it.second )
		return false;

    auto& anim = it.first->second;
    auto& frames = m_frameList.emplace_back();
    frames.reserve(duration);

	auto tex_size = pTexture->getSize();
	int frame_width = tex_size.x / duration;

	for (int i = 0; i < duration; ++i)
		frames.emplace_back(i * frame_width, 0, frame_width, tex_size.y);

    anim.pTexture = pTexture;
	anim.pFrames  = frames.data();
	anim.duration = frames.size();
	anim.rate     = fps;
	anim.delay    = delay;

	return true;
}

const Animation* AnimationManager::create(const char* name, const Texture2D* pTexture, int rows, int columns, float fps, float delay) noexcept
{
	if (!m_pInstance)
		return nullptr;

	const auto pAnim = m_pInstance->getAnimation(name);

	if (pAnim)
		return pAnim;

	auto it = m_animMap.try_emplace(name);

	if( ! it.second )
		return false;

    auto& anim = it.first->second;
    auto& frames = m_frameList.emplace_back();
    std::size_t duration = rows * columns;
    frames.reserve(duration);

	auto tex_size = pTexture->getSize();
	int frame_width = tex_size.x / columns;
	int frame_height = tex_size.y / rows;

	for (int y = 0; y < rows; ++y)
		for (int x = 0; x < columns; ++x)
			frames.emplace_back(x * frame_width, y * frame_height, frame_width, frame_height);

    anim.pTexture = pTexture;
	anim.pFrames  = frames.data();
	anim.duration = frames.size();
	anim.rate     = fps;
	anim.delay    = delay;

	return true;
}

const AnimationManager::SpriteSheet* AnimationManager::loadSpriteSheet(const std::string& filename, const Texture2D* pTexture) noexcept
{
	if (!m_pInstance)
		return nullptr;

	const auto pSpriteSheet = m_pInstance->getSpriteSheet(name);

	if (pSpriteSheet)
		return pSpriteSheet;

	const std::string filepath = FileUtils::getPathToFile(filename, "animations");

	if(filepath.empty())
		return false;

	auto document = std::make_unique<rapidxml::xml_document<char>>();
	rapidxml::file<char> xmlFile(filepath.c_str());
	document->parse<0>(xmlFile.data());
	const auto pSpritesNode = document->first_node("sprites");

	auto ssIt = m_spriteSheets.try_emplace(filename);

	if( ! ssIt.second )
		return false;

	auto pSpriteSheet = &ssIt.first->second;

	for(auto pAnimNode = pSpritesNode->first_node("animation");
		     pAnimNode != nullptr;
		     pAnimNode = pAnimNode->next_sibling("animation"))
	{
		auto pTitle = pAnimNode->first_attribute("title");

		if(!pTitle)
			continue;

		std::string title = pTitle->value();

		auto it = m_animMap.try_emplace(title);

		if( ! it.second )
			continue;

		auto pAnim   = &it.first->second;
		auto pDelay = pAnimNode->first_attribute("delay");

		pAnim->pTexture = pTexture;
		pAnim->delay    = pDelay ? sf::milliseconds(std::atoi(pDelay->value())).asSeconds() : 0.16f;
		pAnim->rate     = 1.0f / pAnim->delay;

		auto pCutNode = pAnimNode->first_node("cut");

		auto& frames = m_frameList.emplace_back();

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

			pCutNode = pCutNode->next_sibling();
		}

		pAnim->duration = static_cast<int>(frames.size());
		pSpriteSheet->emplace(title, pAnim);
	}

	return !pSpriteSheet->empty();
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
#include "Managers/AnimationManager.hpp"
#include "Utils/Files.hpp"

#include "rapidxml_ext.h"
#include "rapidxml_utils.hpp"

AnimationManager::AnimationManager() noexcept
{
}

AnimationManager::~AnimationManager()
{
    m_animMap.clear();
    m_frameList.clear();
}

bool AnimationManager::create(const std::string& name, const sf::Texture* pTexture, const glm::ivec4& frame) noexcept
{
	auto it = m_animMap.try_emplace(name);

	if( ! it.second )
		return false;

	auto& anim = it.first->second;
	auto& frames = m_frameList.emplace_back();
	frames.emplace_back(frame);

	anim.pTexture = pTexture;
	anim.pFrames  = frames.data();
	anim.duration = frames.size();

	return true;
}

bool AnimationManager::create(const std::string& name, const sf::Texture* pTexture, const glm::ivec4& startFrame, int duration, float fps, float delay) noexcept
{
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

bool AnimationManager::create(const std::string& name, const sf::Texture* pTexture, int duration, float fps, float delay) noexcept
{
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

bool AnimationManager::create(const std::string& name, const sf::Texture* pTexture, int rows, int columns, float fps, float delay) noexcept
{
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

bool AnimationManager::loadSpriteSheet(const std::string& filename, const sf::Texture* pTexture) noexcept
{
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

const Animation* AnimationManager::getAnimation(const std::string& name) const noexcept
{
    auto found = m_animMap.find(name);

    return (found != m_animMap.end()) ? &found->second : nullptr;
}

const AnimationManager::SpriteSheet* AnimationManager::getSpriteSheet(const std::string& name) const noexcept
{
	auto found = m_spriteSheets.find(name);

    return (found != m_spriteSheets.end()) ? &found->second : nullptr;
}
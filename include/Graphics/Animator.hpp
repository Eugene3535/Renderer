#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include <unordered_map>

#include "Graphics/Sprite.hpp"
#include "Managers/Animation2DManager.hpp"

class Animator
{
public: 
	struct Status
	{
		bool isPlaying    = false;
		bool isLooped     = false;
		bool isOver       = false;
		bool isReversed   = false;
	};

public:
	Animator() noexcept;
	~Animator();

	void setSprite(Sprite* pSprite) noexcept;
	void setAnimation(const Animation2DManager::Animation2D* pAnimation) noexcept;

	const Sprite* getSprite()    const noexcept;
	const Animation2DManager::Animation2D* getAnimation() const noexcept;
	const Status* getStatus()    const noexcept;

	void update(float dt) noexcept;
	void stop()           noexcept;
	void play()           noexcept;
	void loop(bool l)     noexcept;
	void restart()        noexcept;
	void reverse(bool r)  noexcept;

private:
	Sprite* m_pSprite;
	const Animation2DManager::Animation2D* m_pCurrentAnimation;

	int   m_currentFrame;
	float m_timer;

    Status m_status;
};

#endif
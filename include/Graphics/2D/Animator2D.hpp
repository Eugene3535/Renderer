#ifndef ANIMATOR2D_HPP
#define ANIMATOR2D_HPP

#include <unordered_map>

#include "Graphics/2D/Sprite2D.hpp"
#include "Managers/Animation2DManager.hpp"

class Animator2D
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
	Animator2D() noexcept;
	~Animator2D();

	void setSprite(Sprite2D* pSprite) noexcept;
	void setAnimation(const Animation2DManager::Animation2D* pAnimation) noexcept;

	const Sprite2D* getSprite()    const noexcept;
	const Animation2DManager::Animation2D* getAnimation() const noexcept;
	const Status* getStatus()    const noexcept;

	void update(float dt) noexcept;
	void stop()           noexcept;
	void play()           noexcept;
	void loop(bool l)     noexcept;
	void restart()        noexcept;
	void reverse(bool r)  noexcept;

private:
	Sprite2D* m_pSprite;
	const Animation2DManager::Animation2D* m_pCurrentAnimation;

	int   m_currentFrame;
	float m_timer;

    Status m_status;
};

#endif
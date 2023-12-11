#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include <string>
#include <unordered_map>

#include "graphics/Animation.hpp"

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

	bool addAnimation(const std::string& name, const Animation& anim) noexcept;
	bool setAnimation(const std::string& name) noexcept;

	void update(int dt)  noexcept;
	void stop()          noexcept;
	void play()          noexcept;
	void loop(bool l)    noexcept;
	void restart()       noexcept;
	void reverse(bool r) noexcept;

	const Sprite2D*  getCurrentFrame() const noexcept;
	const Animation* getAnimation()    const noexcept;
	const Status*    getStatus()       const noexcept;

private:
	std::unordered_map<std::string, Animation> m_animations;

	Animation m_currentAnimation;
	int m_currentFrame;
	int m_timer;

	Status m_status;
};

#endif
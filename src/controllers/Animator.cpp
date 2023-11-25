#include "graphics/Texture2D.hpp"
#include "controllers/Animator.hpp"

Animator::Animator() noexcept:
    m_currentFrame(0),
    m_timer(0)
{
}

Animator::~Animator()
{
    m_animations.clear();
}

bool Animator::addAnimation(const std::string& name, const Animation& anim) noexcept
{
    return m_animations.try_emplace(name, anim).second;
}

bool Animator::setAnimation(const std::string &name) noexcept
{
    if(auto found = m_animations.find(name); found != m_animations.end())
    {
        if(m_currentAnimation != found->second)
        {
            restart();
            loop(false);
            reverse(false);
            play();
            m_currentAnimation = found->second;

            return true;
        }
    }

    return false;
}

void Animator::update(std::int32_t dt) noexcept
{
    bool needToPlay = (m_status.isPlaying && ( ! m_status.isOver ));

    if (needToPlay) 
    {
        m_timer += dt;

        if(m_timer > m_currentAnimation.delay)
        {
            m_currentFrame += m_status.isReversed ? -1 : 1;
            m_timer = 0;

            bool isOutOfRange = m_status.isReversed ? (m_currentFrame < 0) : (m_currentFrame >= m_currentAnimation.duration);

            if (isOutOfRange)
            { 
                if ( ! m_status.isLooped )
                {
                    m_status.isOver = true;
                    return;
                }
                restart();
            }
        }
    }
}

void Animator::stop() noexcept
{
    m_status.isPlaying = false;
}

void Animator::play() noexcept
{
    if( ! m_status.isOver )
        m_status.isPlaying = true;
}

void Animator::loop(bool b) noexcept
{
    m_status.isLooped = b;
}

void Animator::restart() noexcept
{
    m_currentFrame  = m_status.isReversed ? (m_currentAnimation.duration - 1) : 0;
    m_timer         = 0;
    m_status.isOver = false;
}

void Animator::reverse(bool r) noexcept
{
    m_status.isReversed = r;
}

const Sprite2D* Animator::getCurrentFrame() const noexcept
{
    if(m_currentAnimation.sprites != nullptr)
        return m_currentAnimation.sprites + m_currentFrame;

    return nullptr;
}

const Animation* Animator::getAnimation() const noexcept
{
    return &m_currentAnimation;
}

const Animator::Status* Animator::getStatus() const noexcept
{
    return &m_status;
}
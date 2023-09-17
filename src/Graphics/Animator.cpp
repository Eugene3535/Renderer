#include "Graphics/Texture2D.hpp"
#include "Graphics/Animator.hpp"

Animator::Animator() noexcept :
    m_pSprite(nullptr),
    m_pCurrentAnimation(nullptr),
    m_currentFrame(0),
    m_timer(0)
{
}

Animator::~Animator()
{
}

void Animator::setSprite(Sprite* pSprite) noexcept
{
    m_pSprite = pSprite;
}

void Animator::setAnimation(const Animation2DManager::Animation2D* pAnim) noexcept
{
#ifdef DEBUG
    if( ! (m_pSprite && pAnim) ) return;
#endif

    if(m_pCurrentAnimation != pAnim);
    {
		restart();
        loop(false);
        reverse(false);
        play();

        m_pCurrentAnimation = pAnim;
        m_pSprite->setTexture(pAnim->pTexture->texture);
        m_currentFrame = m_pCurrentAnimation->startFrame;
		m_pSprite->setFrame(m_currentFrame);		
    }
}

const Sprite* Animator::getSprite() const noexcept
{
	return m_pSprite;
}

const Animation2DManager::Animation2D* Animator::getAnimation() const noexcept
{
	return m_pCurrentAnimation;
}

const Animator::Status* Animator::getStatus() const noexcept
{
    return &m_status;
}

void Animator::update(float dt) noexcept
{
#ifdef DEBUG
    if( !( m_pSprite && m_pCurrentAnimation )) return;
#endif

    if (m_status.isPlaying) 
    {
        m_timer += m_pCurrentAnimation->fps * dt;

        if(m_timer > m_pCurrentAnimation->delay)
        {
            m_currentFrame += m_status.isReversed ? -1 : 1;
            m_timer = 0.0f;

            bool isOutOfRange = m_status.isReversed ? (m_currentFrame < 0) : (m_currentFrame >= m_pCurrentAnimation->duration);

            if (isOutOfRange)
            { 
                if ( ! m_status.isLooped )
                {
                    m_status.isOver = true;
                    return;
                }
                restart();
            }
			m_pSprite->setFrame(m_currentFrame);
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
    m_currentFrame  = m_status.isReversed ? (m_pCurrentAnimation->duration - 1) : 0;
    m_timer         = 0.0f;
    m_status.isOver = false;
}

void Animator::reverse(bool r) noexcept
{
    m_status.isReversed = r;
}
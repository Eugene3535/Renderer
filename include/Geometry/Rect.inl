#include "Rect.hpp"
template <class T>
Rect<T>::Rect() noexcept :
    left  (0),
    top   (0),
    width (0),
    height(0)
{

}

template <class T>
Rect<T>::Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight) noexcept :
    left  (rectLeft),
    top   (rectTop),
    width (rectWidth),
    height(rectHeight)
{

}

template<class T>
template<class U>
Rect<T>::Rect(const U& position, const U& size) noexcept:
    left(static_cast<T>(position.x)),
    top(static_cast<T>(position.y)),
    width(static_cast<T>(size.x)),
    height(static_cast<T>(size.y))
{
}

template <class T>
template <class U>
Rect<T>::Rect(const Rect<U>& rectangle) noexcept :
    left  (static_cast<T>(rectangle.left)),
    top   (static_cast<T>(rectangle.top)),
    width (static_cast<T>(rectangle.width)),
    height(static_cast<T>(rectangle.height))
{
}

template <class T>
bool Rect<T>::contains(T x, T y) const noexcept
{
    if ((x < left) || (x > left + width) || (y < top) || (y > top + height))
        return false;

    return true;
}

template<class T>
template <class U>
inline bool Rect<T>::contains(const U& vec2) const noexcept
{
    return contains(static_cast<T>(vec2.x), static_cast<T>(vec2.y));
}

template <class T>
bool Rect<T>::intersects(const Rect<T>& rectangle) const noexcept
{
    if ((rectangle.left + rectangle.width < left) ||
        (rectangle.left > left + width)           ||
        (rectangle.top + rectangle.height < top)  ||
        (rectangle.top > top + height))
        return false;

    return true;
}
#ifndef ITEM_EXTENTS_HPP
#define ITEM_EXTENTS_HPP

#include <algorithm>
#include <type_traits>
#include <cstdlib>

#include "extmath/Vector2.hpp"

template <typename T>
class Rect
{
public:
    Rect() noexcept;
    Rect(T X, T Y, T Width, T Height) noexcept;
    Rect(const Vector2<T>& position, const Vector2<T>& size) noexcept;

    template <typename U>
    explicit Rect(const Rect<U>& rect) noexcept;

    bool contains(T X, T Y) const noexcept;
    bool contains(const Vector2<T>& point) const noexcept;

    bool intersects(const Rect<T>& rect) const noexcept;

    T x;
    T y;
    T width;
    T height;
};

template <typename T>
bool operator ==(const Rect<T>& left, const Rect<T>& right);

template <typename T>
bool operator !=(const Rect<T>& left, const Rect<T>& right);

#include "extmath/Rect.inl"

typedef Rect<int>   IntRect;
typedef Rect<float> FloatRect;

#endif 
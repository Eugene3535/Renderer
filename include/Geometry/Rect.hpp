#ifndef RECT_HPP
#define RECT_HPP

#include <algorithm>

template <class T>
class Rect
{
public:
    Rect() noexcept;

    Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight) noexcept;

    template <class U>
    explicit Rect(const U& position, const U& size) noexcept;

    template <class U>
    explicit Rect(const Rect<U>& rectangle) noexcept;

    bool contains(T x, T y) const noexcept;

    template <class U>
    bool contains(const U& vec2) const noexcept;

    bool intersects(const Rect<T>& rectangle) const noexcept;

    T left;   ///< Left coordinate of the rectangle
    T top;    ///< Top coordinate of the rectangle
    T width;  ///< Width of the rectangle
    T height; ///< Height of the rectangle
};

#include "Geometry/Rect.inl"

using IntRect = Rect<int>;
using FloatRect = Rect<float> ;

#endif 

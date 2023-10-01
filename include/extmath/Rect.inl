template <typename T>
Rect<T>::Rect() noexcept:
    x(0),
    y(0),
    width(0),
    height(0)
{
}

template <typename T>
Rect<T>::Rect(T X, T Y, T Width, T Height) noexcept:
    x(X),
    y(Y),
    width(Width),
    height(Height)
{
}

template <typename T>
Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size) noexcept:
    x(position.x),
    y(position.y),
    width (size.x),
    height(size.y)
{
}

template <typename T>
template <typename U>
Rect<T>::Rect(const Rect<U>& rectangle) noexcept:
    x(static_cast<T>(rectangle.x)),
    y(static_cast<T>(rectangle.y)),
    width (static_cast<T>(rectangle.width)),
    height(static_cast<T>(rectangle.height))
{
}

template <typename T>
bool Rect<T>::contains(T X, T Y) const noexcept
{
    return (X > x) && (X < x + width) && (Y > y) && (Y < y + height);
}

template <typename T>
bool Rect<T>::contains(const Vector2<T>& point) const noexcept
{
    return contains(point.x, point.y);
}

template <typename T>
bool Rect<T>::intersects(const Rect<T>& rect) const noexcept
{
    if constexpr (std::is_same<T, int>::value)
    {
        return (std::abs(x - rect.x) < ((width + rect.width) >> 1) && std::abs(y - rect.y) < ((height + rect.height) >> 1))
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        return (fabsf(x - rect.x) < ((width + rect.width) * 0.5f) && fabsf(y - rect.y) < ((height + rect.height) * 0.5f))
    } 

    return false;
}

template <typename T>
inline bool operator ==(const Rect<T>& left, const Rect<T>& right)
{
    return (left.x == right.x) && (left.width == right.width) &&
           (left.y == right.y) && (left.height == right.height);
}

template <typename T>
inline bool operator !=(const Rect<T>& left, const Rect<T>& right)
{
    return !(left == right);
}

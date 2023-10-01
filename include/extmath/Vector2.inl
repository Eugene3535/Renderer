template <typename T>
inline Vector2<T>::Vector2() noexcept:
    x(0),
    y(0)
{
}

template <typename T>
inline Vector2<T>::Vector2(T X, T Y) noexcept:
    x(X),
    y(Y)
{
}

template <typename T>
template <typename U>
inline Vector2<T>::Vector2(const Vector2<U>& vector) noexcept:
    x(static_cast<T>(vector.x)),
    y(static_cast<T>(vector.y))
{
}

template <typename T>
inline Vector2<T> operator -(const Vector2<T>& right) noexcept
{
    return Vector2<T>(-right.x, -right.y);
}

template <typename T>
inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right) noexcept
{
    left.x += right.x;
    left.y += right.y;

    return left;
}

template <typename T>
inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right) noexcept
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}

template <typename T>
inline Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right) noexcept
{
    return Vector2<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
inline Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right) noexcept
{
    return Vector2<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
inline Vector2<T> operator *(const Vector2<T>& left, T right) noexcept
{
    return Vector2<T>(left.x * right, left.y * right);
}

template <typename T>
inline Vector2<T> operator *(T left, const Vector2<T>& right) noexcept
{
    return Vector2<T>(right.x * left, right.y * left);
}

template <typename T>
inline Vector2<T>& operator *=(Vector2<T>& left, T right) noexcept
{
    left.x *= right;
    left.y *= right;

    return left;
}

template <typename T>
inline Vector2<T> operator /(const Vector2<T>& left, T right) noexcept
{
    return Vector2<T>(left.x / right, left.y / right);
}

template <typename T>
inline Vector2<T> operator /(T left, const Vector2<T>& right) noexcept
{
    return Vector2<T>(left / right.x, left / right.y);
}

template <typename T>
inline Vector2<T>& operator /=(Vector2<T>& left, T right) noexcept
{
    left.x /= right;
    left.y /= right;

    return left;
}

template <typename T>
inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right) noexcept
{
    return (left.x == right.x) && (left.y == right.y);
}

template <typename T>
inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right) noexcept
{
    return (left.x != right.x) || (left.y != right.y);
}

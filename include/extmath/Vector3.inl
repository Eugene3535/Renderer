template <typename T>
inline Vector3<T>::Vector3() noexcept:
    x(0),
    y(0),
    z(0)
{
}

template <typename T>
inline Vector3<T>::Vector3(T X, T Y, T Z) noexcept:
    x(X),
    y(Y),
    z(Z)
{
}

template <typename T>
template <typename U>
inline Vector3<T>::Vector3(const Vector3<U>& vector) noexcept:
    x(static_cast<T>(vector.x)),
    y(static_cast<T>(vector.y)),
    z(static_cast<T>(vector.z))
{
}

template <typename T>
inline Vector3<T> operator -(const Vector3<T>& left) noexcept
{
    return Vector3<T>(-left.x, -left.y, -left.z);
}

template <typename T>
inline Vector3<T>& operator +=(Vector3<T>& left, const Vector3<T>& right) noexcept
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}

template <typename T>
inline Vector3<T>& operator -=(Vector3<T>& left, const Vector3<T>& right) noexcept
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}

template <typename T>
inline Vector3<T> operator +(const Vector3<T>& left, const Vector3<T>& right) noexcept
{
    return Vector3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}

template <typename T>
inline Vector3<T> operator -(const Vector3<T>& left, const Vector3<T>& right) noexcept
{
    return Vector3<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}

template <typename T>
inline Vector3<T> operator *(const Vector3<T>& left, T right) noexcept
{
    return Vector3<T>(left.x * right, left.y * right, left.z * right);
}

template <typename T>
inline Vector3<T> operator *(T left, const Vector3<T>& right) noexcept
{
    return Vector3<T>(right.x * left, right.y * left, right.z * left);
}

template <typename T>
inline Vector3<T>& operator *=(Vector3<T>& left, T right) noexcept
{
    left.x *= right;
    left.y *= right;
    left.z *= right;

    return left;
}

template <typename T>
inline Vector3<T> operator /(const Vector3<T>& left, T right) noexcept
{
    return Vector3<T>(left.x / right, left.y / right, left.z / right);
}

template <typename T>
inline Vector3<T> operator /(T left, const Vector3<T>& right) noexcept
{
    return Vector3<T>(left / right.x, left / right.y, left / right.z);
}

template <typename T>
inline Vector3<T>& operator /=(Vector3<T>& left, T right) noexcept
{
    left.x /= right;
    left.y /= right;
    left.z /= right;

    return left;
}

template <typename T>
inline bool operator ==(const Vector3<T>& left, const Vector3<T>& right) noexcept
{
    return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

template <typename T>
inline bool operator !=(const Vector3<T>& left, const Vector3<T>& right) noexcept
{
    return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}

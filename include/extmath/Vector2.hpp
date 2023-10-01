#ifndef VECTOR2_HPP
#define VECTOR2_HPP

template <typename T>
class Vector2
{
public:
    Vector2() noexcept;
    Vector2(T X, T Y) noexcept;

    template <typename U>
    explicit Vector2(const Vector2<U>& vector) noexcept;

    T x;
    T y;
};

template <typename T>
Vector2<T> operator -(const Vector2<T>& right) noexcept;

template <typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right) noexcept;

template <typename T>
Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right) noexcept;

template <typename T>
Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right) noexcept;

template <typename T>
Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right) noexcept;

template <typename T>
Vector2<T> operator *(const Vector2<T>& left, T right) noexcept;

template <typename T>
Vector2<T> operator *(T left, const Vector2<T>& right) noexcept;

template <typename T>
Vector2<T>& operator *=(Vector2<T>& left, T right) noexcept;

template <typename T>
Vector2<T> operator /(const Vector2<T>& left, T right) noexcept;

template <typename T>
Vector2<T> operator /(T left, const Vector2<T>& right) noexcept;

template <typename T>
Vector2<T>& operator /=(Vector2<T>& left, T right) noexcept;

template <typename T>
bool operator ==(const Vector2<T>& left, const Vector2<T>& right) noexcept;

template <typename T>
bool operator !=(const Vector2<T>& left, const Vector2<T>& right) noexcept;

#include "extmath/Vector2.inl"

typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;

#endif
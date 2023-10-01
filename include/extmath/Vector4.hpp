#ifndef VECTOR4_HPP
#define VECTOR4_HPP

template <typename T>
class Vector4
{
public:
    Vector4() noexcept;
    Vector4(T X, T Y, T Z, T W) noexcept;

    template <typename U>
    explicit Vector4(const Vector4<U>& vector) noexcept;

    T x;
    T y;
    T z;
    T w;
};

template <typename T>
Vector4<T> operator -(const Vector4<T>& left) noexcept;

template <typename T>
Vector4<T>& operator +=(Vector4<T>& left, const Vector4<T>& right) noexcept;

template <typename T>
Vector4<T>& operator -=(Vector4<T>& left, const Vector4<T>& right) noexcept;

template <typename T>
Vector4<T> operator +(const Vector4<T>& left, const Vector4<T>& right) noexcept;

template <typename T>
Vector4<T> operator -(const Vector4<T>& left, const Vector4<T>& right) noexcept;

template <typename T>
Vector4<T> operator *(const Vector4<T>& left, T right) noexcept;

template <typename T>
Vector4<T> operator *(T left, const Vector4<T>& right) noexcept;

template <typename T>
Vector4<T>& operator *=(Vector4<T>& left, T right) noexcept;

template <typename T>
Vector4<T> operator /(const Vector4<T>& left, T right) noexcept;

template <typename T>
Vector4<T> operator /(T left, const Vector4<T>& right) noexcept;

template <typename T>
Vector4<T>& operator /=(Vector4<T>& left, T right) noexcept;

template <typename T>
bool operator ==(const Vector4<T>& left, const Vector4<T>& right) noexcept;

template <typename T>
bool operator !=(const Vector4<T>& left, const Vector4<T>& right) noexcept;

#include "extmath/Vector4.inl"

typedef Vector4<int>   Vector4i;
typedef Vector4<float> Vector4f;

#endif
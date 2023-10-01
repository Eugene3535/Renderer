#ifndef VECTOR3_HPP
#define VECTOR3_HPP

template <typename T>
class Vector3
{
public:
    Vector3() noexcept;
    Vector3(T X, T Y, T Z) noexcept;

    template <typename U>
    explicit Vector3(const Vector3<U>& vector) noexcept;

    T x;
    T y;
    T z;
};

template <typename T>
Vector3<T> operator -(const Vector3<T>& left) noexcept;

template <typename T>
Vector3<T>& operator +=(Vector3<T>& left, const Vector3<T>& right) noexcept;

template <typename T>
Vector3<T>& operator -=(Vector3<T>& left, const Vector3<T>& right) noexcept;

template <typename T>
Vector3<T> operator +(const Vector3<T>& left, const Vector3<T>& right) noexcept;

template <typename T>
Vector3<T> operator -(const Vector3<T>& left, const Vector3<T>& right) noexcept;

template <typename T>
Vector3<T> operator *(const Vector3<T>& left, T right) noexcept;

template <typename T>
Vector3<T> operator *(T left, const Vector3<T>& right) noexcept;

template <typename T>
Vector3<T>& operator *=(Vector3<T>& left, T right) noexcept;

template <typename T>
Vector3<T> operator /(const Vector3<T>& left, T right) noexcept;

template <typename T>
Vector3<T> operator /(T left, const Vector3<T>& right) noexcept;

template <typename T>
Vector3<T>& operator /=(Vector3<T>& left, T right) noexcept;

template <typename T>
bool operator ==(const Vector3<T>& left, const Vector3<T>& right) noexcept;

template <typename T>
bool operator !=(const Vector3<T>& left, const Vector3<T>& right) noexcept;

#include "extmath/Vector3.inl"

typedef Vector3<int>   Vector3i;
typedef Vector3<float> Vector3f;

#endif
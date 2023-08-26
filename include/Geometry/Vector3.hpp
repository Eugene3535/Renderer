#ifndef VECTOR3_HPP
#define VECTOR3_HPP

template <typename T>
class Vector3
{
public:
    Vector3();
    Vector3(T X, T Y, T Z);

    template <typename U>
    explicit Vector3(const Vector3<U>& vector);

    T x;
    T y;
    T z;
};

template <typename T>
Vector3<T> operator -(const Vector3<T>& left);

template <typename T>
Vector3<T>& operator +=(Vector3<T>& left, const Vector3<T>& right);

template <typename T>
Vector3<T>& operator -=(Vector3<T>& left, const Vector3<T>& right);

template <typename T>
Vector3<T> operator +(const Vector3<T>& left, const Vector3<T>& right);

template <typename T>
Vector3<T> operator -(const Vector3<T>& left, const Vector3<T>& right);

template <typename T>
Vector3<T> operator *(const Vector3<T>& left, T right);

template <typename T>
Vector3<T> operator *(T left, const Vector3<T>& right);

template <typename T>
Vector3<T>& operator *=(Vector3<T>& left, T right);

template <typename T>
Vector3<T> operator /(const Vector3<T>& left, T right);

template <typename T>
Vector3<T>& operator /=(Vector3<T>& left, T right);

template <typename T>
bool operator ==(const Vector3<T>& left, const Vector3<T>& right);

template <typename T>
bool operator !=(const Vector3<T>& left, const Vector3<T>& right);

#include "Vector3.inl"

// Define the most common types
typedef Vector3<int>   Vector3i;
typedef Vector3<float> Vector3f;

#endif
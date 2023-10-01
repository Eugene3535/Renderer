#ifndef CUBOID_HPP
#define CUBOID_HPP

#include "extmath/Vector3.hpp"

template <typename T>
class Cuboid
{
public:
    Cuboid() noexcept;
    Cuboid(T minX, T minY, T minZ, T maxX, T maxY, T maxZ) noexcept;
    Cuboid(const Vector3<T>& minPt, const Vector3<T>& maxPt) noexcept;

    template <typename U>
    explicit Cuboid(const Cuboid<U>& cuboid) noexcept;

    bool contains(const Vector3<T>& point) const noexcept;
    bool contains(T x, T y, T z) const noexcept;

    Vector3<T> minPoint;
    Vector3<T> maxPoint;
};

#include "extmath/Cuboid.inl"

typedef Cuboid<int>   IntCuboid;
typedef Cuboid<float> FloatCuboid;

#endif
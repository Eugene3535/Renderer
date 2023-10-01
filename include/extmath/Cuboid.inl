template <typename T>
Cuboid<T>::Cuboid() noexcept:
    minPoint(),
    maxPoint()
{
}

template <typename T>
Cuboid<T>::Cuboid(T minX, T minY, T minZ, T maxX, T maxY, T maxZ) noexcept:
    minPoint(minX, minY, minZ),
    maxPoint(maxX, maxY, maxZ)
{
}

template <typename T>
Cuboid<T>::Cuboid(const Vector3<T>& minPt, const Vector3<T>& maxPt) noexcept:
    minPoint(minPt),
    maxPoint(maxPt)
{
}

template <typename T>
template <typename U>
Cuboid<T>::Cuboid(const Cuboid<U>& cuboid) noexcept:
    minPoint(static_cast<T>(cuboid.minPoint.x), static_cast<T>(cuboid.minPoint.y), static_cast<T>(cuboid.minPoint.z)),
    maxPoint(static_cast<T>(cuboid.maxPoint.x), static_cast<T>(cuboid.maxPoint.y), static_cast<T>(cuboid.maxPoint.z))
{
}
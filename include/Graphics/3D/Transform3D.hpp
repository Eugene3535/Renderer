#ifndef TRANSFORM_3D_HPP
#define TRANSFORM_3D_HPP

class Transform3D
{
public:
    Transform3D() noexcept;
    Transform3D(const float* matrix) noexcept;
    ~Transform3D();

    void translate() noexcept;

    static const Transform3D Identity; 

private:
    float m_matrix[16];
};




#endif
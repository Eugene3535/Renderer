#include <cmath>
#include <vector>

#include "Utils/Defines.hpp"
#include "Utils/Geometry.hpp"

float Geometry::sine(float angle)
{
#ifdef DEBUG
    if( angle > 359.0f || angle < 0.0f)
        return 0.0f;
#endif

    static std::vector<float> table;

    if (table.empty())
    {
        table.resize(360, 0.0f);

        for (std::int32_t i = 0; i < 360; ++i)     
            table[i] = std::sinf(static_cast<float>(i) * DEGTORAD);
    }
    
    return table[static_cast<std::size_t>(angle)];
}

float Geometry::cosine(float angle)
{
#ifdef DEBUG
    if( angle > 359.0f || angle < 0.0f)
        return 0.0f;
#endif

    static std::vector<float> table;

    if (table.empty())
    {
        table.resize(360, 0.0f);

        for (std::int32_t i = 0; i < 360; ++i)     
            table[i] = std::cosf(static_cast<float>(i) * DEGTORAD);
    }

    return table[static_cast<std::size_t>(angle)];
}
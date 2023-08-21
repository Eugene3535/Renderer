#include <cmath>
#include <vector>

#include "Utils/Defines.hpp"
#include "Geometry/FastMath.hpp"

float FastMath::sine(float angle)
{
#ifdef DEBUG
    if( angle > 359.0f || angle < 0.0f)
        return 0.0f;
#endif

    static std::vector<float> table;

    if (table.empty())
    {
        table.resize(360, 0.0f);

        for (int i = 0; i < 360; ++i)
            table[i] = sinf(static_cast<float>(i) * DEGTORAD);
    }

    return table[static_cast<std::size_t>(angle)];
}

float FastMath::cosine(float angle)
{
#ifdef DEBUG
    if( angle > 359.0f || angle < 0.0f)
        return 0.0f;
#endif

    static std::vector<float> table;

    if (table.empty())
    {
        table.resize(360, 0.0f);

        for (int i = 0; i < 360; ++i)
            table[i] = cosf(static_cast<float>(i) * DEGTORAD);
    }

    return table[static_cast<std::size_t>(angle)];
}

#include <cmath>
#include <vector>

#include "Utils/Defines.hpp"
#include "Geometry/FastSinCos.hpp"

float FastSinCos::sine(int angle)
{
    if( angle > 359 || angle < 0)
        return 0;

    static std::vector<float> table;

    if (table.empty()) // First call only
    {
        table.resize(360, 0.0f);

        for (int i = 0; i < 360; ++i)
            table[i] = sinf(static_cast<float>(i) * DEGTORAD);
    }

    return table[angle];
}

float FastSinCos::cosine(int angle)
{
    if( angle > 359 || angle < 0)
        return 0;

    static std::vector<float> table;

    if (table.empty()) // First call only
    {
        table.resize(360, 0.0f);

        for (int i = 0; i < 360; ++i)
            table[i] = cosf(static_cast<float>(i) * DEGTORAD);
    }

    return table[angle];
}

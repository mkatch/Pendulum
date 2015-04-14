#include "pendulum.hpp"

namespace jg {

int const   Pendulum::DEFAULT_SEGMENT_CNT   = 1;
float const Pendulum::DEFAULT_LENGTH        = 1;
float const Pendulum::DEFAULT_MASS          = 1;
float const Pendulum::DEFAULT_RADIUS        = 0.2;

Pendulum::PolyChain
Pendulum::positions() const
{
    PolyChain result(weightCnt());
    result[0] = math::float2(deflection(0), 0);
    for (int i = 1; i < weightCnt(); ++i)
    {
        float const dySq = m_lengthSq
            - math::sq(deflection(i) - deflection(i - 1));
        float const dy = dySq <= 0.0f ? 0.0f : std::sqrt(dySq);
        result[i] = math::float2(
            deflection(i),
            result[i - 1].y + dy
        );
    }
    return result;
}

} // namespace jg

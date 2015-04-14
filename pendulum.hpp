#ifndef JG_PENDULUM_HPP
#define JG_PENDULUM_HPP

#include <vector>
#include "math/math.hpp"
#include "ode.hpp"

namespace jg {

/*******************************************************************************
********************************************************************************
**                                                                            **
**                                Pendulum                                    **
**                                                                            **
********************************************************************************
*******************************************************************************/

class Pendulum
{
public:
    static int const    DEFAULT_SEGMENT_CNT;
    static float const  DEFAULT_LENGTH;
    static float const  DEFAULT_MASS;
    static float const  DEFAULT_RADIUS;

    typedef std::vector<math::vector2<float> > PolyChain;

    Pendulum(
        size_t  segmentCnt  = DEFAULT_SEGMENT_CNT,
        float   length      = DEFAULT_LENGTH,
        float   mass        = DEFAULT_MASS,
        float   radius      = DEFAULT_RADIUS
    );
    Pendulum(
        std::vector<float> const&   v,
        float                       length  = DEFAULT_LENGTH,
        float                       mass    = DEFAULT_MASS,
        float                       radius  = DEFAULT_RADIUS
    );


    size_t      segmentCnt() const;
    void        setSegmentCnt(size_t segmentCnt);
    size_t      weightCnt() const;
    float       length() const;
    float       totalLength() const;
    void        setLength(float length);
    float       mass() const;
    float       totalMass() const;
    void        setMass(float mass);
    float       radius() const;
    void        setRadius(float radius);
    float       deflection(int i) const;
    void        setDeflection(int i, float d);
    void        setDeflections(std::vector<float> const& v);
    PolyChain   positions() const;

private:
    std::vector<float>  m_deflection;
    float               m_length;
    float               m_mass;
    float               m_radius;
    float               m_lengthSq;
};

inline
Pendulum::Pendulum(
    size_t  segmentCnt,
    float   length,
    float   mass,
    float   radius
)
:   m_deflection(segmentCnt + 1, 0),
    m_length(length),
    m_mass(mass),
    m_radius(radius),
    m_lengthSq(math::sq(length))
{/* Do nothing. */}

inline
Pendulum::Pendulum(
    std::vector<float> const&   deflection,
    float                       length,
    float                       mass,
    float                       radius
)
:   m_deflection(deflection),
    m_length(length),
    m_mass(mass),
    m_radius(radius)
{/* Do nothing. */}

inline size_t
Pendulum::segmentCnt() const { return m_deflection.size() - 1; }

inline void
Pendulum::setSegmentCnt(size_t segmentCnt)
{ m_deflection = std::vector<float>(segmentCnt + 1, 0); }

inline size_t
Pendulum::weightCnt() const { return m_deflection.size(); }

inline float
Pendulum::length() const { return m_length; }

inline float
Pendulum::totalLength() const { return length() * segmentCnt(); }

inline void
Pendulum::setLength(float length)
{
    m_length    = length;
    m_lengthSq  = length * length;
}

inline float
Pendulum::mass() const { return m_mass; }

inline float
Pendulum::totalMass() const { return mass() * weightCnt(); }

inline void
Pendulum::setMass(float mass) { m_mass = mass; }

inline float
Pendulum::radius() const { return m_radius; }

inline void
Pendulum::setRadius(float radius) { m_radius = radius; }

inline float
Pendulum::deflection(int i) const { return m_deflection[i]; }

inline void
Pendulum::setDeflection(int i, float d) { m_deflection[i] = d; }

inline void
Pendulum::setDeflections(std::vector<float> const& d) { m_deflection = d; }

/*******************************************************************************
********************************************************************************
**                                                                            **
**                             PendulumODEFun                                 **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T>
class PendulumODEFun : public ODEFun<T>
{
public:
    static T const GRAV_ACCEL;
    static T const DRAG_COEFF;

    PendulumODEFun(
        T length,
        T mass,
        T radius,
        T angFrequency,
        T amplitude,
        T viscosity,
        T density
    )
    :   C(GRAV_ACCEL / length),
        mass(mass),
        angFrequency(angFrequency),
        amplitude(amplitude),
        L(viscosity * math::PI * radius * radius / mass),
        Q(0.5 * DRAG_COEFF * math::PI * radius * radius * density / mass)
    {/* Do nothing. */}
    
    typename ODE<T>::Y
    operator () (typename ODE<T>::X x, typename ODE<T>::Y const& y) const
    {
        int const n = y.size() / 2 - 1;
        typename ODE<T>::Y Dy(y.size(), 0);
        Dy[0] = amplitude * angFrequency * std::cos(angFrequency * x);
        for (int k = 1; k < n; ++k)
        {
            Dy[k]           = y[n + k + 1];
            Dy[n + k + 1]   = C * ( static_cast<T>(n - k + 1) * y[k - 1]
                            + static_cast<T>(n - k) * y[k + 1]
                            - static_cast<T>(2 * (n - k) + 1) * y[k] )
                            - y[n + k + 1] * (L + (y[n + k + 1] < 0 ? -Q : Q)
                            * y[n + k + 1]);

        }
        Dy[n]           = y[2 * n + 1];
        Dy[2 * n + 1]   = C * (y[n - 1] - y[n])
                        - y[2 * n + 1] * (L + (y[2 * n + 1] < 0 ? -Q : Q)
                        * y[2 * n + 1]);
        return Dy;
    }

private:
    T const C;
    T const mass;
    T const angFrequency;
    T const amplitude;
    T const L;
    T const Q;
};

template <typename T> T const PendulumODEFun<T>::GRAV_ACCEL = 9.81;
template <typename T> T const PendulumODEFun<T>::DRAG_COEFF = 0.47;

} // namespace jg

#endif // JG_PENDULUM_HPP

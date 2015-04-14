#ifndef JG_ODE_HPP
#define JG_ODE_HPP

#include "math/math.hpp"
#include "spawner.hpp"
#include "buffer.hpp"

namespace jg {

/*******************************************************************************
********************************************************************************
**                                                                            **
**                                  ODE                                       **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T>
class ODE
{
public:
    typedef T               X;
    typedef std::vector<T>  Y;
    
    struct Point
    {
        X x;
        Y y;

        Point(X x = 0, Y y = Y(1, 0))
        : x(x), y(y)
        {/* Do nothing. */}
    };
};

/*******************************************************************************
********************************************************************************
**                                                                            **
**                                 ODEFun                                     **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T>
class ODEFun
{
public:
    virtual ~ODEFun() {/* Do nothing. */}
    virtual typename ODE<T>::Y operator () (typename ODE<T>::X x,
        typename ODE<T>::Y const& y) const = 0;
};

/*******************************************************************************
********************************************************************************
**                                                                            **
**                               Integrator                                   **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T>
class Integrator
{
public:
    static typename ODE<T>::X const DEFAULT_STEP;

    virtual ~Integrator() {/* Do nothing. */}
    virtual void advance(typename ODE<T>::Point& p, ODEFun<T> const& f) = 0;
};

template <typename T>
typename ODE<T>::X const Integrator<T>::DEFAULT_STEP = 1e-3;

/*******************************************************************************
********************************************************************************
**                                                                            **
**                             EulerIntegrator                                **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T>
class EulerIntegrator : public Integrator<T>
{
public:
    EulerIntegrator(typename ODE<T>::X step = Integrator<T>::DEFAULT_STEP);
    void advance(typename ODE<T>::Point& point, ODEFun<T> const& f);

private:
    typename ODE<T>::X h;
};

template <typename T> inline
EulerIntegrator<T>::EulerIntegrator(typename ODE<T>::X step)
:   h(step)
{/* Do nothing. */}

template <typename T>
inline void
EulerIntegrator<T>::advance(typename ODE<T>::Point& p, ODEFun<T> const& f)
{
    p.y += h * f(p.x, p.y);
    p.x += h;
}

/*******************************************************************************
********************************************************************************
**                                                                            **
**                              RK4Integrator                                 **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T>
class RK4Integrator : public Integrator<T>
{
public:
    RK4Integrator(typename ODE<T>::X step = Integrator<T>::DEFAULT_STEP)
    :   h(step)
    {/* Do nothing. */}

    void advance(typename ODE<T>::Point& p, ODEFun<T> const& f)
    {
        typename ODE<T>::X& x = p.x;
        typename ODE<T>::Y& y = p.y;

        typename ODE<T>::Y const k1 = h * f(x          , y               );
        typename ODE<T>::Y const k2 = h * f(x + 0.5 * h, y + 0.5 * h * k1);
        typename ODE<T>::Y const k3 = h * f(x + 0.5 * h, y + 0.5 * h * k2);
        typename ODE<T>::Y const k4 = h * f(x + h      , y + h * k3      );

        x += h;
        y += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    }

private:
    typename ODE<T>::X h;
};

/*******************************************************************************
********************************************************************************
**                                                                            **
**                               ODESolution                                  **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T>
class ODESolution
{
public:
    typedef std::pair<typename ODE<T>::X, typename ODE<T>::X> Range;

    ODESolution(typename ODE<T>::X x = 0,
        typename ODE<T>::Y const& y = typename ODE<T>::Y(1, 0),
        ODEFun<T>* f = NULL,
        Integrator<T>* = new EulerIntegrator<T>());
    ODESolution(typename ODE<T>::Point const& p, ODEFun<T>* f = NULL,
        Integrator<T>* = new EulerIntegrator<T>());

    void setInitialCondition(typename ODE<T>::X x, typename ODE<T>::Y const& y);
    void setInitialCondition(typename ODE<T>::Point const& p);
    void setEquation(ODEFun<T>* f);
    void setIntegrator(Integrator<T>* integrator);

    typename ODE<T>::Y operator () (typename ODE<T>::X x);
    typename ODE<T>::Y eval(typename ODE<T>::X x);

    Range   bufferedRange();
    void    start();
    void    stop();
    void    pause();
    bool    running() const;
    bool    buffering() const;

private:
    class PointSpawner : public Spawner<typename ODE<T>::Point>
    {
    public:
        PointSpawner(typename ODE<T>::Point const& lastPoint
            = typename ODE<T>::Point(), ODEFun<T>* f = NULL,
            Integrator<T>* integrator = NULL);
        ~PointSpawner();

        void setLastPoint(typename ODE<T>::Point const& lastPoint);
        void setF(ODEFun<T>* f);
        void setIntegrator(Integrator<T>* integrator);

        typename ODE<T>::Point spawn();
    
    private:
        typename ODE<T>::Point  m_lastPoint;
        ODEFun<T>*              m_f;
        Integrator<T>*          m_integrator;
    };

    PointSpawner                    m_spawner;
    Buffer<typename ODE<T>::Point>  m_buffer;
    typename ODE<T>::X              m_lastArg;
    typename ODE<T>::Point          m_beg;
    typename ODE<T>::Point          m_end;
    typename ODE<T>::Point          m_initialCondition;
};

template <typename T> inline
ODESolution<T>::ODESolution(
    typename ODE<T>::X          x,
    typename ODE<T>::Y const&   y,
    ODEFun<T>*                  f,
    Integrator<T>*              integrator
)
:   m_spawner(typename ODE<T>::Point(x, y), f, integrator),
    m_buffer(&m_spawner),
    m_initialCondition(x, y)
{
    if (f != NULL) start();
}

template <typename T> inline
ODESolution<T>::ODESolution(
    typename ODE<T>::Point const&   p,
    ODEFun<T>*                      f,
    Integrator<T>*                  integrator
)
:   m_spawner(p, f, integrator),
    m_buffer(&m_spawner),
    m_initialCondition(p)
{
    if (f != NULL) start();
}

template <typename T>
inline void
ODESolution<T>::setInitialCondition(
    typename ODE<T>::X          x,
    typename ODE<T>::Y const&   y
)
{ setInitialCondition(typename ODE<T>::Point(x, y)); }

template <typename T>
inline void
ODESolution<T>::setInitialCondition(typename ODE<T>::Point const& p)
{
    if (m_buffer.running())
        throw std::runtime_error("ODESolution::setInitialCondition(): Cannot \
modify solution while bufering");
    m_initialCondition = p;
}

template <typename T>
inline void
ODESolution<T>::setEquation(ODEFun<T>* f)
{
    if (m_buffer.running())
        throw std::runtime_error("ODESolution::setEquation(): Cannot modify \
solution while buffering.");
    m_spawner.setF(f);
}

template <typename T>
inline void
ODESolution<T>::setIntegrator(Integrator<T>* integrator)
{
    if (m_buffer.running())
        throw std::runtime_error("ODESolution::setIntegrator(): Cannot modify \
solution while buffering.");
    m_spawner.setIntegrator(integrator);
}

template <typename T>
inline typename ODE<T>::Y
ODESolution<T>::operator () (typename ODE<T>::X x) { return eval(x); }

template <typename T>
typename ODE<T>::Y
ODESolution<T>::eval(typename ODE<T>::X x)
{
    if (x < m_lastArg)
        throw std::invalid_argument("ODESolution::eval(): Argument must not be \
smaller than in the last call and the initial argument.");

    m_lastArg = x;
    while (m_end.x < x)
    {
        m_beg = m_end;
        m_end = m_buffer.next();
    }

    return ((x - m_beg.x) * m_beg.y + (m_end.x - x) * m_end.y)
        / (m_end.x - m_beg.x);
}

template <typename T>
typename ODESolution<T>::Range
ODESolution<T>::bufferedRange() 
{
    return Range(m_buffer.peek().x, m_buffer.peekLast().x);
}

template <typename T>
void
ODESolution<T>::start() {
    if (!m_buffer.buffering())
    {
        m_lastArg   = m_initialCondition.x;
        m_end       = m_initialCondition;
        m_spawner.setLastPoint(m_initialCondition);
    }
    m_buffer.startBuffering();
}

template <typename T> inline void
ODESolution<T>::stop() { m_buffer.stop(); }

template <typename T> inline void
ODESolution<T>::pause() { m_buffer.pause(); }

template <typename T> inline bool
ODESolution<T>::running() const { return m_buffer.running(); }

template <typename T> inline bool
ODESolution<T>::buffering() const { return m_buffer.buffering(); }

/*******************************************************************************
********************************************************************************
**                                                                            **
**                        ODESolution::PointSpawner                           **
**                                                                            **
********************************************************************************
*******************************************************************************/

template <typename T> inline
ODESolution<T>::PointSpawner::PointSpawner(
    typename ODE<T>::Point const& lastPoint,
    ODEFun<T>* f,
    Integrator<T>* integrator
)
:   m_lastPoint(lastPoint),
    m_f(f),
    m_integrator(integrator)
{/* Do nothing. */}

template <typename T> inline
ODESolution<T>::PointSpawner::~PointSpawner()
{
    delete m_f;
    delete m_integrator;
}

template <typename T>
inline void
ODESolution<T>::PointSpawner::setLastPoint(
    typename ODE<T>::Point const& lastPoint
)
{
    m_lastPoint = lastPoint;
}

template <typename T>
inline void
ODESolution<T>::PointSpawner::setF(ODEFun<T>* f)
{
    if (f != m_f)
    {
        delete m_f;
        m_f = f;
    }
}

template <typename T>
inline void
ODESolution<T>::PointSpawner::setIntegrator(Integrator<T>* integrator)
{
    if (integrator != m_integrator)
    {
        delete m_integrator;
        m_integrator = integrator;
    }
}

template <typename T>
inline typename ODE<T>::Point
ODESolution<T>::PointSpawner::spawn()
{
    m_integrator->advance(m_lastPoint, *m_f);
    return m_lastPoint;
}

} // namespace jg

#endif // JG_ODE_HPP

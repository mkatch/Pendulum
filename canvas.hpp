#ifndef JG_CANVAS_HPP
#define JG_CANVAS_HPP

#include <QtOpenGL/QtOpenGL>
#include <QtGui/QtGui>

#include "math/math.hpp"
#include "pendulum.hpp"
#include "ode.hpp"

namespace jg {

class Canvas;

class CanvasUpdater : public QObject
{
    Q_OBJECT

public:
    CanvasUpdater(Canvas* canvas);

public slots:
    void update();

private:
    Canvas* const canvas;
};

class Canvas : public QGLWidget
{
    Q_OBJECT

public:
    Canvas(int fps = 30, QWidget* parent = 0);
    ~Canvas();

    bool running() const;
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void checkHover(math::float2 pos);

public slots:
    void start();
    void stop();
    void pause();
    void update();
    void setSegmentCnt(int value);
    void setSegmentLength(float value);
    void setNodeMass(float value);
    void setNodeRadius(float value);
    void setAmplitude(float value);
    void setAngFrequency(float value);
    void setViscosity(float value);
    void setDensity(float density);
    void setIntegrator(QString const& value);
    void setPrecision(QString const& value);
    void setStepExp(int exp);

private:
    enum Integrator {
        EULER,
        RK4
    };
    enum Precision {
        FLOAT,
        DOUBLE
    };

    static float const          HEIGHT;
    static float const          MARGIN;
    static float const          BAR_WIDTH;
    static float const          SECOND_LENGTH;
    static float const          RADIUS_FACTOR;
    static float const          CROSS_FACTOR;
    static float const          LENGTH_FACTOR;
    static uint const           CIRCLE_SIDES;
    static math::float3 const   ANCHOR_COLOR;
    static math::float3 const   SEGMENT_COLOR;
    static math::float3 const   WEIGHT_COLOR;
    static math::float3 const   SECOND_POS_COLOR;
    static math::float3 const   SECOND_NEG_COLOR;
    static math::float3 const   HOVER_COLOR;
    static math::float3 const   HOLD_COLOR;
    static math::float3 const   CROSS_COLOR;

    Pendulum            referencePendulum;
    Pendulum::PolyChain referencePositions;
    Pendulum            pendulum;
    float               angFrequency;
    float               amplitude;
    float               viscosity;
    float               density;
    Integrator          integrator;
    Precision           precision;
    float               step;
    float               scale;
    int                 holding;
    int                 hovering;
    math::float2        grabbedPos;
    QMutex              pendulumMutex;
    QMutex              timeMutex;
    QMutex              flowMutex;
    QTimer              repaintTimer;
    CanvasUpdater       updater;
    QThread             updateThread;
    QTimer              updateTimer;

    ODESolution<float>  solutionFloat;
    ODESolution<double> solutionDouble;
    QElapsedTimer       timer;
    float               canvasWidth;
    float const         canvasHeight;
    qint64              refTime;
    qint64              pauseTime;
    float               elapsedTime;
    float               bufferedTime;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void paintPendulum(Pendulum const& p);
    void paintCircle(math::float2 const& pos, float radius);
    void paintTime(float t);
    void paintCross(math::float2 const& pos, float size);

    math::float2 toLocal(QPoint const& pos) const;
};

} // namespace jg

#endif // JG_CANVAS_HPP

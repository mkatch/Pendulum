#include <cmath>
#include <iostream>

#include "canvas.hpp"

namespace jg {

float const         Canvas::HEIGHT              = 100;
float const         Canvas::MARGIN              = 15;
float const         Canvas::BAR_WIDTH           = 3;
float const         Canvas::SECOND_LENGTH       = 40;
float const         Canvas::RADIUS_FACTOR       = 1.1f;
float const         Canvas::CROSS_FACTOR        = 0.3f;
float const         Canvas::LENGTH_FACTOR       = 0.1f;
uint const          Canvas::CIRCLE_SIDES        = 16;
math::float3 const  Canvas::ANCHOR_COLOR        = math::float3(0.0f, 0.0f, 0.0f);
math::float3 const  Canvas::SEGMENT_COLOR       = math::float3(0.2f, 0.2f, 0.2f);
math::float3 const  Canvas::WEIGHT_COLOR        = math::float3(0.2f, 0.3f, 0.8f);
math::float3 const  Canvas::SECOND_POS_COLOR    = math::float3(1.0f, 1.0f, 1.0f);
math::float3 const  Canvas::SECOND_NEG_COLOR    = math::float3(0.0f, 0.0f, 0.0f);
math::float3 const  Canvas::HOVER_COLOR         = math::float3(1.0f, 1.0f, 1.0f);
math::float3 const  Canvas::HOLD_COLOR          = math::float3(1.0f, 0.0f, 0.0f);
math::float3 const  Canvas::CROSS_COLOR         = math::float3(1.0f, 0.0f, 0.0f);

Canvas::Canvas(int fps, QWidget* parent)
:   QGLWidget(parent),
    step(1e-4),
    holding(0),
    hovering(0),
    updater(this),
    canvasHeight(HEIGHT + 2 * MARGIN)
{
    setMouseTracking(true);
    repaintTimer.setInterval(1000 / fps);
    updateTimer.setInterval(1000 / fps);
    connect(&repaintTimer, SIGNAL(timeout()), this, SLOT(repaint()));
    connect(&updateTimer, SIGNAL(timeout()), &updater, SLOT(update()));
    updater.moveToThread(&updateThread);
    updateTimer.moveToThread(&updateThread);
    updateThread.start();
    QMetaObject::invokeMethod(&updateTimer, "start");
    repaintTimer.start();
}

Canvas::~Canvas()
{
    updateThread.quit();
    updateThread.wait();
}

inline bool
Canvas::running() const
{ return solutionFloat.running() || solutionDouble.running(); }

void
Canvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton || running() || hovering == 0) return;
    holding     = hovering;
    hovering    = 0;
    grabbedPos  = toLocal(event->pos()) - referencePositions[holding];
}

void
Canvas::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton || running() || holding == 0) return;
    holding = 0;
    checkHover(toLocal(event->pos()));
    
}

void
Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if (running()) return;  
    checkHover(toLocal(event->pos()));
}

void
Canvas::checkHover(math::float2 pos)
{
    if (holding == 0)
    {
        for (int i = 1; i < referencePendulum.weightCnt(); ++i)
            if ((referencePositions[i] - pos).lengthSq()
                <= math::sq(referencePendulum.radius()))
            {
                hovering = i;
                return;
            }
        hovering = 0;
    }
    else
    {
        pos     -= grabbedPos;
        pos.y    = std::max(pos.y, referencePositions[holding - 1].y);
        math::float2 const newPos = referencePositions[holding - 1]
            + (pos - referencePositions[holding - 1]).normalized()
            * referencePendulum.length();
        float dx = newPos.x - referencePendulum.deflection(holding);
        for (int i = holding; i < referencePendulum.weightCnt(); ++i)
            referencePendulum.setDeflection(i, referencePendulum.deflection(i) + dx);
        referencePositions = referencePendulum.positions();
    }
}

void
Canvas::start()
{
    flowMutex.lock();
    if (solutionFloat.running() && !solutionFloat.buffering())
    {
        refTime += timer.elapsed() - pauseTime;
        solutionFloat.start();
    }
    else if (solutionDouble.running() && !solutionDouble.buffering())
    {
        refTime += timer.elapsed() - pauseTime;
        solutionDouble.start();
    }
    else
    {
        if(running()) stop();

        pendulum = referencePendulum;
        switch (precision)
        {
        case FLOAT: {
            std::vector<float> y(pendulum.weightCnt() * 2, 0);
            for (int i = 0; i < pendulum.weightCnt(); ++i)
                y[i] = pendulum.deflection(i);
            solutionFloat.setInitialCondition(0, y);
            solutionFloat.setEquation(new PendulumODEFun<float>(
                pendulum.length(),
                pendulum.mass(),
                pendulum.radius(),
                angFrequency,
                amplitude,
                viscosity,
                density
            ));
            switch (integrator)
            {
            case EULER:
                solutionFloat.setIntegrator(new EulerIntegrator<float>(step));
                break;
            case RK4:
                solutionFloat.setIntegrator(new RK4Integrator<float>(step));
                break;
            }
            solutionFloat.start();
            break;
        }
        case DOUBLE: {
            std::vector<double> y(pendulum.weightCnt() * 2, 0);
            for (int i = 0; i < pendulum.weightCnt(); ++i)
                y[i] = pendulum.deflection(i);
            solutionDouble.setInitialCondition(0, y);
            solutionDouble.setEquation(new PendulumODEFun<double>(
                pendulum.length(),
                pendulum.mass(),
                pendulum.radius(),
                angFrequency,
                amplitude,
                viscosity,
                density
            ));
            switch (integrator)
            {
            case EULER:
                solutionDouble.setIntegrator(new EulerIntegrator<double>(step));
                break;
            case RK4:
                solutionDouble.setIntegrator(new RK4Integrator<double>(step));
                break;
            }
            solutionDouble.start();
            break;
        }
        }
        timer.restart();
        refTime = timer.elapsed();
    }
    flowMutex.unlock();
}

void
Canvas::stop()
{
    flowMutex.lock();
    solutionFloat.stop();
    solutionDouble.stop();
    flowMutex.unlock();
}

void
Canvas::pause()
{
    flowMutex.lock();
    solutionFloat.pause();
    solutionDouble.pause();
    pauseTime = timer.elapsed();
    flowMutex.unlock();
}


void
Canvas::update()
{
    flowMutex.lock();
    if (solutionFloat.buffering())
    {
        Pendulum newPendulum = referencePendulum;
        float t = static_cast<float>(timer.elapsed() - refTime) / 1000.0f;
        ODE<float>::Y y = solutionFloat(t);
        for (int i = 0; i < newPendulum.weightCnt(); ++i)
            newPendulum.setDeflection(i, y[i]);
        timeMutex.lock();
            elapsedTime = t;
        timeMutex.unlock();
        pendulumMutex.lock();
            pendulum = newPendulum;
        pendulumMutex.unlock();
    }
    else if (solutionDouble.buffering())
    {
        Pendulum newPendulum = referencePendulum;
        double t = static_cast<double>(timer.elapsed() - refTime) / 1000.0;
        ODE<double>::Y y = solutionDouble(t);
        for (int i = 0; i < newPendulum.weightCnt(); ++i)
            newPendulum.setDeflection(i, y[i]);
        timeMutex.lock();
            elapsedTime = t;
        timeMutex.unlock();
        pendulumMutex.lock();
            pendulum = newPendulum;
        pendulumMutex.unlock();
    }
    flowMutex.unlock();
}

void Canvas::setSegmentCnt(int value)
{
    referencePendulum.setSegmentCnt(value);
    scale = HEIGHT / referencePendulum.totalLength();
    referencePositions = referencePendulum.positions();
}
void Canvas::setSegmentLength(float value)
{
    referencePendulum.setLength(value);
    scale = HEIGHT / referencePendulum.totalLength();
    referencePositions = referencePendulum.positions();
}
void Canvas::setNodeMass(float value) { referencePendulum.setMass(value); }
void Canvas::setNodeRadius(float value) { referencePendulum.setRadius(value); }
void Canvas::setAmplitude(float value) { amplitude = value; }
void Canvas::setAngFrequency(float value) { angFrequency = value; }
void Canvas::setViscosity(float value) { viscosity = value; }
void Canvas::setDensity(float value) { density = value; }

void
Canvas::setIntegrator(QString const& value)
{
    if      (value == "Euler"   ) integrator = EULER;
    else if (value == "RK4"     ) integrator = RK4;
}

void
Canvas::setPrecision(QString const& value)
{
    if      (value == "Float"   ) precision = FLOAT;
    else if (value == "Double"  ) precision = DOUBLE;
}

void
Canvas::setStepExp(int exp)
{
    step = 1.0 / static_cast<double>(1 << exp);
}

void
Canvas::initializeGL()
{
    glClearColor(0.7, 0.5, 0.5, 0.0);
    glShadeModel(GL_FLAT);
}

void
Canvas::resizeGL(
    int w,
    int h
)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    canvasWidth = canvasHeight / static_cast<float>(h) * static_cast<float>(w);
    glOrtho(-0.5f * canvasWidth, 0.5f * canvasWidth, HEIGHT + MARGIN, -MARGIN, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void Canvas::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();   

    if (running())
    {
        pendulumMutex.lock();
            Pendulum paintedPendulum = pendulum;
        pendulumMutex.unlock();
        paintPendulum(paintedPendulum);
        paintTime(elapsedTime);
    }
    else
    {
        paintPendulum(referencePendulum);
        paintTime(0);
    }
}

void
Canvas::paintPendulum(Pendulum const& p)
{
    Pendulum::PolyChain const positions = p.positions();
    
    glScalef(scale, scale, 1.0f);

    glColor3fv(SEGMENT_COLOR);
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i < p.weightCnt(); ++i)
            glVertex2fv(positions[i]);
    glEnd();

    glColor3fv(ANCHOR_COLOR);
    paintCircle(positions[0], p.radius());

    glColor3fv(WEIGHT_COLOR);
    for (int i = 1; i < p.weightCnt(); ++i)
        paintCircle(positions[i], p.radius());

    if (hovering != 0)
    {
        glColor3fv(HOVER_COLOR);
        paintCircle(positions[hovering], p.radius() * RADIUS_FACTOR);
    }
    else if (holding != 0)
    {
        glColor3fv(HOLD_COLOR);
        paintCircle(positions[holding], p.radius() * RADIUS_FACTOR);
    }

    glColor3fv(CROSS_COLOR);
    for (int i = 0; i < p.segmentCnt(); ++i)
        if (std::abs(positions[i].y - positions[i + 1].y) <
            LENGTH_FACTOR * p.length())
            paintCross(0.5f * (positions[i] + positions[i + 1]),
                CROSS_FACTOR * p.length());
}

void
Canvas::paintTime(float t)
{
    t = -t;
    glLoadIdentity();
    glTranslatef(-0.5f * canvasWidth, -MARGIN, 0);
    float offset = SECOND_LENGTH
        * (t - static_cast<float>(static_cast<int>(t / 2.0f) * 2) - 2.0f);
    int n = canvasWidth / SECOND_LENGTH + 2;
    glBegin(GL_QUADS);
        glColor3fv(SECOND_POS_COLOR);
        glVertex2f(0, 0);
        glVertex2f(canvasWidth, 0);
        glVertex2f(canvasWidth, BAR_WIDTH);
        glVertex2f(0, BAR_WIDTH);
        glColor3fv(SECOND_NEG_COLOR);
        for (int k = 0; k < n; ++k)
        {
            float beg = offset + 2.0f * static_cast<float>(k) * SECOND_LENGTH;
            float end = beg + SECOND_LENGTH;
            glVertex2f(beg, 0);
            glVertex2f(end, 0);
            glVertex2f(end, BAR_WIDTH);
            glVertex2f(beg, BAR_WIDTH);
        }
    glEnd();
}

void
Canvas::paintCircle(
    math::float2 const& pos,
    float               radius
)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2fv(pos);
        for (int i = 0; i <= CIRCLE_SIDES; ++i)
        {
            float angle = static_cast<float>(i)
                / static_cast<float>(CIRCLE_SIDES) * 2.0f * math::PI;
            float x = pos.x + radius * std::cos(angle);
            float y = pos.y + radius * std::sin(angle);
            glVertex2f(x, y);
        }
    glEnd();
}

void
Canvas::paintCross(
    math::float2 const& pos,
    float               size
)
{
    size /= 4.0f;
    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0.0f);
    glScalef(size, size, 1.0f);
    glTranslatef(-2.0f, -2.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(1, 0);
        glVertex2f(4, 3);
        glVertex2f(3, 4);
        glVertex2f(0, 1);
        glVertex2f(0, 3);
        glVertex2f(3, 0);
        glVertex2f(4, 1);
        glVertex2f(1, 4);
    glEnd();
    glPopMatrix();
}

inline math::float2
Canvas::toLocal(QPoint const& pos) const
{
    return math::float2(
        pos.x() / static_cast<float>(width()) * canvasWidth - 0.5f * canvasWidth,
        pos.y() / static_cast<float>(height()) * canvasHeight - MARGIN
    ) / scale;
}




CanvasUpdater::CanvasUpdater(Canvas* canvas)
: canvas(canvas)
{/* Do nothing. */}

void
CanvasUpdater::update()
{
    canvas->update();
}

} // namespace jg

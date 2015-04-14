#include "central_widget.hpp"
#include <iostream>

namespace jg {

CentralWidget::CentralWidget(QWidget* parent)
:   QWidget(parent),
    canvas(30, this),
    iface(this)
{
    connect(&iface, SIGNAL(start()), &canvas, SLOT(start()));
    connect(&iface, SIGNAL(stop()), &canvas, SLOT(stop()));
    connect(&iface, SIGNAL(pause()), &canvas, SLOT(pause()));
    connect(&iface, SIGNAL(segmentCountChanged(int)),
        &canvas, SLOT(setSegmentCnt(int)));
    connect(&iface, SIGNAL(segmentLengthChanged(float)),
        &canvas, SLOT(setSegmentLength(float)));
    connect(&iface, SIGNAL(nodeMassChanged(float)),
        &canvas, SLOT(setNodeMass(float)));
    connect(&iface, SIGNAL(nodeRadiusChanged(float)),
        &canvas, SLOT(setNodeRadius(float)));
    connect(&iface, SIGNAL(angFrequencyChanged(float)),
        &canvas, SLOT(setAngFrequency(float)));
    connect(&iface, SIGNAL(amplitudeChanged(float)),
        &canvas, SLOT(setAmplitude(float)));
    connect(&iface, SIGNAL(viscosityChanged(float)),
        &canvas, SLOT(setViscosity(float)));
    connect(&iface, SIGNAL(densityChanged(float)),
        &canvas, SLOT(setDensity(float)));
    connect(&iface, SIGNAL(integratorChanged(QString const&)),
        &canvas, SLOT(setIntegrator(QString const&)));
    connect(&iface, SIGNAL(precisionChanged(QString const&)),
        &canvas, SLOT(setPrecision(QString const&)));
    connect(&iface, SIGNAL(stepChanged(int)),
        &canvas, SLOT(setStepExp(int)));

    iface.broadcast();
}

void
CentralWidget::resizeEvent(QResizeEvent* event)
{
    int w = event->size().width();
    int h = event->size().height();
    canvas.setGeometry(0, 50, w - 300, h - 50);
    iface.setGeometry(w - 300, 0, 300, h);
    iface.flowControl.setGeometry(0, 0, w - 300, 50);
}

} // namespace jg

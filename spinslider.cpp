#include "spinslider.hpp"

namespace jg {

SpinSlider::SpinSlider()
:   slider(Qt::Horizontal),
    adjusting(false)
{
    slider.setTickPosition(QSlider::TicksBelow);
    updateSpinBoxStep();
    addWidget(&slider);
    addWidget(&spinBox);

    connect(&slider, SIGNAL(valueChanged(int)),
        this, SLOT(sliderChanged(int)));
    connect(&spinBox, SIGNAL(valueChanged(double)),
        this, SLOT(spinBoxChanged(double)));
}

void
SpinSlider::setRange(double minimum, double maximum)
{
    spinBox.setRange(minimum, maximum);
    spinBoxChanged(spinBox.value());
    updateSpinBoxStep();
}

void
SpinSlider::setValue(double value)
{
    spinBox.setValue(value);
}

double
SpinSlider::value() const
{
    return spinBox.value();
}

void
SpinSlider::setEnabled(bool state)
{
    slider.setEnabled(state);
    spinBox.setEnabled(state);
}

void
SpinSlider::sliderChanged(int value)
{
    if (adjusting) return;
    double a = static_cast<double>(value) /
        static_cast<double>(slider.maximum() - slider.minimum());
    double dvalue = (1.0 - a) * spinBox.minimum() + a * spinBox.maximum();
    adjusting = true;
    spinBox.setValue(dvalue);
    adjusting = false;
    emit valueChanged(dvalue);
}

void
SpinSlider::spinBoxChanged(double value)
{
    if (adjusting) return;
    double a = (value - spinBox.minimum()) /
        (spinBox.maximum() - spinBox.minimum());
    adjusting = true;
    slider.setValue((1.0 - a) * static_cast<double>(slider.minimum())
        + a * static_cast<double>(slider.maximum()));
    adjusting = false;
    emit valueChanged(value);
}

void
SpinSlider::updateSpinBoxStep()
{
    spinBox.setSingleStep(static_cast<double>(slider.singleStep())
        / static_cast<double>(slider.maximum() - slider.minimum())
        * (spinBox.maximum() - spinBox.minimum()));
}

} // namespace jg

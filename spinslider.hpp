#ifndef JG_SPINSLIDER_HPP
#define JG_SPINSLIDER_HPP

#include <QSlider>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

namespace jg {

class SpinSlider : public QHBoxLayout
{
    Q_OBJECT

public:
    SpinSlider();

    void    setRange(double minimum, double maximum);
    void    setValue(double value);
    double  value() const;
    void    setEnabled(bool state);

signals:
    void valueChanged(double value);

private:
    QSlider         slider;
    QDoubleSpinBox  spinBox;
    bool            adjusting;

    void updateSpinBoxStep();

private slots:
    void sliderChanged(int value);
    void spinBoxChanged(double value);
};

} // namespace jg

#endif // JG_SPINSLIDER

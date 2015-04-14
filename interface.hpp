#ifndef JG_INTERFACE_HPP
#define JG_INTERFACE_HPP

#include <QtGui>
#include <QtWidgets>

#include <vector>
#include "spinslider.hpp"

namespace jg
{

class Interface : public QWidget
{
    Q_OBJECT

public:
    Interface(QWidget* parent = 0);
    void setDefault();
    void broadcast();

    static int const    MIN_SEGMENT_COUNT;
    static int const    MAX_SEGMENT_COUNT;
    static int const    DEFAULT_SEGMENT_COUNT;
    static float const  MIN_SEGMENT_LENGTH;
    static float const  MAX_SEGMENT_LENGTH;
    static float const  DEFAULT_SEGMENT_LENGTH;
    static float const  MIN_NODE_MASS;
    static float const  MAX_NODE_MASS;
    static float const  DEFAULT_NODE_MASS;
    static float const  MIN_NODE_RADIUS;
    static float const  MAX_NODE_RADIUS;
    static float const  DEFAULT_NODE_RADIUS;
    static int const    MAX_RESONANCE_SUPPORT;
    static float const  MIN_ANG_FREQUENCY;
    static float const  MAX_ANG_FREQUENCY;
    static float const  DEFAULT_ANG_FREQUENCY;
    static float const  MIN_AMPLITUDE;
    static float const  MAX_AMPLITUDE;
    static float const  DEFAULT_AMPLITUDE;
    static float const  MIN_VISCOSITY;
    static float const  MAX_VISCOSITY;
    static float const  DEFAULT_VISCOSITY;
    static float const  MIN_DENSITY;
    static float const  MAX_DENSITY;
    static float const  DEFAULT_DENSITY;
    static int const    MIN_STEP_EXP;
    static int const    MAX_STEP_EXP;
    static int const    DEFAULT_STEP_EXP;

    QWidget     flowControl;
    QPushButton startButton;
    QPushButton stopButton;
    QPushButton pauseButton;

    QGroupBox   pendulumPropertiesGroupBox;
    QSpinBox    segmentCountSpinBox;
    SpinSlider  segmentLengthSpinSlider;
    SpinSlider  nodeMassSpinSlider;
    SpinSlider  nodeRadiusSpinSlider;

    QGroupBox   driverPropertiesGroupBox;
    QComboBox   angFrequencyComboBox;
    SpinSlider  angFrequencySpinSlider;
    SpinSlider  amplitudeSpinSlider;

    QGroupBox   fluidPropertiesGroupBox;
    SpinSlider  viscositySpinSlider;
    SpinSlider  densitySpinSlider;

    QGroupBox   integratorPropertiesGroupBox;
    QComboBox   integratorComboBox;
    QComboBox   precisionComboBox;
    QSpinBox    stepSpinBox;

private slots:
    void startButtonClicked();
    void stopButtonClicked();
    void pauseButtonClicked();
    void segmentCountSpinBoxValueChanged(int value);
    void segmentLengthSpinSliderValueChanged(double value);
    void nodeMassSpinSliderValueChanged(double value);
    void nodeRadiusSpinSliderValueChanged(double value);
    void angFrequencyComboBoxCurrentIndexChanged(int value);
    void angFrequencySpinSliderValueChanged(double value);
    void amplitudeSpinSliderValueChanged(double value);
    void viscositySpinSliderValueChanged(double value);
    void densitySpinSliderValueChanged(double value);
    void integratorComboBoxCurrentIndexChanged(QString const& value);
    void precisionComboBoxCurrentIndexChanged(QString const& value);
    void stepSpinBoxValueChanged(int value);

signals:
    void start();
    void stop();
    void pause();
    void segmentCountChanged(int);
    void segmentLengthChanged(float);
    void nodeMassChanged(float);
    void nodeRadiusChanged(float);
    void angFrequencyChanged(float);
    void amplitudeChanged(float);
    void viscosityChanged(float);
    void densityChanged(float);
    void integratorChanged(QString const&);
    void precisionChanged(QString const&);
    void stepChanged(int);

private:
    std::vector<std::vector<double> > lambdas;
};

} // namespace jg

#endif // JG_INTERFACE_HPP

#include "interface.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

namespace jg
{

int const   Interface::MIN_SEGMENT_COUNT        = 1;
int const   Interface::MAX_SEGMENT_COUNT        = 128;
int const   Interface::DEFAULT_SEGMENT_COUNT    = 3;
float const Interface::MIN_SEGMENT_LENGTH       = 1;
float const Interface::MAX_SEGMENT_LENGTH       = 8;
float const Interface::DEFAULT_SEGMENT_LENGTH   = 2;
float const Interface::MIN_NODE_MASS            = 0.1;
float const Interface::MAX_NODE_MASS            = 5;
float const Interface::DEFAULT_NODE_MASS        = 1;
float const Interface::MIN_NODE_RADIUS          = 0.05;
float const Interface::MAX_NODE_RADIUS          = 0.5;
float const Interface::DEFAULT_NODE_RADIUS      = 0.25;
int const   Interface::MAX_RESONANCE_SUPPORT    = 9;
float const Interface::MIN_ANG_FREQUENCY        = 0;
float const Interface::MAX_ANG_FREQUENCY        = 50;
float const Interface::DEFAULT_ANG_FREQUENCY    = 20;
float const Interface::MIN_AMPLITUDE            = 0;
float const Interface::MAX_AMPLITUDE            = 0.5;
float const Interface::DEFAULT_AMPLITUDE        = 0;
float const Interface::MIN_VISCOSITY            = 0;
float const Interface::MAX_VISCOSITY            = 5;
float const Interface::DEFAULT_VISCOSITY        = 0;
float const Interface::MIN_DENSITY              = 0;
float const Interface::MAX_DENSITY              = 200;
float const Interface::DEFAULT_DENSITY          = 0;
int const   Interface::MIN_STEP_EXP             = 6;
int const   Interface::MAX_STEP_EXP             = 16;
int const   Interface::DEFAULT_STEP_EXP         = 9;


Interface::Interface(QWidget* parent)
:   QWidget(parent),
    
    flowControl(parent),
    startButton("Start"),
    stopButton("Stop"),
    pauseButton("Pause"),
    pendulumPropertiesGroupBox("Pendulum properties"),
    driverPropertiesGroupBox("Driver properties."),
    fluidPropertiesGroupBox("Fluid properties"),
    integratorPropertiesGroupBox("Integrator properties"),
    lambdas(MAX_RESONANCE_SUPPORT + 1)
{
    lambdas[1].push_back(1.000000000000);
    
    lambdas[2].push_back(0.585786437627);
    lambdas[2].push_back(3.414213562373);
    
    lambdas[3].push_back(0.415774556783);
    lambdas[3].push_back(2.294280360279);
    lambdas[3].push_back(6.289945082937);
    
    lambdas[4].push_back(0.322547689619);
    lambdas[4].push_back(1.745761101158);
    lambdas[4].push_back(4.536620296921);
    lambdas[4].push_back(9.395070912301);
    
    lambdas[5].push_back(.263560319718); 
    lambdas[5].push_back(1.413403059107);
    lambdas[5].push_back(3.596425771041);
    lambdas[5].push_back(7.085810005859);
    lambdas[5].push_back(12.640800844276);

    lambdas[6].push_back(0.222846604179);
    lambdas[6].push_back(1.188932101673); 
    lambdas[6].push_back(2.992736326059);
    lambdas[6].push_back(5.775143569105);
    lambdas[6].push_back(9.837467418383); 
    lambdas[6].push_back(15.982873980602);

    lambdas[7].push_back(0.193043676560); 
    lambdas[7].push_back(1.026664895339);
    lambdas[7].push_back(2.567876744951); 
    lambdas[7].push_back(4.900353084526); 
    lambdas[7].push_back(8.182153444563);
    lambdas[7].push_back(12.734180291798);
    lambdas[7].push_back(19.395727862263);

    lambdas[8].push_back(0.170279632305); 
    lambdas[8].push_back(0.903701776799); 
    lambdas[8].push_back(2.251086629866); 
    lambdas[8].push_back(4.266700170288); 
    lambdas[8].push_back(7.045905402393); 
    lambdas[8].push_back(10.758516010181); 
    lambdas[8].push_back(15.740678641278); 
    lambdas[8].push_back(22.863131736889); 
    
    lambdas[9].push_back(0.152322227732); 
    lambdas[9].push_back(0.807220022742); 
    lambdas[9].push_back(2.005135155619); 
    lambdas[9].push_back(3.783473973331); 
    lambdas[9].push_back(6.204956777877); 
    lambdas[9].push_back(9.372985251688); 
    lambdas[9].push_back(13.466236911092); 
    lambdas[9].push_back(18.833597788992); 
    lambdas[9].push_back(26.374071890927);

    connect(&startButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
    connect(&stopButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));
    connect(&pauseButton, SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));
    connect(&segmentCountSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(segmentCountSpinBoxValueChanged(int)));
    connect(&segmentLengthSpinSlider, SIGNAL(valueChanged(double)),
        this, SLOT(segmentLengthSpinSliderValueChanged(double)));
    connect(&nodeMassSpinSlider, SIGNAL(valueChanged(double)),
        this, SLOT(nodeMassSpinSliderValueChanged(double)));
    connect(&nodeRadiusSpinSlider, SIGNAL(valueChanged(double)),
        this, SLOT(nodeRadiusSpinSliderValueChanged(double)));
    connect(&angFrequencyComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(angFrequencyComboBoxCurrentIndexChanged(int)));
    connect(&angFrequencySpinSlider, SIGNAL(valueChanged(double)),
        this, SLOT(angFrequencySpinSliderValueChanged(double)));
    connect(&amplitudeSpinSlider, SIGNAL(valueChanged(double)),
        this, SLOT(amplitudeSpinSliderValueChanged(double)));
    connect(&viscositySpinSlider, SIGNAL(valueChanged(double)),
        this, SLOT(viscositySpinSliderValueChanged(double)));
    connect(&densitySpinSlider, SIGNAL(valueChanged(double)),
        this, SLOT(densitySpinSliderValueChanged(double)));
    connect(&integratorComboBox, SIGNAL(currentIndexChanged(QString const&)),
        this, SLOT(integratorComboBoxCurrentIndexChanged(QString const&)));
    connect(&precisionComboBox, SIGNAL(currentIndexChanged(QString const&)),
        this, SLOT(precisionComboBoxCurrentIndexChanged(QString const&)));
    connect(&stepSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(stepSpinBoxValueChanged(int)));

    angFrequencyComboBox.addItem("Custom");
    
    QVBoxLayout* layout = new QVBoxLayout;

    /* Flow control. */
    stopButton.setEnabled(false);
    pauseButton.setEnabled(false);
    QHBoxLayout* flowControlHLayout = new QHBoxLayout;
    QVBoxLayout* flowControlLayout = new QVBoxLayout;
    flowControlHLayout->addWidget(&startButton);
    flowControlHLayout->addWidget(&stopButton);
    flowControlHLayout->addWidget(&pauseButton);
    flowControlLayout->addLayout(flowControlHLayout);
    flowControl.setLayout(flowControlLayout);

    /* Pendulum properities. */
    QVBoxLayout* pendulumPropertiesLayout = new QVBoxLayout;
    QHBoxLayout* segmentCountLayout = new QHBoxLayout();
    segmentCountSpinBox.setRange(MIN_SEGMENT_COUNT, MAX_SEGMENT_COUNT);
    segmentCountLayout->addWidget(new QLabel("Segment count"));
    segmentCountLayout->addWidget(&segmentCountSpinBox);
    pendulumPropertiesLayout->addLayout(segmentCountLayout);
    pendulumPropertiesLayout->addWidget(new QLabel("Segment length"));
    segmentLengthSpinSlider.setRange(MIN_SEGMENT_LENGTH, MAX_SEGMENT_LENGTH);
    pendulumPropertiesLayout->addLayout(&segmentLengthSpinSlider);
    pendulumPropertiesLayout->addWidget(new QLabel("Node mass"));
    nodeMassSpinSlider.setRange(MIN_NODE_MASS, MAX_NODE_MASS);
    pendulumPropertiesLayout->addLayout(&nodeMassSpinSlider);
    pendulumPropertiesLayout->addWidget(new QLabel("Node radius"));
    nodeRadiusSpinSlider.setRange(MIN_NODE_RADIUS, MAX_NODE_RADIUS);
    pendulumPropertiesLayout->addLayout(&nodeRadiusSpinSlider);
    pendulumPropertiesGroupBox.setLayout(pendulumPropertiesLayout);

    /* Driver properties. */
    QVBoxLayout* driverPropertiesLayout = new QVBoxLayout;
    QHBoxLayout* angFrequencyLayout = new QHBoxLayout;
    angFrequencyLayout->addWidget(new QLabel("Angluar frequency"));
    angFrequencyLayout->addWidget(&angFrequencyComboBox);
    angFrequencySpinSlider.setRange(MIN_ANG_FREQUENCY, MAX_ANG_FREQUENCY);
    amplitudeSpinSlider.setRange(MIN_AMPLITUDE, MAX_AMPLITUDE);
    driverPropertiesLayout->addLayout(angFrequencyLayout);
    driverPropertiesLayout->addLayout(&angFrequencySpinSlider);
    driverPropertiesLayout->addWidget(new QLabel("Amplitude"));
    driverPropertiesLayout->addLayout(&amplitudeSpinSlider);
    driverPropertiesGroupBox.setLayout(driverPropertiesLayout);

    /* Fluid properties. */
    QVBoxLayout* fluidPropertiesLayout = new QVBoxLayout;
    viscositySpinSlider.setRange(MIN_VISCOSITY, MAX_VISCOSITY);
    densitySpinSlider.setRange(MIN_DENSITY, MAX_DENSITY);
    fluidPropertiesLayout->addWidget(new QLabel("Viscosity"));
    fluidPropertiesLayout->addLayout(&viscositySpinSlider);
    fluidPropertiesLayout->addWidget(new QLabel("Density"));
    fluidPropertiesLayout->addLayout(&densitySpinSlider);
    fluidPropertiesGroupBox.setLayout(fluidPropertiesLayout);

    /* Integrator properties. */
    QVBoxLayout* integratorPropertiesLayout = new QVBoxLayout;
    QHBoxLayout* integratorLayout = new QHBoxLayout;
    integratorLayout->addWidget(new QLabel("Integrator"));
    integratorComboBox.addItem("Euler");
    integratorComboBox.addItem("RK4");
    integratorLayout->addWidget(&integratorComboBox);
    QHBoxLayout* precisionLayout = new QHBoxLayout;
    precisionLayout->addWidget(new QLabel("Precision"));
    precisionComboBox.addItem("Float");
    precisionComboBox.addItem("Double");
    precisionLayout->addWidget(&precisionComboBox);
    QHBoxLayout* stepLayout = new QHBoxLayout;
    stepSpinBox.setRange(MIN_STEP_EXP, MAX_STEP_EXP);
    stepSpinBox.setPrefix("2^-");
    stepLayout->addWidget(new QLabel("Step"));
    stepLayout->addWidget(&stepSpinBox);
    integratorPropertiesLayout->addLayout(integratorLayout);
    integratorPropertiesLayout->addLayout(precisionLayout);
    integratorPropertiesLayout->addLayout(stepLayout);
    integratorPropertiesGroupBox.setLayout(integratorPropertiesLayout);

    //layout->addWidget(&flowControlGroupBox);
    layout->addWidget(&pendulumPropertiesGroupBox);
    layout->addWidget(&driverPropertiesGroupBox);
    layout->addWidget(&fluidPropertiesGroupBox);
    layout->addWidget(&integratorPropertiesGroupBox);
    setLayout(layout);

    setDefault();
}

void
Interface::setDefault()
{
    segmentCountSpinBox.setValue(DEFAULT_SEGMENT_COUNT);
    segmentLengthSpinSlider.setValue(DEFAULT_SEGMENT_LENGTH);
    nodeMassSpinSlider.setValue(DEFAULT_NODE_MASS);
    nodeRadiusSpinSlider.setValue(DEFAULT_NODE_RADIUS);
    angFrequencySpinSlider.setValue(DEFAULT_ANG_FREQUENCY);
    amplitudeSpinSlider.setValue(DEFAULT_AMPLITUDE);
    viscositySpinSlider.setValue(DEFAULT_VISCOSITY);
    densitySpinSlider.setValue(DEFAULT_DENSITY);
    integratorComboBox.setCurrentIndex(0);
    precisionComboBox.setCurrentIndex(0);
    stepSpinBox.setValue(DEFAULT_STEP_EXP);
}

void
Interface::broadcast()
{
    emit segmentCountChanged(segmentCountSpinBox.value());
    emit segmentLengthChanged(segmentLengthSpinSlider.value());
    emit nodeMassChanged(nodeMassSpinSlider.value());
    emit nodeRadiusChanged(nodeRadiusSpinSlider.value());
    emit angFrequencyChanged(angFrequencySpinSlider.value());
    emit amplitudeChanged(amplitudeSpinSlider.value());
    emit viscosityChanged(viscositySpinSlider.value());
    emit densityChanged(densitySpinSlider.value());
    emit integratorChanged(integratorComboBox.itemText(
        integratorComboBox.currentIndex()));
    emit precisionChanged(precisionComboBox.itemText(
        precisionComboBox.currentIndex()));
    emit stepChanged(stepSpinBox.value());
}

/* Flow control. */

void
Interface::startButtonClicked()
{
    startButton.setEnabled(false);
    stopButton.setEnabled(true);
    pauseButton.setEnabled(true);
    pendulumPropertiesGroupBox.setEnabled(false);
    driverPropertiesGroupBox.setEnabled(false);
    fluidPropertiesGroupBox.setEnabled(false);
    integratorPropertiesGroupBox.setEnabled(false);
    emit start();
}

void
Interface::stopButtonClicked()
{
    startButton.setEnabled(true);
    stopButton.setEnabled(false);
    pauseButton.setEnabled(false);
    pendulumPropertiesGroupBox.setEnabled(true);
    driverPropertiesGroupBox.setEnabled(true);
    fluidPropertiesGroupBox.setEnabled(true);
    integratorPropertiesGroupBox.setEnabled(true);
    emit stop();
}

void
Interface::pauseButtonClicked()
{
    startButton.setEnabled(true);
    stopButton.setEnabled(true);
    pauseButton.setEnabled(false);
    emit pause();
}

void Interface::segmentCountSpinBoxValueChanged(int value)
{
    emit segmentCountChanged(value);

    angFrequencyComboBox.setCurrentIndex(0);
    angFrequencySpinSlider.setEnabled(true);
    if (value > MAX_RESONANCE_SUPPORT)
        angFrequencyComboBox.setEnabled(false);
    else
    {
        angFrequencyComboBox.setEnabled(true);
        while (angFrequencyComboBox.count() > value + 1)
            angFrequencyComboBox.removeItem(angFrequencyComboBox.count() - 1);
        while (angFrequencyComboBox.count() <= value)
            angFrequencyComboBox.addItem("Resonance "
                + QString::number(angFrequencyComboBox.count()));
    }
}

void Interface::segmentLengthSpinSliderValueChanged(double value)
{ emit segmentLengthChanged(value); }

void Interface::nodeMassSpinSliderValueChanged(double value)
{ emit nodeMassChanged(value); }

void Interface::nodeRadiusSpinSliderValueChanged(double value)
{ emit nodeRadiusChanged(value); }

void Interface::angFrequencyComboBoxCurrentIndexChanged(int value)
{
    if (value > 0)
    {
        angFrequencySpinSlider.setEnabled(false);
        angFrequencySpinSlider.setValue(
            std::sqrt(lambdas[segmentCountSpinBox.value()][value - 1]
            * 9.81 / segmentLengthSpinSlider.value() )
        );
    }
    else angFrequencySpinSlider.setEnabled(true);
}

void Interface::angFrequencySpinSliderValueChanged(double value)
{ emit angFrequencyChanged(value); }

void Interface::amplitudeSpinSliderValueChanged(double value)
{ emit amplitudeChanged(value); }

void Interface::viscositySpinSliderValueChanged(double value)
{ emit viscosityChanged(value); }

void Interface::densitySpinSliderValueChanged(double value)
{ emit densityChanged(value); }

void Interface::integratorComboBoxCurrentIndexChanged(QString const& value)
{ emit integratorChanged(value); }

void Interface::precisionComboBoxCurrentIndexChanged(QString const& value)
{ emit precisionChanged(value); }

void Interface::stepSpinBoxValueChanged(int value)
{ emit stepChanged(value); }

} // namespace jg

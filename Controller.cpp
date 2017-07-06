#include "Controller.h"
//

Controller::Controller() : dropButton(PORT_PIN(3, 5)), spinButton(PORT_PIN(5, 1)), joy_X(15)
{
    dropButton.gpioMode(GPIO::INPUT | GPIO::PULLDOWN); //S1
    spinButton.gpioMode(GPIO::INPUT | GPIO::PULLDOWN); //S2
    // configure channel with 10 bit resolution
    joy_X.adcMode(ADC::ADC_10_BIT);
    joy_X_Voffset = joy_X.adcReadVoltage();
}

bool Controller::dropButtonPressed()
{
    return dropButton.gpioRead();
}

bool Controller::spinButtonPressed()
{
    return spinButton.gpioRead();
}

int Controller::isMoving()
{
    if ((joy_X.adcReadVoltage() - joy_X_Voffset) < -0.2)
        return -1; //left
    if ((joy_X.adcReadVoltage() - joy_X_Voffset) > 0.2)
        return 1; //right

    return 0; //nothing
}

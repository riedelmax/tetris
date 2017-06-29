#include "Controller.h"

Controller::Controller()
{
    dropButton = gpio_msp432_pin(PORT_PIN(1, 0));
    spinButton = gpio_msp432_pin(PORT_PIN(1, 0));

    dropButton.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
    spinButton.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);

    joy_X = adc14_msp432_channel(15);
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
    if ((volt - volt_offset) < -0.2)
        return -1; //left
    if ((volt - volt_offset) > 0.2)
        return 1; //right

    return 0; //nothing
}

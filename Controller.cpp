#include "Controller.h"

Controller::Controller()
{
    dropButton = gpio_msp432_pin(PORT_PIN(3, 5)); //S2 Button
    spinButton = gpio_msp432_pin(PORT_PIN(5, 1)); //S1 Button

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

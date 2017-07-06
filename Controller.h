#ifndef CONTROLLER_H
#define CONTROLLER_H
//

#include "gpio_msp432.h"
#include "adc14_msp432.h"

class Controller
{
    private:
        gpio_msp432_pin dropButton;
        gpio_msp432_pin spinButton;
        adc14_msp432_channel joy_X;
        float joy_X_Voffset; //VoltageOffset

    public:
        Controller();
        bool dropButtonPressed();
        bool spinButtonPressed();
        int isMoving();
};

#endif /* CONTROLLER_H */

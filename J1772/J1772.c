//
//  J1772.c
//  J1772
//
//  Created by Florian Uhlemann on 29.10.16.
//  Copyright © 2016 Florian Uhlemann. All rights reserved.
//

#include <stdio.h>
#include "J1772.h"

/// Variable, if PWM should be running or not.
volatile bool PWM_is_running = false;

/// Conversion between duty cycle percentage (input) to
/// current amps (output). Includes the switch of formulae
/// around 80% for high amperages.
/// double percent
/// int amps
int percent_to_amps(double percent) {
    if (percent <= (1600.0/19.0)) {
        return 0.6 * percent;
    } else {
        return (percent - 64.0) * 2.5;
    }
}

/// Conversion between current amps (output) to duty cycle
/// percentage (input). Includes the switch of formulae
/// around 80% for high amperages.
/// double percent
/// int amps
double amps_to_percent(int amps) {
    if (amps <= (960/19)) {
        return amps / 0.6;
    } else {
        return amps / 2.5 + 64;
    }
}

/// Determining the current vehicle state by analyzing the
/// given voltages for upper segment, lower segment and the
/// reference voltage. Tolerance levels included.
/// Lower segment voltage (-12V) shall be given in positive numbers.
VehicleState detectVehicleState(double upperVoltage, double lowerVoltage, double referenceVoltage) {
    
    bool plus_12v  = false,
    plus_9v   = false,
    plus_6v   = false,
    plus_3v   = false,
    minus_12v = false;
    
    if (((upperVoltage / referenceVoltage * 12.0) <= 12.6) && ((upperVoltage / referenceVoltage * 12.0 ) >= 11.4)) {
        //printf("Upper voltage is between 11.4V and 12.6V. Vehicle is disconnected.\n");
        plus_12v = true;
    } else {
        if (((upperVoltage / referenceVoltage * 12.0) <= 9.56) && ((upperVoltage / referenceVoltage * 12.0 ) >= 8.36)) {
            //printf("Upper voltage is between 8.36V and 9.56V. Vehicle is connected.\n");
            plus_9v = true;
            if (PWM_is_running == false) {
                //start PWM
                PWM_is_running = true;
            }
        }
        if (((upperVoltage / referenceVoltage * 12.0) <= 6.49) && ((upperVoltage / referenceVoltage * 12.0 ) >= 5.48)) {
            //printf("Upper voltage is between 5.48V and 6.49V. Vehicle is charging.\n");
            plus_6v = true;
        }
        if (((upperVoltage / referenceVoltage * 12.0) <= 3.25) && ((upperVoltage / referenceVoltage * 12.0 ) >= 2.62)) {
            //printf("Upper voltage is between 2.62V and 3.25V. Vehicle requests ventilation.\n");
            plus_3v = true;
        }
        if (((lowerVoltage / referenceVoltage * 12.0) <= 12.6) && ((lowerVoltage / referenceVoltage * 12.0 ) >= 11.4)) {
            //printf("Lower voltage is between -11.4V and -12.6V. PWM is active.\n");
            minus_12v = true;
        } else {
            //printf("Lower voltage is outside of envelope. Error detected.\n");
        }
    }
    
    int binaryValue = plus_12v << 0 | plus_9v << 1 | plus_6v << 2 | plus_3v << 3 | minus_12v << 4;
    
    switch (binaryValue) {
        case 0b00001:
            printf("State A - Vehicle disconnected. No PWM.\n");
            //stop PWM
            PWM_is_running = false;
            break;
        case 0b00010:
            printf("State AB - Vehicle connected. PWM not started yet.\n");
            //start PWM
            PWM_is_running = true;
            break;
        case 0b10010:
            printf("State B - Vehicle connected. PWM active.\n");
            break;
        case 0b10100:
            printf("State C - Vehicle charging. PWM active.\n");
            break;
        case 0b11000:
            printf("State D - Vehicle with ventilation. PWM active.\n");
            break;
        default:
            printf("State F - Unknown condition or error occured.\n");
            break;
    }
    
    
    VehicleState myState;
    myState = connected;
    return myState;
}

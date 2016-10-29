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
    plus_9v     = false,
    plus_6v     = false,
    plus_3v     = false,
    minus_12v   = false,
    minus_fault = false;
    
    //Checking if upper voltage = 12.00
    if (((upperVoltage / referenceVoltage * 12.0) <= 12.6) && ((upperVoltage / referenceVoltage * 12.0 ) >= 11.4)) {
        plus_12v = true;
        if (PWM_is_running) {
            PWM_is_running = false;
        }
    }
    //Checking if upper voltage = 9.00
    if (((upperVoltage / referenceVoltage * 12.0) <= 9.56) && ((upperVoltage / referenceVoltage * 12.0 ) >= 8.36)) {
        plus_9v = true;
    }
    //Checking if upper voltage = 6.00
    if (((upperVoltage / referenceVoltage * 12.0) <= 6.49) && ((upperVoltage / referenceVoltage * 12.0 ) >= 5.48)) {
        plus_6v = true;
    }
    //Checking if upper voltage = 3.00
    if (((upperVoltage / referenceVoltage * 12.0) <= 3.25) && ((upperVoltage / referenceVoltage * 12.0 ) >= 2.62)) {
        plus_3v = true;
    }
    //Checking if lower voltage = -12.00
    //Special Case: If upper voltage is NOT +12.0V, then lower voltage must not be other than -12V. (PWM active)
    //Special Case: Transition from no PWM to PWM phase is tricky when checking lower voltage.
    if (((lowerVoltage / referenceVoltage * 12.0) <= 12.6) && ((lowerVoltage / referenceVoltage * 12.0 ) >= 11.4)) {
        minus_12v = true;
    } else if (PWM_is_running) {
        if (PWM_is_running) {
            minus_fault = true;
        } else {
            //turn PWM on
            PWM_is_running = true;
        }
    }
    
    int binaryValue = plus_12v << 0 | plus_9v << 1 | plus_6v << 2 | plus_3v << 3 | minus_12v << 4 | PWM_is_running << 5;
    
    switch (binaryValue) {
        case 0b000001:
        case 0b010001:
            printf("State A - Vehicle disconnected. No PWM.\n");
            PWM_is_running = false;
            return disconnected;
            break;
        case 0b000010:
        case 0b010010:
            printf("State AB - Vehicle connected. PWM not started yet.\n");
            PWM_is_running = true;
            return connected_no_pwm;
            break;
        case 0b110010:
            printf("State B - Vehicle connected. PWM active.\n");
            return connected;
            break;
        case 0b110100:
            printf("State C - Vehicle charging. PWM active.\n");
            return charging;
            break;
        case 0b111000:
            printf("State D - Vehicle with ventilation. PWM active.\n");
            return ventilation;
            break;
        default:
            printf("State F - Unknown condition or error occured.\n");
            return fault;
            break;
    }
}

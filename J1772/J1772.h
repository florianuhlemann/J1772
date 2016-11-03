//
//  J1772.h
//  J1772
//
//  Created by Florian Uhlemann on 29.10.16.
//  Copyright © 2016 Florian Uhlemann. All rights reserved.
//

#ifndef J1772_h
#define J1772_h

/// The following voltages are taken from SAE J1772 document
/// and include tolerances. They are a suggestion and may
/// require tweaking, if incompatibilities with vehicles exist.
#define CPILOTREFERENCE 12.00
#define STATE_A_MAXIMUM 12.60
#define STATE_A_MINIMUM 11.40
#define STATE_B_MAXIMUM 9.56
#define STATE_B_MINIMUM 8.36
#define STATE_C_MAXIMUM 6.49
#define STATE_C_MINIMUM 5.48
#define STATE_D_MAXIMUM 3.25
#define STATE_D_MINIMUM 2.62
#define NEGATIVEMINIMUM 11.40
#define NEGATIVEMAXIMUM 12.60

int percent_to_amps(double percent);
double amps_to_percent(int amps);

/// There are five defined states the vehicle can be in.
/// A  - disconnected
/// AB - connected, but no PWM started yet
/// B  - connected, but not charging
/// C  - connected and charging
/// D  - connected, charging and requesting ventilation
/// F  - fault
typedef enum {
    disconnected = 0xA0,
    connected_no_pwm = 0xB1,
    connected = 0xC2,
    charging = 0xD3,
    ventilation = 0xE4,
    fault = 0xF5
} VehicleState;

typedef enum {
    false = 0,
    true = 1
} bool;

VehicleState detectVehicleState(double upperVoltage, double lowerVoltage, double referenceVoltage);

#endif /* J1772_h */

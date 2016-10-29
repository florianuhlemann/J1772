//
//  J1772.h
//  J1772
//
//  Created by Florian Uhlemann on 29.10.16.
//  Copyright © 2016 Florian Uhlemann. All rights reserved.
//

#ifndef J1772_h
#define J1772_h

int percent_to_amps(double percent);
double amps_to_percent(int amps);

/// There are five defined states the vehicle can be in.
/// A - disconnected
/// B - connected, but not charging
/// C - connected and charging
/// D - connected, charging and requesting ventilation
/// F - fault
typedef enum {
    disconnected = 0xA0,
    connected = 0xB1,
    charging = 0xC2,
    ventilation = 0xD3,
    fault = 0xE4
} VehicleState;

typedef enum {
    false = 0,
    true = 1
} bool;

VehicleState detectVehicleState(double upperVoltage, double lowerVoltage, double referenceVoltage);

#endif /* J1772_h */

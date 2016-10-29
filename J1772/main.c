//
//  main.c
//  J1772
//
//  Created by Florian Uhlemann on 29.10.16.
//  Copyright © 2016 Florian Uhlemann. All rights reserved.
//

#include <stdio.h>
#include "J1772.h"

int main(int argc, const char * argv[]) {
    
    detectVehicleState(4.95, 0.00, 5.0); //A
    detectVehicleState(3.73, 0.00, 5.0); //AB only when coming from A directly, not when going back.
    detectVehicleState(3.73, 0.00, 5.0); //AB only when coming from A directly, not when going back.
    detectVehicleState(3.72, 4.96, 5.0); //B
    detectVehicleState(2.48, 4.96, 5.0); //C
    detectVehicleState(1.23, 4.96, 5.0); //D
    detectVehicleState(4.27, 4.27, 5.0); //F
    detectVehicleState(4.98, 4.65, 5.0); //A
    detectVehicleState(3.78, 4.28, 5.0); //F should not be AB, but is recognized as such.
    detectVehicleState(4.99, 4.99, 5.0); //A
    
    return 0;
}

/*
 * bumperMovement.c
 *
 *  Created on: Oct 15, 2022
 *      Author: kaiheng9
 */

#include "bumperMovement.h"
#include "Movement.h"
#include "Timer.h"


void collisionBumpLeft(oi_t*sensor){
    //MOVE BACKWARD
    oi_setWheels(-100,-100);
    oi_moveBackward(sensor, -10);
    oi_setWheels(0,0);

    //TURN CLOCKWISE WITH 90 DEG
    oi_setWheels(-50, 50);
    oi_turnClockwise(sensor, -82);
    timer_waitMillis(2);
    oi_setWheels(0,0);

    //MOVE FORWARD
    oi_setWheels(100,100);
    oi_moveForward(sensor, 15);
    oi_setWheels(0,0);

    //TURN COUNTERCLOCKWISE WITH 90 DEG
    oi_setWheels(50, -50);
    oi_turnCounterClockwise(sensor, 82);
    timer_waitMillis(2);
    oi_setWheels(0,0);

    oi_setWheels(100,100);
    oi_moveForward(sensor, 10);
    oi_setWheels(0,0);
}

// Collision occurs with right bumper
void collisionBumpRight(oi_t*sensor){
    //MOVE BACKWARD
    oi_setWheels(-100,-100);
    oi_moveBackward(sensor, -10);
    oi_setWheels(0,0);

    //TURN COUNTERCLOCKWISE WITH 90 DEG
    oi_setWheels(50, -50);
    oi_turnCounterClockwise(sensor, 82);
    timer_waitMillis(2);
    oi_setWheels(0,0);

    //MOVE FORWARD
    oi_setWheels(100,100);
    oi_moveForward(sensor, 15);
    oi_setWheels(0,0);

    //TURN CLOCKWISE WITH 90 DEG
    oi_setWheels(-50, 50);
    oi_turnClockwise(sensor, -82);
    timer_waitMillis(2);
    oi_setWheels(0,0);

    oi_setWheels(100,100);
    oi_moveForward(sensor, 10);
    oi_setWheels(0,0);
}

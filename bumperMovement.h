/*
 * bumperMovement.h
 *
 *  Created on: Oct 15, 2022
 *      Author: kaiheng9
 */

#ifndef BUMPERMOVEMENT_H_
#define BUMPERMOVEMENT_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "Movement.h"

// Collision occurs with left bumper
void collisionBumpLeft(oi_t*sensor);

// Collision occurs with right bumper
void collisionBumpRight(oi_t*sensor);

#endif /* BUMPERMOVEMENT_H_ */

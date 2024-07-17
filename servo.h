/*
 * servo.h
 *
 *  Created on: Nov 2, 2022
 *      Author: kaiheng9
 */

#ifndef SERVO_H_
#define SERVO_H_


volatile signed pulse_width;


void servo_init(void);

void servo_move(float);



#endif /* SERVO_H_ */

/*
 * ping.h
 *
 *  Created on: Oct 26, 2022
 *      Author: kaiheng9
 */

#ifndef PING_H_
#define PING_H_

void ping_sensor_init(void);

int ping_read(void);

void ping_sendPulse(void);

void TIMER3B_Interrupt_Handler(void);

void ping_interrupt_init(void);

#endif /* PING_H_ */

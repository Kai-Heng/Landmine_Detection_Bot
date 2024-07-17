/*
 * servo.c
 *
 *  Created on: Nov 2, 2022
 *      Author: kaiheng9
 */




#include "servo.h"
#include "Timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"


volatile enum {LOW, HIGH, DONE} state; // set by ISR
volatile unsigned int rising_time; //Pulse start time: Set by ISR
volatile unsigned int falling_time; //Pulse end time: Set by ISR


void servo_init(){
    SYSCTL_RCGCGPIO_R |= 0x2; //enable port B



    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x700000;

    GPIO_PORTB_DEN_R |= 0x20; //enable wire 5
    GPIO_PORTB_DIR_R |= 0x20; //set PB5 as output


    SYSCTL_RCGCTIMER_R |= 0x2;

    TIMER1_CTL_R &= ~(0x0100); //disable clock
    TIMER1_CFG_R |= 0x4;

    TIMER1_TBMR_R |= 0xA;
    TIMER1_TBMR_R &= ~(0x4);

    //set to non-inverted PWM mode
    TIMER1_CTL_R &= 0xBFFF;

    // set lower 16 bits of interval
    TIMER1_TBILR_R |= (320000 & 0xFFFF);

    //set upper 8 bits of interval
    TIMER1_TBPR_R |= (320000 >> 16);

    pulse_width = 0;
    servo_move(pulse_width);

    // set lower 16 bits of pulse width
    TIMER1_TBMATCHR_R |= ((320000 - pulse_width) & 0xFFFF);

    //set upper 8 bits of pulse width
    TIMER1_TBPMR_R |= ((320000- pulse_width) >> 16);

    TIMER1_CTL_R |= 0x0100; //enable
}

void servo_move(float degree){
//    TIMER1_TBILR_R = 20000;
    float ratio = 28500/180;
    pulse_width = ratio*degree;
//
//    TIMER1_TBMATCHR_R = TIMER1_TBILR_R - pulseWidth;
//
//    while((TIMER1_TBILR_R - TIMER1_TBMATCHR_R) != pulseWidth){
//        GPIO_PORTB_AFSEL_R &= 0xDF;
//        GPIO_PORTB_DIR_R |= 0x20;
//        GPIO_PORTB_DATA_R = 0x8;
//    }
//    GPIO_PORTB_DATA_R = 0x0;
//    GPIO_PORTB_AFSEL_R |= 0x20;

    //set lower 16 bits of pulse width
    TIMER1_TBMATCHR_R = ((320000 - pulse_width) & 0xFFFF);

    //set the upper 8 bits of the pulse width
    TIMER1_TBPMR_R |= ((320000 - pulse_width) >> 16);

//    GPIO_PORTB_AFSEL_R &= 0x0F;
//    GPIO_PORTB_DIR_R |= 0x20;
//    GPIO_PORTB_DATA_R |= 0x8;
//    timer_waitMicros(pulse_width);
//    GPIO_PORTB_DATA_R = 0x0;
//    GPIO_PORTB_AFSEL_R |= 0x20;


    timer_waitMillis(50);

}

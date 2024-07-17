/*
 * ping.c
 *
 *  Created on: Oct 26, 2022
 *      Author: kaiheng9
 */

#include "ping.h"
#include "Timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"


volatile enum {LOW, HIGH, DONE} state; // set by ISR
volatile unsigned int rising_time; //Pulse start time: Set by ISR
volatile unsigned int falling_time; //Pulse end time: Set by ISR


void ping_sensor_init(){
    SYSCTL_RCGCGPIO_R |= 0x2; //enable port B


    GPIO_PORTB_DEN_R |= 0x8; //enable wire 3
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x00007000;


    SYSCTL_RCGCTIMER_R |= 0x8;
    TIMER3_CTL_R &= ~0x100;
    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R |= 0x7;
    TIMER3_TBILR_R = 0xFFFF;
    TIMER3_TBPR_R = 0xFF;
    TIMER3_CTL_R |= 0x100;

    // Enable interrupts for receiving bytes through UART1
     //enable interrupt on capture event mode - page 924
    TIMER3_IMR_R = 0x400;

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN1_R |= 0x10; //enable timer3B interrupts - page 104

    // Find the vector number of timer3B
    IntRegister(INT_TIMER3B, TIMER3B_Interrupt_Handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number


}

int ping_read(){
    TIMER3_IMR_R = 0x0;
    TIMER3_CTL_R &= 0x0FF;
    ping_sendPulse();
    TIMER3_CTL_R |= 0xD00;
    state = LOW;


    while(state!=DONE);

    return (rising_time - falling_time);
}

void ping_sendPulse(){

    GPIO_PORTB_AFSEL_R &= ~(0x08);
    GPIO_PORTB_PCTL_R &= ~(0x0000F000);
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0xF7;
    GPIO_PORTB_DIR_R &= 0xF7;
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x00007000;

    TIMER3_ICR_R |= 0xFFFFF;
    TIMER3_IMR_R = 0x400;


}

void TIMER3B_Interrupt_Handler(void){
    if (state == LOW){
        rising_time = TIMER3_TBR_R;
        state = HIGH;
    }
    else{
        falling_time = TIMER3_TBR_R;
        state = LOW;
        state = DONE;
    }
    TIMER3_ICR_R |= 0xFFFFF;
}

void ping_interrupt_init(){

    // Enable interrupts for receiving bytes through UART1
    TIMER3_IMR_R = 0x400; //enable interrupt on capture event mode - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN1_R |= 0x10; //enable timer3B interrupts - page 104

    // Find the vector number of timer3B
    IntRegister(INT_TIMER3B, TIMER3B_Interrupt_Handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number
}

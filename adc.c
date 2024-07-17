/*
 *
 *   uart.c
 *
 *
 *
 *
 *
 *   @author
 *   @date
 */

#include "adc.h"
#include "Timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void adc_init(void)
{
    //TODO

//    GPIO_PORTB_DEN_R |= 0xFF;
//    GPIO_PORTB_AMSEL_R |= 0xFF;
//    GPIO_PORTB_PCTL_R  |= 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    // sets PB4 (AIN10)
    // enables PB4


    SYSCTL_RCGCGPIO_R |= 0x2; //portB
    SYSCTL_RCGCADC_R |= 0x1;      // enable clock ADC0 (page 340)
    GPIO_PORTB_DEN_R &= ~0x10; //disable PB4
//    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;

    GPIO_PORTB_AMSEL_R |= 0x10;

    ADC0_ACTSS_R &= ~0x1;
    ADC0_EMUX_R = 0x0;
    ADC0_SSMUX0_R |= 0xA;
    ADC0_SSCTL0_R |= 0x6;
    ADC0_SAC_R = 0x4;
    ADC0_PSSI_R |= 0x1;

    ADC0_ACTSS_R |= 0x01; //SS0

//    //compute baud values [UART clock= 16 MHz]
//    double fbrd;
//    int    ibrd;
//
//    fbrd = 0.68055555556 * 64 + 0.5; // page 903
//    ibrd = 8;
//
//
//
//    UART1_CTL_R &= 0xFFFFFFFE;      // disable UART1 (page 918)
//    UART1_IBRD_R = 0x8;        // write integer portion of BRD to IBRD
//    UART1_FBRD_R = 0x2c;   // write fractional portion of BRD to FBRD
//    UART1_LCRH_R = 0x60;        // write serial communication parameters (page 916) * 8bit and no parity
//    UART1_CC_R   = 0x00;          // use system clock as clock source (page 939)
//    UART1_CTL_R |= 0x1;        // enable UART1
}

int adc_read(void)
{
    ADC0_PSSI_R = ADC_PSSI_SS0;

    while ((ADC0_RIS_R & ADC_RIS_INR0) == 0)
    {

    }

    ADC0_ISC_R = ADC_RIS_INR0;

    return ADC0_SSFIFO0_R;
}

//void ADC_sendChar(char data){
//	//TODO
//    ADC1_SSFIFO1_R = data;
//}
//
//char ADC_receive(void){
//	//TODO
//    char data;
//
//    data = ADC1_SSFIFO1_R;
//    GPIO_PORTB_DATA_R = ADC1_SSFIFO1_R >> 8;
//
//    return data;
//}
//
//void uart_sendStr(const char *data){
//	//TODO for reference see lcd_puts from lcd.c file
//}
//
//// _PART3
//
//
//void uart_interrupt_init()
//{
//    // Enable interrupts for receiving bytes through UART1
//    UART1_IM_R |= 0x0010; //enable interrupt on receive - page 924
//
//    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
//    // Note: NVIC register descriptions are found in chapter 3.4
//    NVIC_EN0_R |= 0x40; //enable uart1 interrupts - page 104
//
//    uart_event = 0;
//
//    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
//    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number
//
//}
//
//void uart_interrupt_handler()
//{
//// STEP1: Check the Masked Interrupt Status
//    if(UART1_MIS_R & 0x0010){
//        uart_event = 1;
//    }
////STEP3:  Clear the interrupt
//    UART1_ICR_R = 0xFF;
//    uart_typed = ADC_receive();
//
//}

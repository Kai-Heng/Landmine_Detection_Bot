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

#include "uart.h"
#include "adc.h"
#include "Timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"



void uart_init(void){
    //TODO
    SYSCTL_RCGCGPIO_R |= 0x02;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0x02;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R |= 0x03;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  |= 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   |= 0x03;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   &= ~0x0;        // sets pb0 as output, pb1 as input

    //compute baud values [UART clock= 16 MHz]
    double fbrd;
    int    ibrd;

    fbrd = 0.68055555556 * 64 + 0.5; // page 903
    ibrd = 8;



    UART1_CTL_R &= 0xFFFFFFFE;      // disable UART1 (page 918)
    UART1_IBRD_R = 0x8;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = 0x2c;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = 0x60;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0x00;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0x1;        // enable UART1
}

void uart_sendChar(char data){
    //TODO
    while((UART1_FR_R & 0x20) != 0);
    UART1_DR_R = data;
}

char uart_receive(void){
    //TODO
    uint32_t i;
    char data;

    while((UART1_FR_R & 0x10) != 0);
    i = UART1_DR_R;

    if(i & 0xF00){
        GPIO_PORTB_DATA_R = 0xF;
    }
    else{
        data = (char) UART1_DR_R & 0xFF;
    }

    return data;
}

void uart_sendStr(const char *data){
    //TODO for reference see lcd_puts from lcd.c file
    int i = 0;
    while (data[i] != '\0') {
        uart_sendChar(data[i]);
        i++;
    }
}

// _PART3


void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0x0010; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0x40; //enable uart1 interrupts - page 104

    uart_event = 0;

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler()
{
// STEP1: Check the Masked Interrupt Status
    if(UART1_MIS_R & 0x0010){
        uart_event = 1;
    }
//STEP3:  Clear the interrupt
    UART1_ICR_R = 0xFF;
    uart_typed = uart_receive();

}

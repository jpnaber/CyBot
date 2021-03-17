/*
 * servo.c
 *
 *  Created on: Mar 31, 2020
 *      Author: natha
 */

#include "timer.h"
#include "servo.h"

int move_servo(int angle)
{
    int a = angle;
    if(a < 0) return -1;
    if(a > 180) return -1;

    double seconds = (a / 180.0 / 1000) + 0.001;
    uint32_t cycles = seconds * 16000000;

    TIMER1_TBPMR_R = (cycles >> 16);
    TIMER1_TBMATCHR_R = cycles & 0x0000FFFF;
    return angle;
}

void servo_init()
{
    SYSCTL_RCGC1_R |= (1 << 17);                    //Enable Timer 1 clock
    TIMER1_CTL_R &= ~(1 << 8);                      //Disable Timer B while configuring
    TIMER1_CFG_R = 0x4;                             //Set Timer 1 to 16 bit mode

    SYSCTL_RCGCGPIO_R |= (1 << 1);                  //Enable GPIO clock
    while(SYSCTL_PRGPIO_R & (1 << 1) != (1 << 1)) ; //Wait for peripheral ready

    GPIO_PORTB_DEN_R |= (1 << 5);                   //Enable GPIO for PB5

    GPIO_PORTB_AFSEL_R |= (1 << 5);                 //Enable alternate function for PB5

    GPIO_PORTB_PCTL_R &= ~0x00F00000;               //Clear Port B pin 5 alternate function
    GPIO_PORTB_PCTL_R |= 0x00700000;                //Set PB5 alternate function to T1CCP1

    TIMER1_TBMR_R |= 0b1010;                        //Enable PWM mode
    TIMER1_CTL_R |= (1 << 14);                      //Invert Timer B PWM output

    TIMER1_TBPR_R = 0x0004;
    TIMER1_TBILR_R = 0xe200;                        //320,000 cycles (20 ms) (16,000,000 Hz * 0.02 seconds) (0x4e200)
    TIMER1_CTL_R |= (1 << 8);                       //Enable Timer B

    move_servo(90);
}

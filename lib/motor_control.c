#include "msp.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * motor_control.c
 *
 *  Created on: Oct 10, 2018
 *      Author: elena
 */

/*_____________GLOBAL VARIABLES _________________*/

int d_L = 500; //Duty cycle of the left motor
int d_R = 500; //Duty cycle of the right motor
int state = 0; //State of the system
int period = 1000; //Period of the motors
long clock_sec = 12000000;
//stop = 0x2C0
//start = 0x02d

/*_____________SET UP FUNCTIONS__________________*/

void pwmSetUp()
{
    /*NOTE:
     *
     *LEFT MOTOR: (Pin 7: Port 3,1,)
     *RIGHT MOTOR (Pin 6: Port 3,1,)
     * */

    /************ Port 2 Set Up: PWM L/R (IN2)**************/
    P2SEL0 |= 0xC0;  //Sets up TIMER A
    P2SEL1 &= ~0xC0;  // Likewise for the sel 1 register.
    P2DIR = 0xC0;  // Configures bits 6 and 7 of Port 1 to be an output.
    P2DS = 0xC0;  // Set bits 6 & 7 to be high drive strength

    /*--------Register Set Up-----------------*/
    TIMER_A0->CCTL[0] = 0x0090;    // CCI0 toggle
    TIMER_A0->CCR[0] = period; // Period will vary with the clock settings.  You can measure it.
    TIMER_A0->EX0 = 0x0000;    // Divide by 1
    TIMER_A0->CCTL[3] = 0x0040;    // CCR1 toggle/reset
    TIMER_A0->CCR[3] = d_R;    //right motor duty cycle
    TIMER_A0->CCTL[4] = 0x0040;    // CCR1 toggle/reset
    TIMER_A0->CCR[4] = d_L; // Left motor duty cycle
    TIMER_A0->CTL = 0x0230;    // SMCLK=12MHz, divide by 1, up-down mode

}

void pinSetUp()
{

    /************ Port 1 Set Up: DIR L/R (IN1) **************/
    P1SEL0 = 0;  // This sets all 8 bits in the "port 2 sel 0 register" to zero.
    P1SEL1 = 0;  // Likewise for the sel 1 register.
    P1DIR = 0xC0;  // Configures bits 6 and 7 of Port 1 to be an output.
    P1DS = 0xC0;  // Set bits 6 & 7 to be high drive strength

    pwmSetUp();

    /************ Port 3 Set Up: SLP L/R**************/
    P3SEL0 = 0;  // This sets all 8 bits in the "port 2 sel 0 register" to zero.
    P3SEL1 = 0;  // Likewise for the sel 1 register.
    P3DIR = 0xC0;  // Configures bits 6 and 7 of Port 1 to be an output.
    //P3DS = 0xC0;  // Set bits 6 & 7 to be high drive strength
    P3OUT = 0xC0;
}


void TA1_0_IRQHandler() // Interrupt handler for TimerA0.0 corresponds to the period
{
    TIMER_A1->CCTL[0] &= ~0x0001; //resets the interrupt flag to no pending interrupt
    TIMER_A1->CTL = 0x2C0; //Sets up the TA0CTL Register

}
void clockSetUp(){

    /*--------Register Set Up-----------------*/
    TIMER_A1->CTL = 0x0200; //Sets up the TA0CTL Register
    TIMER_A1->CCTL[0] |= (0x0010);   // Enables an interrupt
    TIMER_A1->CCR[0] = clock_sec; // Period will vary with the clock settings.  You can measure it.
    TIMER_A1->EX0 = 0x0000;    // Divide by 1

     NVIC_EnableIRQ(TA1_0_IRQn);

     __enable_irq(); //Sets up all global interrupts
}

void wait(int de)
{
//   //adjust clock
//    TIMER_A1->CCR[0] = de*clock_sec;
//    ///start clock
//    TIMER_A1->CTL = 0x0210; //Sets up the TA0CTL Register
//    //interupt turns off clock
    while(de){
        de-=1;
    }

}




/*_____________HELPER FUNCTIONS__________________*/

void update(int dutyL, int dutyR)
{
//Update values of duty cycles
    if (dutyL <= period && dutyR <= period)
    {
        d_L = dutyL;
        d_R = dutyR;
        TIMER_A0->CCR[3] = d_R;    //adjusts right duty cycle
        TIMER_A0->CCR[4] = d_L;    //adjust left duty cycle
    }

}
/*____________GETTER AND SETTER FUNCTIONS__________________*/



void setState(int s)
{
    state = s;
}

int getState()
{
    return state;
}
int getd_R()
{
    return d_R;
}


int getd_L()
{
    return d_L;
}
//

/*_____________MOTION FUNCTIONS__________________*/


void stop()
{
    state = 0;
    int dl = d_L;
    int dr = d_R;
    int i;
    for (i = 1000; i > 0; i--)
    {    //slows the motor down
        update(d_L, d_R);
        d_L -= dl * 0.001;
        d_R -= dr * 0.001;
    }
    update(0, 0);    //sets duty to zero
    P1OUT &= ~(BIT7 | BIT6 );

}
/*
 * Moves the robot forward by having both wheels will move at the same given speed.
 *
 * @speed: the
 * @preconditions: non-negative number cannont exceede
 * @Postconditions
 * */
void forward(int duty)
{
    state = 1;

    update(duty, duty);
    P1OUT &= ~(BIT7 | BIT6 );



}

void reverse(int duty)
{
    state = -1;

    update(duty, duty);
    P1OUT |= (BIT7 | BIT6 );


}
void turn90Deg(int dir){

    //P1OUT &= ~(BIT7 | BIT6 );

    if(dir ==-1){//Turn Left
        P1OUT = (BIT7 & ~BIT6);

    }else if(dir ==1){//Turn Right
        P1OUT = (~BIT7 & BIT6);
    }

    int wait = 3018000/31;

    update(500, 500);
    while(wait--); //delay

    stop();
}

void turn180Deg(int dir){
   turn90Deg(dir);
   turn90Deg(dir);
}
void turn(int dutyL, int dutyR){
    P1OUT &= ~(BIT7 | BIT6 );
    update(dutyL, dutyR);

}//void rightTurn(int r)
//{
//    state = 1;
//    // update(duty, duty);
//}
//
//void leftTurn(int r)
//{
//
//}





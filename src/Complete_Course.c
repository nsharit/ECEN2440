/*
 * @Title: ECEN 2440 Lab 0.8: Complete Course
 * @Author: Elena Murray & Frank Chen
 * @Date: 10/15/18
 *
 *
 *  * */

/*-----------------------Header Files----------------------------------------*/
#include "msp.h"
#include <stdio.h>
#include <lib/motor_control.h>
#include <stdint.h>
#include <stdlib.h>

//*_____________GLOBAL VARIABLES _________________*/
int sensor_2_count = 0; //Stores the count for the second sensor
int sensor_5_count = 0; //Stores the count for the fifth sensor

double cavg_2 = 0;
long cavg_5 = 0;
long start;
long store;

int cruiseSp = 200;
int turnSp = 675;

int d = 300000;
//Interrupt counts
int count3 = 0;
int count0 = 0;
int count2 = 0;
int count5 = 0;
int count6 = 0;
int count7 = 0;

/*
 * Ground = 8000
 * Black Line = 11500
 *Other Colors - less than 5500
 *
 * */
int thresh2 = 10000;//9750
int thresh1 = 5600;
int hit = 0;

//int thresh1 = TBD
int ticks_per_ms = 3000;
long sample_time = 3018000; //3018000 is how many ticks there are per second, we want the delay to work for 1/10 of a second

/*-----------------------XXX Registers----------------------------------------*/

volatile long *systick_control_reg = (volatile long *) 0xE000E010; //STCSR
volatile long *systick_reload_value_reg = (volatile long *) 0xE000E014; // STRVR
volatile long *systick_current_value_reg = (volatile long *) 0xE000E018; // STCVR
volatile long *systick_calibration_reg = (volatile long *) 0xE000E01C; // STCR

/*-----------------------Functions----------------------------------------*/

/*-----------------------SET UP----------------------------------------*/
void port4SetUp()
{

    /*--------Register Set Up-----------------*/
    P4SEL0 &= 0x00;
    P4SEL1 &= 0x00;
    P4DIR &= ~(BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );
    P4->IE |= (BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );
    P4->IES |= (BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );
    P4->REN |= (BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );
    P4->OUT |= (BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );
    P4->IFG &= ~(BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );

    P4IE &= (BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );
    P4IES &= (BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );

    NVIC_EnableIRQ(PORT4_IRQn);
    __enable_irq(); //Sets up all global interrupts

}

/*-----------------------HELPER----------------------------------------*/
void setup()
{

    /*_____________PORT 7 SET UP_________________*/
    P7SEL0 = 0;  // This sets all 8 bits in the "port 2 sel 0 register" to zero.
    P7SEL1 = 0;  // Likewise for the sel 1 register.
    pinSetUp();
    P2SEL0 &= ~(BIT0 | BIT1 | BIT2 );
    P2SEL1 &= ~(BIT0 | BIT1 | BIT2 );
    P2DIR |= (BIT0 | BIT1 | BIT2 );
   port4SetUp();
}

void avoid(int delay_width, int delay_length)
{
    int duty0L = getd_L();
    int duty0R = getd_R();
    //Back up to get more room
    stop();
    wait(d);
    reverse(200);
    wait(d);

    //Turn Left to avoid the width object
    stop(); //stop
    turn90Deg(-1); //turn left
    stop(); //stop
    forward(300); //go forward for a bit
    wait(d * delay_width * .25);
    stop(); //stop

    //Turn Right to avoid the length object
    turn90Deg(1); //turn right
    stop(); //stop
    forward(300); //go forward for a bit
    wait(d * delay_length * .25);
    stop(); //stop

    //Turn Right to avoid the width of the objct
    turn90Deg(1); //turn right
    stop(); //stop
    forward(300); //go forward for a bit
    wait(d * delay_width * .25);
    stop(); //stop

    //Return to normal position
    turn90Deg(-1); //left turn
    stop();
    //turn(duty0L, duty0R);

}

/*-----------------------INTERRUPTS----------------------------------------*/

void PORT4_IRQHandler()         // Interrupt handler for port 1
{

    if (P4->IFG & (BIT5 | BIT3 ))
    {
        P4->IFG &= ~(BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );     //clear bits

        //wait
        wait(50);
        if ((P4->IFG & BIT5 ) | (P4->IFG & BIT3 ))
        {
            avoid(5, 10);
            hit++;
        }

    }

    P4->IFG &= ~(BIT3 | BIT0 | BIT2 | BIT5 | BIT6 | BIT7 );       //clear bits

}
/*-----------------------RUN----------------------------------------*/

void runTest_avoid()
{
    setup();
    forward(300);
    while (hit != 2)
    {

    }

    stop();
    turn180Deg(1);

}



/**
 * main.c
 * Sensor 2 -> right motor
 * Sensor 5 -> left motor
 */

void runCourse()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    setup();
    /*_____________Local Variables_________________*/
    int it = 0;
    int dutyL = 0;
    int dutyR = 0;
    int on2 = 0;
    int on5 = 0;

    /*_____________SYSTICKS VARIABLES__________*/

    *systick_control_reg = 0x5; //enable systick
    *systick_reload_value_reg = 0xFFFFFF;

    /*_____________WHILE LOOP_________________*/

    while (1)
    {
        //GO HIGH and SET TO AN OUTPUT (PIN 7.2 (SENSOR 5) and PIN 7.5 (SENSOR 2))
        P7DIR |= (BIT4 | BIT1 );  //output
        //SEND 1 TO OUT
        P7OUT |= (BIT4 | BIT1 );          //set ouput to high
        //WAIT
        int i;
        for (i = 0; i < 100; i++)
            ;        //delay
        //CHANGE TO AN INPUT
        P7DIR &= ~(BIT4 | BIT1 );          //intput
        //START TIMER END WHEN INPUT = 0
        *systick_current_value_reg = 0xFFFFFF;
        start = *systick_current_value_reg;

        while (P7IN)
        {          //loop until inputs are zero
            if ((P7IN & BIT1 ) == 0 && on2 == 0)          //sensor 2
            {          //Checks to see if the input for sensor 5 is zero

                store = *systick_current_value_reg;
                sensor_2_count = start - store;
                on2++;
            }
            if ((P7IN & BIT4 ) == 0 && on5 == 0)          //Sensor 5
            {          //Checks to see if the input for sensor 5 is zero

                store = *systick_current_value_reg;
                sensor_5_count = start - store;
                on5++;
            }

        }
        on2 = 0;
        on5 = 0;

        //P2OUT &= ~(BIT0 | BIT2 ); //clears other bits
       // P2OUT |= BIT1; //green
        if (sensor_2_count < thresh2 && thresh1 < sensor_2_count) //thresh1 < sensor_2_count &&
        {
            //printf("Sensor 2: Ground Detected & ");
            dutyL = cruiseSp;  //ground is detected keep at the same speed
            //dutyR= cruiseSp;
        }
        else
        {
            //printf("Sensor 2: Line Detected & ");
            dutyL = turnSp;  //Line Detected
            dutyR= cruiseSp;
           // P2OUT &= ~(BIT0 | BIT1 );  //CLears bits
           // P2OUT |= BIT2; //Sets LED TO BLUE
        }

        if (sensor_5_count < thresh2 && thresh1 < sensor_5_count) //thresh1 < sensor_5_count &&
        {
            //printf("Sensor 5: Ground Detected \n\n ");
            dutyR = cruiseSp;  //ground is detected keep at the same speed
            //dutyL= cruiseSp;
        }
        else
        {
            // printf("Sensor 5: Line Detected \n\n ");
            dutyR = turnSp;  //Line Detected
            dutyL= cruiseSp;
//            P2OUT &= ~(BIT1 | BIT2 );
//            P2OUT |= BIT0; //Sets LED TO RED
     }
        turn(dutyL, dutyR);

    }

}


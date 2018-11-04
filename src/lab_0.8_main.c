/*
 * @Title: ECEN 2440 Lab 0.5 Interrupts
 * @Author: Elena Murray & Billy Murillo
 * @Date: 9/24/2018 -
 *  * */

/*-----------------------Header Files----------------------------------------*/
//Project Header Files
#include <src/Complete_Course.h>
#include <src/tester.h>
#include <lib/motor_control.h>
#include "msp.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * main.c
 * Sensor 2 -> left side of the car
 * Sensor 5 -> right side of the car
 */
void main(void)
{
    //Initial Default Set Up
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    //pinSetUp();
    //avoidObj(10,10);
    //test_motor_control();
   //runTest_avoid();
   //runCalc();
    runCourse();
    //turn(300,300);
    //test_motor_control();
//    clockSetUp();
//    printf("On your marks... get set.. \n\n");
//    wait(4);
//    printf("GO!!!!!!!!!!!!! \n\n");

}

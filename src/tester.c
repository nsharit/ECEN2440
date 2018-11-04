/*
 * tester.c
 *
 *  Created on: Oct 17, 2018
 *      Author: elena
 */
#include <lib/motor_control.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void test_motor_control()
{

    printf("TEST_MOTOR_CONTROL\n");
    pinSetUp();
    //Testing turn function
    turn(300, 300);
    wait(500000);

    stop();
    turn(500, 300); //right turn
    wait(500000 / 5);

    stop();
    turn(300, 500); //left turn
    wait(500000 / 5);

}





void follow_line()
{

//Get input from each sensor

//Compare sensor 2 to the threshold

//Adjust speed accordingly

//Compare sensor 5 to the threshold

//Adjust speed accordingly

}

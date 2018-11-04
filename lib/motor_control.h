/*
 * motor_control.h
 *
 *  Created on: Oct 10, 2018
 *      Author: elena
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

void pinSetUp();
void clockSetUp();

void wait(int de);
void stop();

void forward(int duty);
void reverse(int duty);

void turn(int dutyL, int dutyR);

void turn90Deg(int dir);
void turn180Deg(int dir);

int getd_R();
int getd_L();


//void rightTurn(int r);
//
//void leftTurn(int r);


#endif /* MOTOR_CONTROL_H_ */

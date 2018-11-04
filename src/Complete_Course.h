/*
 * Complete_Course.h
 *
 *  Created on: Oct 17, 2018
 *      Author: elena
 */

#ifndef SRC_COMPLETE_COURSE_H_
#define SRC_COMPLETE_COURSE_H_
int line_detected(long count2, long count5 );

void PORT4_IRQHandler();         // Interrupt handler for port 1

void avoid(int delay_width, int delay_length);
void runTest_avoid();
void port4SetUp();
void runCourse();



#endif /* SRC_COMPLETE_COURSE_H_ */

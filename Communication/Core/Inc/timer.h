/*
 * timer.h
 *
 *  Created on: Dec 21, 2023
 *      Author: Dell
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TIMER_CYCLE		10

void timer_run();

void setTimer0(int duration);
unsigned char getTimer0Flag();

void setTimer1(int duration);
unsigned char getTimer1Flag();

#endif /* INC_TIMER_H_ */

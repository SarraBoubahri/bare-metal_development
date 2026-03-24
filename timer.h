/*
 * timer.h
 *
 *  Created on: 19 mars 2026
 *      Author: mi11p016
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

void tim2_init(uint32_t psc, uint32_t arr);
int tim2_update_pending(void);
void tim2_clear_update(void);

void tim2_init(uint32_t psc, uint32_t arr);
void tim2_enable_irq(void);
void tim2_irq_handler(void);

#endif /* TIMER_H_ */


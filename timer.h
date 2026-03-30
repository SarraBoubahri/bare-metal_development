/*
 * timer.h
 * Configuration et l'utilisation du timer TIM2
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

/* Méthode 1 : Timer sans interruption */

void tim2_init(uint32_t psc, uint32_t arr); // Initialise TIM2
int tim2_update_pending(void); // Test le flag d'update
void tim2_clear_update(void); // Efface le flag d'update

/* Méthode 2 : Timer avec interruption */

void tim2_enable_irq(void); // Active les interruptions TIM2
void tim2_irq_handler(void); // Handler d'interruption TIM2

#endif /* TIMER_H_ */


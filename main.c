/*
 * main.c
 *
 *  Created on: 17 mars 2026
 *      Author: mi11p016
 */

#include <stdio.h>
#include <stdint.h>
#include "gpio.h"
#include "timer.h"
#define irq_enable() asm volatile("cpsie i\n" :::)


int main(){
	gpio_init('I');
	gpio_mode('I',12,0);
	gpio_mode('I',13,0);
	gpio_mode('I',14,0);
	gpio_mode('I',15,0);


//	gpio_set('I',12,0);
//	gpio_set('I',13,0);
//	gpio_set('I',14,0);
//	gpio_set('I',15,0);

	gpio_set('I',12,1);
	gpio_set('I',13,1);
	gpio_set('I',14,1);
	gpio_set('I',15,1);

	/*
	// METHODE 1 : BOUCLE
	while(1){
		for(int i=0; i<10000000; i++){}
		gpio_toggle('I', 13);
	}*/

	/*
	// METHODE 2 : TIMER SANS INTERRUPTION
    tim2_init(63999, 999);

    while (1) {
        if (tim2_update_pending()) {
            tim2_clear_update();
            gpio_toggle('I', 12);
            gpio_toggle('I', 13);
            gpio_toggle('I', 14);
            gpio_toggle('I', 15);
        }
    }*/

	// METHODE 1 : TIMER AVEC INTERRUPTION
	tim2_init(63999, 999);
	tim2_enable_irq();

    while (1) {
    	asm volatile("wfi");
    }
 }


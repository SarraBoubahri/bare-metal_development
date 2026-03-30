/*
 * gpio.c
 * Configuration et manipulation des GPIO sur STM32H747XI
 */

#include <stdio.h>
#include <stdint.h>
#include "gpio.h"

volatile uint32_t* RCC_AHB4ENR = (uint32_t*)(0x58024400 + 0x0E0); // Pointeur vers le registre RCC_AHB4ENR

/* Configurer un port GPIO via RCC */
void gpio_init(char port){
	int n = port - 'A';
	uint32_t temp;
	
	// Activer le port GPIO correspondant
	*RCC_AHB4ENR = *RCC_AHB4ENR  | (1 << n);
	
	// Attendre que le port soit prêt
	while ((*RCC_AHB4ENR & (1 << n)) == 0 ){
		temp = *RCC_AHB4ENR;
	};

	// Lecture de la valeur du registre GPIO_IDR
	volatile uint32_t* gpio_idr = (uint32_t*) (0x58020000 + 0x400*n + 0x10);
	temp = *gpio_idr; 
}

/* Configurer le mode d'un bit d'un port GPIO 
Chaque bit est représenté par 2 bits dans le registre MODER */
void gpio_mode(char port, int bit, int dir){
	int n = port - 'A';
	gpio_t* gpio = (gpio_t *)(0x58020000 + (n*0x400));
	uint32_t temp_gpio = gpio->MODER;

  	// mettre a 0 les deux bits correspondant
	temp_gpio = temp_gpio & ~(1 << (bit*2));
	temp_gpio = temp_gpio & ~(1 << (bit*2 +1));

	// Définir le mode : 00 = entrée, 01 = sortie
	if (dir == 0){
		temp_gpio = temp_gpio  | (1 << (bit*2));
	}

	// Changer valeur registre MODER
	gpio->MODER = temp_gpio ;
}

/* Affecter un bit à un port configuré en sortie */
void gpio_set(char port, int n, int value){
	int p = port - 'A';
	gpio_t* gpio = (gpio_t *)(0x58020000 + (p*0x400));
	uint32_t  temp_gpio = gpio->ODR;
	
	// Réinitialiser le bit d'intérêt (mise à 0)
	temp_gpio = temp_gpio & ~(1 << n);
	
	// Appliquer la valeur demandée
	if (value == 1){
	  temp_gpio = temp_gpio  | (1 << n);
	} else if (value == 0){
	  temp_gpio = temp_gpio & ~(1 << n);
	}
	
	// Changer valeur registre ODR
	gpio->ODR = temp_gpio ;
}

/* Lire l'état d'un bit d'un port configuré en entrée */
int gpio_get(char port, int bit){
	int p = port - 'A';
	gpio_t* gpio = (gpio_t *)(0x58020000 + (p*0x400));

	// Retourner la valeur du bit 
	return (gpio->IDR & (1 << bit)) ? 1:0 ;
}

/* Inverser la valeur d'un bit d'un port GPIO en sortie */
void gpio_toggle(char port, int bit){
	int p = port - 'A';
	gpio_t* gpio = (gpio_t *)(0x58020000 + (p*0x400));
	uint32_t  temp_gpio = gpio->ODR;
	
	// Basculer le bit
	if(temp_gpio & (1 << bit)){
	  temp_gpio = temp_gpio & ~(1 << bit);
	}
	else {
	  temp_gpio = temp_gpio | (1 << bit);
	}

	// Changer valeur registre ODR
	gpio->ODR = temp_gpio;
}

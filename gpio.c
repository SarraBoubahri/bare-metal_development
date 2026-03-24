/*
 * gpio.c
 *
 *  Created on: 17 mars 2026
 *      Author: mi11p016
 */

#include <stdio.h>
#include <stdint.h>
#include "gpio.h"

volatile uint32_t* RCC_AHB4ENR = (uint32_t*)(0x58024400 + 0x0E0);


  void gpio_init(char port){

	  int n = port - 'A';
	  uint32_t temp;
	  *RCC_AHB4ENR = *RCC_AHB4ENR  | (1 << n);

	  while ((*RCC_AHB4ENR & (1 << n)) == 0 ){
		  temp = *RCC_AHB4ENR;
	  };

	  volatile uint32_t* gpio_idr = (uint32_t*) (0x58020000 + 0x400*n + 0x10);
	  temp = *gpio_idr; // lecture de la valeur du registre GPIO_IDR
  }

  /*
     Le bit est initialisé en entrée ou sortie en fonction de dir :
       - 0 : sortie
       - 1 : entrée
  */
  void gpio_mode(char port, int bit, int dir){

	  int n = port - 'A';
	  gpio_t* gpio = (gpio_t *)(0x58020000 + (n*0x400));
	  uint32_t temp_gpio = gpio->MODER;

	  // mettre a 0 bit concerné

	  temp_gpio = temp_gpio & ~(1 << (bit*2));
	  temp_gpio = temp_gpio & ~(1 << (bit*2 +1));

	  // on met valeur selon dir

	  if (dir == 0){
		  temp_gpio = temp_gpio  | (1 << (bit*2));
	  }

	  // changer valeur registre MODER

	  gpio->MODER = temp_gpio ;

  }

  /* Affecter un bit à un port configuré en sortie */
  void gpio_set(char port, int n, int value){

	  int p = port - 'A';
	  gpio_t* gpio = (gpio_t *)(0x58020000 + (p*0x400));
	  uint32_t  temp_gpio = gpio->ODR;

	  // mettre a 0 bit concerné

	  temp_gpio = temp_gpio & ~(1 << n);

	  // on met valeur selon value

	  if (value == 1){
		  temp_gpio = temp_gpio  | (1 << n);
	  } else if (value == 0){
		  temp_gpio = temp_gpio & ~(1 << n);
	  }

	  // changer valeur registre ODR

	  gpio->ODR = temp_gpio ;

  }

  /* Lire un bit d'un port configuré en entrée */
  int gpio_get(char port, int bit){

	  int p = port - 'A';
	  gpio_t* gpio = (gpio_t *)(0x58020000 + (p*0x400));

	  return (gpio->IDR & (1 << bit)) ? 1:0 ;

  }

  /* Inverser la valeur d'une sortie */
  void gpio_toggle(char port, int bit){

	  int p = port - 'A';
	  gpio_t* gpio = (gpio_t *)(0x58020000 + (p*0x400));

	  uint32_t  temp_gpio = gpio->ODR;

	  if(temp_gpio & (1 << bit)){
		  temp_gpio = temp_gpio & ~(1 << bit);
	  }
	  else {
		  temp_gpio = temp_gpio | (1 << bit);
	  }

	  //temp_gpio = temp_gpio ^ (1 << bit);
	  gpio->ODR = temp_gpio;

  }

/*
 * gpio.h
 *
 *  Created on: 17 mars 2026
 *      Author: mi11p016
 */

#ifndef GPIO_H_
#define GPIO_H_

typedef struct __attribute__((packed, aligned(4))) {
  volatile uint32_t MODER;
  volatile uint32_t reserved1[3];
  volatile uint32_t IDR;
  volatile uint32_t ODR;
} gpio_t;


  /* Initialiser un port GPIO
     Active le périphérique GPIO associé au port donné en paramètre
     au moyen du RCC.
     Le port est identifié par une lettre de 'A' à 'K'.
  */
  void gpio_init(char port);

  /* Choisir le mode d'un bit d'un port GPIO
     Le bit est initialisé en entrée ou sortie en fonction de dir :
       - 0 : sortie
       - 1 : entrée
     Le port est identifié par une lettre de 'A' à 'K'
     Le bit par un numéro d'ordre de 0 à 15.
  */
  void gpio_mode(char port, int bit, int dir);

  /* Affecter un bit à un port configuré en sortie */
  void gpio_set(char port, int n, int value);

  /* Lire un bit d'un port configuré en entrée */
  int gpio_get(char port, int bit);

  /* Inverser la valeur d'une sortie */
  void gpio_toggle(char port, int bit);

#endif /* GPIO_H_ */

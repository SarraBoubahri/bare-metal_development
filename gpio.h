/*
 * gpio.h
 * Configuration et manipulation des GPIOs 
 */

#ifndef GPIO_H_
#define GPIO_H_

typedef struct __attribute__((packed, aligned(4))) {
  volatile uint32_t MODER; // Mode des bits des ports GPIO
  volatile uint32_t reserved1[3];
  volatile uint32_t IDR; // Entrée
  volatile uint32_t ODR; // Sortie
} gpio_t;

void gpio_init(char port); // Initialiser un port GPIO

void gpio_mode(char port, int bit, int dir); // Choisir le mode d'un bit d'un port GPIO

void gpio_set(char port, int n, int value); // Affecter un bit à un port configuré en sortie

int gpio_get(char port, int bit); // Lire un bit d'un port configuré en entrée

void gpio_toggle(char port, int bit); // Inverser la valeur d'une sortie

#endif /* GPIO_H_ */

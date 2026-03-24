/*
 * timer.c
 *
 *  Created on: 19 mars 2026
 *      Author: mi11p016
 */

#include <stdint.h>
#include "timer.h"
#include "gpio.h"

// verifier struct
typedef struct __attribute__((packed, aligned(4))) {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} tim_t;

// verifier @
#define RCC_APB1LENR (*(volatile uint32_t*)(0x58024400 + 0x0E8))
#define TIM2_BASE    0x40000000U
#define TIM2         ((tim_t*)TIM2_BASE)
#define NVIC_ISER0   (*(volatile uint32_t*)0xE000E100)

void tim2_init(uint32_t psc, uint32_t arr)
{
    // activer horloge de TIM2
    RCC_APB1LENR |= (1 << 0);

    // on attends que le changement se fasse
    while ((RCC_APB1LENR & (1 << 0)) == 0) {
    }

    // arrêt du timer pendant la config
    TIM2->CR1 = 0;

    // config prescaler et période
    TIM2->PSC = psc;
    TIM2->ARR = arr;

    // remise à zéro compteur
    TIM2->CNT = 0;

    // générer un update pour prendre en compte PSC/ARR
    TIM2->EGR = 1;

    // effacer le flag update si posé
    TIM2->SR &= ~1;

    // démarrer le timer
    TIM2->CR1 |= 1;
}

// METHODE 2 : TIMER SANS INTERRUPTION

int tim2_update_pending(void)
{
    return (TIM2->SR & 1) ? 1 : 0;
}

void tim2_clear_update(void)
{
    TIM2->SR &= ~1;
}

// METHODE 3 : TIMER AVEC INTERRUPTION

void tim2_enable_irq(void)
{
    /* Autoriser interruption update côté timer */
    TIM2->DIER |= 1;

    /* Autoriser IRQ TIM2 côté NVIC (IRQ 28) */
    NVIC_ISER0 |= (1 << 28);
}

void tim2_irq_handler(void)
{
    if (TIM2->SR & 1) {
        TIM2->SR &= ~1;
        gpio_toggle('I', 12);
        gpio_toggle('I', 13);
        gpio_toggle('I', 14);
        gpio_toggle('I', 15);
    }
}

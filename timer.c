/*
 * timer.c
 * Implémentation des fonctions pour le timer TIM2
 */

#include <stdint.h>
#include "timer.h"
#include "gpio.h"

// Structure des registres principaux du timer */
typedef struct __attribute__((packed, aligned(4))) {
    volatile uint32_t CR1; // Contrôle
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER; // Interruptions
    volatile uint32_t SR; // Flags
    volatile uint32_t EGR; // Génération d'événements
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT; // Compteur
    volatile uint32_t PSC; // Prescaler
    volatile uint32_t ARR; // Auto-Reload
} tim_t;

// Registres matériels
#define RCC_APB1LENR (*(volatile uint32_t*)(0x58024400 + 0x0E8))
#define TIM2_BASE    0x40000000U
#define TIM2         ((tim_t*)TIM2_BASE)
#define NVIC_ISER0   (*(volatile uint32_t*)0xE000E100)

// Initialisation du TIM2
void tim2_init(uint32_t psc, uint32_t arr)
{
    RCC_APB1LENR |= (1 << 0); // Activer horloge de TIM2
    while ((RCC_APB1LENR & (1 << 0)) == 0) { // Attente de prise en compte
    }
    TIM2->CR1 = 0; // Arrêt du timer pendant la configuration
    TIM2->PSC = psc; // Configuration prescaler 
    TIM2->ARR = arr; // Configuration période
    TIM2->CNT = 0; // Remise à zéro compteur
    TIM2->EGR = 1; // Update pour prendre en compte PSC/ARR
    TIM2->SR &= ~1; // Remise à zéro du flag upadtae
    TIM2->CR1 |= 1; // Démarrage du timer
}

// METHODE 2 : TIMER SANS INTERRUPTION

// Test du flag d'update 
int tim2_update_pending(void)
{
    return (TIM2->SR & 1) ? 1 : 0;
}

// Effacement du flag d'update
void tim2_clear_update(void)
{
    TIM2->SR &= ~1;
}

// METHODE 3 : TIMER AVEC INTERRUPTION

// Activation des interruptions de TIM2
void tim2_enable_irq(void)
{
    // Autoriser interruption update 
    TIM2->DIER |= 1;
    // Autoriser IRQ TIM2 côté NVIC (IRQ 28)
    NVIC_ISER0 |= (1 << 28);
}

// Handler interruption TIM2
void tim2_irq_handler(void)
{
    if (TIM2->SR & 1) {
        TIM2->SR &= ~1; // Effacement du flag d'interruption
        gpio_toggle('I', 12);
        gpio_toggle('I', 13);
        gpio_toggle('I', 14);
        gpio_toggle('I', 15);
    }
}

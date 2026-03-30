/*
 * init.S
 * Programme de demarrage
 * Initialisation de la memoire et de la table des vecteurs
 */

.title "Init"

.syntax unified
.section init, "ax", %progbits 

/* Table des vecteurs d'interruptions */
.long _debut_pile /* Initialisation du pointeur de pile */
.long _start /* Point d'entree du programme */

/* Exceptions cœur Cortex-M7 */
.long _default_handler  /* Exceptions cœur */
.long _default_handler  /* HardFault */
.long _default_handler  /* MemManage */
.long _default_handler  /* BusFault */
.long _default_handler  /* UsageFault */
.long 0                 /* Reserve */
.long 0                 /* Reserve */
.long 0                 /* Reserve */
.long 0                 /* Reserve */
.long _default_handler  /* SVCall */
.long _default_handler  /* DebugMonitor */
.long 0                 /* Reserve */
.long _default_handler  /* PendSV */
.long _default_handler  /* SysTick */

/* IRQ externes 0..27 */
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler

/* IRQ = 28 (TIM2) */
.long tim2_irq_handler

/* Handler par défaut : boucle infinie */
.thumb_func
_default_handler:
  b .

/* Point d'entree apres reset */
.global _start
.thumb_func
_start:
	ldr sp, = _debut_pile /* Initialisation du pointeur de pile */

	/* Copie de .data de Flash vers RAM */
    ldr r0, =_sidata /* adresse source (Flash) */
    ldr r1, =_sdata /* adresse de début (RAM) */
    ldr r2, =_edata /* adresse de fin (RAM) */

	data_loop:
	    cmp r1, r2 /* Compare adresse courante et fin de .data */
	    bcs data_done /* Si r1 >= r2, copie terminee */
	    ldr r3, [r0], #4 
	    str r3, [r1], #4 
	    b data_loop

	data_done: 
	    /* Initialiser .bss a zero */
	    ldr r0, =_debutbss
	    ldr r1, =_finbss
	    movs r2, #0

	bss_loop:
		cmp r0,r1 /* Compare adresse courante et fin de .bss */
		bcs bss_done /* Si r0 >= r1 initialisation terminee */
		str r2, [r0], #4
		b bss_loop

	bss_done:
		bl main /* Appel du programme principal(main.c) */
		bl _start /* Redémarrage si main retourne */

.end

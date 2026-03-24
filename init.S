/*
 * init.S
 *
 *  Created on: 10 mars 2026
 *      Author: mi11p016
 */

.title "Init"

.syntax unified
.section init, "ax", %progbits

.long _debut_pile
.long _start
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long _default_handler
.long tim2_irq_handler


.thumb_func
_default_handler:
  b .

.global _start
.thumb_func

_start:
	ldr sp, = _debut_pile

	/* Copier .data de FLASH vers RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

	data_loop:
	    cmp r1, r2
	    bcs data_done
	    ldr r3, [r0], #4
	    str r3, [r1], #4
	    b data_loop

	data_done:
    /* Mettre .bss à zéro */
    ldr r0, =_debutbss
    ldr r1, =_finbss
    movs r2, #0


	bss_loop:
		cmp r0,r1
		bcs bss_done
		str r2, [r0], #4
		b bss_loop

	bss_done:
		bl main
		bl _start

.end

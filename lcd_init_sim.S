#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#include <avr/io.h> 

.global lcd_init_sim

lcd_init_sim:			;Υλοποίηση ρουτίνας lcd_init_sim
		push r24	
		push r25	

		ldi r24, 40 
		ldi r25, 0	
		rcall wait_msec	
		ldi r24, 0x30	
		out PORTD, r24	
		sbi PORTD, PD3	
		cbi PORTD, PD3	
		ldi r24, 39
		ldi r25, 0	
		rcall wait_usec 
		push r24	
		push r25	
		ldi r24,lo8(1000)	
		ldi r25,hi8(1000)
		rcall wait_usec
		pop r25
		pop r24		
		ldi r24, 0x30
		out PORTD, r24
		sbi PORTD, PD3
		cbi PORTD, PD3
		ldi r24,39
		ldi r25,0
		rcall wait_usec 
		push r24	
		push r25	
		ldi r24 ,lo8(1000)	
		ldi r25 ,hi8(1000)
		rcall wait_usec
		pop r25
		pop r24		
		ldi r24,0x20	
		out PORTD, r24
		sbi PORTD, PD3
		cbi PORTD, PD3
		ldi r24,39
		ldi r25,0
		rcall wait_usec
		push r24	
		push r25	
		ldi r24 ,lo8(1000)	
		ldi r25 ,hi8(1000)
		rcall wait_usec
		pop r25
		pop r24		
		ldi r24,0x28	
		rcall lcd_command_sim	
		ldi r24,0x0c	
		rcall lcd_command_sim
		ldi r24,0x01	
		rcall lcd_command_sim
		ldi r24, lo8(1530)
		ldi r25, hi8(1530)
		rcall wait_usec
		ldi r24 ,0x06	
		rcall lcd_command_sim	
		pop r25		
		pop r24
		ret

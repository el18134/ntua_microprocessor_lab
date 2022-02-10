#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#include <avr/io.h> 

.global write_2_nibbles_sim

write_2_nibbles_sim:		;Õëïðïßçóç ñïõôßíáò write_2_nibbles_sim
		push r24	
		push r25	
		ldi r24 ,lo8(6000) 
		ldi r25 ,hi8(6000)
		rcall wait_usec
		pop r25
		pop r24		
		push r24	
		in r25, PIND	
		andi r25, 0x0f	
		andi r24, 0xf0	
		add r24, r25	
		out PORTD, r24	
		sbi PORTD, PD3	
		cbi PORTD, PD3	
		push r24	
		push r25	
		ldi r24 ,lo8(6000) 
		ldi r25 ,hi8(6000)
		rcall wait_usec
		pop r25
		pop r24		
		pop r24		
		swap r24	
		andi r24 ,0xf0	
		add r24, r25
		out PORTD, r24
		sbi PORTD, PD3	
		cbi PORTD, PD3
		ret

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#include <avr/io.h> 

.global lcd_data_sim

lcd_data_sim:			;Υλοποίηση ρουτίνας lcd_data_sim
		push r24	
		push r25	
		sbi PORTD, PD2	
		rcall write_2_nibbles_sim	
		ldi r24 ,43 
		ldi r25 ,0	
		rcall wait_usec
		pop r25		
		pop r24
		ret
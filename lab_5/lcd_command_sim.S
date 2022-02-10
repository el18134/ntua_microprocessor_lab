#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#include <avr/io.h> 

.global lcd_command_sim

lcd_command_sim:		;Õëïðïßçóç ñïõôßíáò lcd_comand_sim
		push r24	
		push r25	
		cbi PORTD, PD2	
		rcall write_2_nibbles_sim	
		ldi r24, 39 
		ldi r25, 0	
		rcall wait_usec	
		pop r25		
		pop r24
		ret

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#include <avr/io.h> 

.global wait_msec

wait_msec:			;Υλοποίηση ρουτίνας wait_msec
		push r24				
		push r25				
		ldi r24 , lo8(998)		
		ldi r25 , hi8(998)		
		rcall wait_usec			
		pop r25					
		pop r24					
		sbiw r24 , 1		
		brne wait_msec			
		ret

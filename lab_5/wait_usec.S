#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#include <avr/io.h> 

.global wait_usec

wait_usec:			;Õëïðïßçóç ñïõôßíáò wait_usec
		sbiw r24 ,1			
		nop					
		nop				
		nop					
		nop					
		brne wait_usec		
		ret

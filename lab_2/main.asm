		.DEF icnt = r15
		.DEF a = r16
		.DEF cnt = r17

		.org 0x0
		rjmp reset
		.org 0x3
		rjmp ISR1

ISR1:	inc icnt				;Μέτρηση δικοπών και αποθήκεσυ στον icnt
		in a, PINA
		andi a, 0xC0			;Λήψη εισόδου και απομόνωση των PA6, PA7
		cpi a, 0xC0				;Αν τα PA6, PA7 είναι ON τότε εμφανίζουμε τον αριθμό των διακοπών στην θύρα PORTB
		brne exit				;Αλλιώς βγαίνουμε από την ρουτίνα διακοπής χωρίς να κάνουμε κάτι
		out PORTB, icnt
exit:	clr r18
		out PORTB, r18
		reti

reset:	ldi	r18, LOW(RAMEND)	;Αρχικοποίηση stack pointer
		out	spl, r18
		ldi	r18, HIGH(RAMEND)
		out	sph, r18	

		ldi r18, 0x0C			
		out MCUCR, r18			;Ενεργοποίηση διακοπής INT1 με ανερχόμενη ακμή
		ldi r18, 0x80
		out GICR, r18			;Επίτρεψη διακοπής INT1
		sei						;Ενεργοποίηση διακοπών

		clr r18
		out DDRA, r18			;Αρχικοποίηση PORTA ως input
		ser r18
		out DDRB, r18			;Αρχικοποίηση PORTB ως output
		out DDRC, r18			;Αρχικοποίηση PORTC ως output

		ldi cnt, 0x00			;Aρχικοποίηση μετρητή cnt
start:	out PORTC, cnt			;Εμφάνιση μετρτητή στην θύρα εξόδου PORTC
		inc cnt					;Αύξηση του μετρητή και επανάληψη
		rjmp start
#include <avr/io.h>
#include <avr/interrupt.h>

int i;
char out, pinb, pa2, cnt, y, w;

ISR(INT0_vect)					//Ρουτίνα εξυπηρέτησης διακοπής INT0
{
	pa2 = PINA & 0x04;			//Λήψη εισόδου Α και απομόνωση του PA2
	pinb = PINB;				//Λήψη εισόδου από την θύρα PINB
	cnt = 0;
	
	for(i = 0; i < 8; i++)		//Μέτρηση αναμένων led της PINB και αποθήκεσυη αριθμού στον cnt
	{
		y = pinb & 0x01;		//Περιστροφή εισόδου 8 φορές και έλεγχος του LSB κάθε φορά	
		pinb = pinb >> 1;
		if (y == 0x01)			//Αν το LSB είναι 1 αυξάνουμε τον καταχωρητή cnt
			cnt = cnt + 1;
	}
	
	if (pa2 == 0x04)			
	{
		PORTC = cnt;			//Αν το PA2 είναι ON εμφανίζουμε τον αριθμό αναμένων led σε δυαδική μορφή
	}
	else						//Αν το PA2 είναι OFF ανάβουμε ίσο αριθμό led ξεκινώντας από το LSB
	{
		out = 0x00;
		for (i = 0; i < cnt; i++)
		{
			out = out << 1;		//Προσθέτουμε αναμμένο led στην έξοδο (λογικό 1) μέχρι να έχουμε όσα led έχουμε μετρήσει στο cnt
			out = out | 0x01;
		}
		PORTC = out;			//Εμφάνιση εξόδου στην θύρα PORTC
	}
	
}

int main(void)
{
	DDRC = 0xFF;				// Αρχικοποίηση PORTB ως output
	DDRB = 0x00;				// Αρχικοποίηση PORTC ως input
	DDRA = 0x00;				// Αρχικοποίηση PORTA ως input
	GICR = 0x40;				//Επίτρεψη διακοπής INT0
	MCUCR = 0x03;				//Ενεργοποίηση διακοπής INT0 με ανερχόμενη ακμή
	sei();						//Ενεργοποίηση διακοπών
    
	while (1)					// Infinite loop που περιμένει την ενεργοποίηση διακοπών
    {
		w = w + 1;
	}
}
#include <avr/io.h>

char i, a, b, c, d, o;

int main(void)
{
   DDRB=0xFF;				//Αρχικοποίηση PORTB ως output
   DDRC=0x00;				//Αρχικοποίηση PORTC ως input	while(1)
	{
		i = PINC & 0x0F;	//Λήψη εισόδου και απομώνοση των 4ων LSB
		a = i & 0x01;		//Απομόνωση και αποθήκευση του Α στον register a
		b = i & 0x02;		
		b = b >> 1;			//Απομόνωση και αποθήκευση του B στον register b
		c = i & 0x04;
		c = c >> 2;			//Απομόνωση και αποθήκευση του C στον register c
		d = i & 0x08;
		d = d >> 3;			//Απομόνωση και αποθήκευση του D στον register d
		o = (a&c)&(b|d);	//Αποθήκεσυη του F1 στον register o και περιστροφή κατά μία θέση ώστε να έρθει στην θέση του 2ου LSB
		o = o << 1;
		PORTB = o | ((~((((~b)&c&d)|((~a)&b))))&0x01);	//Συνδυασμός των F0, F1 και εμφάνιση αποτελέσματος στην θύρα PORTB
	}
	return 0;
}


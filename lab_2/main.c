#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

char reg[2], mem[2], fd, sd;

char swap(char x) {
	return ((x & 0x0F) << 4 | (x & 0xF0) >>4);
}

char scan_row(int r) {
	char o = 0x08;
	o = (o << r);
	PORTC = o;
	_delay_us(500);
	return PINC & 0x0F;
}

void scan_keypad() {
	char c;
	
	c = scan_row(1);
	reg[1] = swap(c);
	
	c = scan_row(2);
	reg[1] = reg[1] + c;
	
	c = scan_row(3);
	reg[0] = swap(c);
	
	c = scan_row(4);
	reg[0] = reg[0] + c;
	
	PORTC = 0x00;
}

int scan_keypad_rising_edge() {
	char tmp[2];
	
	scan_keypad();
	tmp[0] = reg[0];
	tmp[1] = reg[1];
	
	_delay_ms(15);
	
	scan_keypad();
	reg[0] = reg[0] & tmp[0];
	reg[1] = reg[1] & tmp[1];
	
	tmp[0] = mem[0];
	tmp[1] = mem[1];
	
	mem[0] = reg[0];
	mem[1] = reg[1];
	
	reg[0] = reg[0] & (~tmp[0]);
	reg[1] = reg[1] & (~tmp[1]);
	
	return (reg[0] || reg[1]);
}

char keypad_to_ascii() {
	if ((reg[0]&0x01) == 0x01)
	return '*';
	
	if ((reg[0]&0x02) == 0x02)
	return '0';
	
	if ((reg[0]&0x04) == 0x04)
	return '#';
	
	if ((reg[0]&0x08) == 0x08)
	return 'D';
	
	if ((reg[0]&0x10) == 0x10)
	return '7';
	
	if ((reg[0]&0x20) == 0x20)
	return '8';
	
	if ((reg[0]&0x40) == 0x40)
	return '9';
	
	if ((reg[0]&0x80) == 0x80)
	return 'C';
	
	if ((reg[1]&0x01) == 0x01)
	return '4';
	
	if ((reg[1]&0x02) == 0x02)
	return '5';
	
	if ((reg[1]&0x04) == 0x04)
	return '6';
	
	if ((reg[1]&0x08) == 0x08)
	return 'B';
	
	if ((reg[1]&0x10) == 0x10)
	return '1';
	
	if ((reg[1]&0x20) == 0x20)
	return '2';
	
	if ((reg[1]&0x40) == 0x40)
	return '3';
	
	if ((reg[1]&0x80) == 0x80)
	return 'A';
	
	return 0;
}

void welcome() {
	char i;
	PORTB = 0xFF;
	for (i = 1; i <= 160; i++) {
		scan_keypad_rising_edge();
		_delay_ms(6);
	}
	PORTB = 0x00;
}

void alarm() {
	char i, j;
	for (j = 1; j <= 4; j++) {
		PORTB = 0xFF;
		for (i = 1; i <= 20; i++) {
			scan_keypad_rising_edge();
			_delay_ms(6);
		}
		PORTB = 0x00;
		for (i = 1; i <= 20; i++) {
			scan_keypad_rising_edge();
			_delay_ms(6);
		}
	}
}

int main(void) {
	DDRB = 0xFF;
	DDRC = 0xF0;
	
	while (1) {
		mem[0] = 0x00;
		mem[1] = 0x00;
		PORTB = 0x00;
		
		while (1) {
			if (scan_keypad_rising_edge() != 0) {
				fd = keypad_to_ascii();
				break;
			}
		}
		
		while (1) {
			if (scan_keypad_rising_edge() != 0) {
				sd = keypad_to_ascii();
				scan_keypad_rising_edge();
				break;
			}
		}
		
		if (fd != '4' || sd != '8') {
			alarm();
		}
		else {
			welcome();
		}
		
	}
	return 0;
}
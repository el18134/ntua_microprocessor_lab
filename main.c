#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

char reg[2], mem[2], key1, key2, d[5];
volatile float VPA0, previous_VPA0 = 0.0;

extern void lcd_init_sim();
extern void lcd_data_sim(char c);

char swap(char x) { //��������� ������� swap
	return ((x & 0x0F) << 4 | (x & 0xF0) >>4);
}

char scan_row(int r) { //��������� �������� scan_row_sim
	char o = 0x08;
	o = (o << r);	//�������� r ����� ��� o = 00001000
	PORTC = o;	//������� ��� ������ ��� ���������� �� 1
	_delay_us(500);	//����������� 500ms ��� ������������� ����������
	return PINC & 0x0F; //��������� ��������  LSB ��� PORTC (������ �������������)
}

void scan_keypad() { //��������� �������� scan_keypad_sim
	char c;
	
	c = scan_row(1); //���������� ������ ������
	reg[1] = swap(c); //���������� ��� 4 MSB ��� ������ 8 bit ��� 16-bit register reg
	
	c = scan_row(2); //���������� �������� ������
	reg[1] = reg[1] + c; //���������� ��� 4 LSB ��� ������ 8 bit ��� 16-bit register reg
	
	c = scan_row(3); //���������� ������ ������
	reg[0] = swap(c); //���������� ��� 4 MSB ��� �������� 8 bit ��� 16-bit register reg
	
	c = scan_row(4); //���������� �������� ������
	reg[0] = reg[0] + c; //���������� ��� 4 LSB ��� �������� 8 bit ��� 16-bit register reg
	
	PORTC = 0x00;
}

int scan_keypad_rising_edge() {	//��������� �������� scan_row_rising_edge_sim
	char tmp[2];
	
	scan_keypad();	//���������� �������������
	tmp[0] = reg[0]; //��������� ���������� ��� �������������
	tmp[1] = reg[1];
	
	_delay_ms(15);	//������� 15ms ���� ������������
	
	scan_keypad();	//������� ���������� �������������
	reg[0] = reg[0] & tmp[0]; //�������� �������� ��� ��������� �����������
	reg[1] = reg[1] & tmp[1];
	
	tmp[0] = mem[0]; //���� ������������ ���������� ��������� ��� ���  RAM
	tmp[1] = mem[1];
	
	mem[0] = reg[0]; //���������� ������� ���������� ��������� ��� ���  RAM
	mem[1] = reg[1];
	
	reg[0] = reg[0] & (~tmp[0]); //������ ��������� ��� ����� ����� �������
	reg[1] = reg[1] & (~tmp[1]);
	
	return (reg[0] || reg[1]); //��������� ��� ��������� ��� ������ ��������� (0 �� ��� ���� ������� �������)
}

char keypad_to_ascii() { //��������� �������� keypad_to_ascii_sim
	if ((reg[0]&0x01) == 0x01)
	return '*';	//������ ��������� �������� ��� ��������� ��� ������� ASCII ��� ��� �����������
	
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
	
	return 0;	//��� ��� ���� ������� ������ ������� ���������� 0
}

void ADC_init() {
	ADMUX = (1 << REFS0);	//Vref = Vcc ��� ������� A0 ��� ������
	ADCSRA = (1 << ADEN | 1 << ADIE | 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); //������������ ADC, �������� ��� ������� prescaler CK/128=62.5Khz
}

ISR(ADC_vect) {
	VPA0 = (ADC/204.8);	//����������� ��� Vout1 (ADC*Vcc/1024)
	if (VPA0 != previous_VPA0) { //��� ������ ���������� ������� ���� ��� ����������� ���� �����
		previous_VPA0 = VPA0;
		dtostrf(VPA0, 4, 2, d);	//��������� Vout1 ��� float �� string(������������ ��� ����� �� ���� ���� ��� ������ d[5])
		lcd_init_sim();
		lcd_data_sim('V');
		lcd_data_sim('o');
		lcd_data_sim('1');
		lcd_data_sim('\n');
		lcd_data_sim(d[0]);
		lcd_data_sim(d[1]);
		lcd_data_sim(d[2]);
		lcd_data_sim(d[3]);
	}
}

ISR(TIMER1_OVF_vect) {
	key2 = 0;
	if (scan_keypad_rising_edge() != 0) { //������� ������������ ���� ���� ������� �������� ��� TIMER1
		key2 = keypad_to_ascii();	//��������� �� ����� ASCII
	}
	if (key2 == '1') { //�� �������� �� 1 ��������� �� Duty Cycle ���� 1
		OCR0++;
	}
	else if (key2 == '2') { //�� �������� �� 2 ��������� �� Duty Cycle ���� 1
		OCR0--;
	}
	ADCSRA |= (1<<ADSC); //������������ ���������� ADC
	TCNT1 = 0xF3CB;	//������������ ��� TCNT1 ��� ����������� ���� ��� 0.4 sec
}

int main(void) {
    DDRD = 0xFF;	//������������ PORTD �� �����
	DDRC = 0xF0;	//������������ 4�� MSB ��� PORTC �� ����� ��� 4�� LSB ��� PORTC �� ������
	DDRB |= (1<<PB3);	//������������ PB3 �� �����
	
	ADC_init();
	
	TIMSK = (1 << TOIE1); //������������ �������� ������������ ��� �������� TCNT1 ��� ��� TIMER/COUNTER1
	TCCR1B = (1<<CS12) | (0<<CS11) | (1<<CS10); //CK/1024
	TCNT1 = 0xF3CB;	//������������ ��� TCNT1 ��� ����������� ���� ��� 0.4 sec

	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01); //������� TIMER/COUNTER0 ��� �������� �� ���������� FAST PWM ���������� 4KHz
	OCR0 = 0xFF; //������������ ��� Duty Cycle �� 255(������ 100%)
	sei(); //������������ ��������
	
	while (1) {
		mem[0] = 0x00;	//������������ ���������� ������ RAM
		mem[1] = 0x00;
		
		while (1) {	
			if (scan_keypad_rising_edge() != 0) { //�������� ������������� ����� �� ������� �������
				key1 = keypad_to_ascii();	//��������� �� ����� ASCII
				break;
			}
		}
		
		if (key1 == '1') { //�� �������� �� 1 ��������� �� Duty Cycle ���� 1
			OCR0++;
		}
		else if (key1 == '2') { //�� �������� �� 2 ��������� �� Duty Cycle ���� 1
			OCR0--;
		}
	}
	
	return 0;
}


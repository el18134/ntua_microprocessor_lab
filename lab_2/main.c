#include <avr/io.h>

char i, a, b, c, d, o;

int main(void)
{
   DDRB=0xFF;				//������������ PORTB �� output
   DDRC=0x00;				//������������ PORTC �� input	while(1)
	{
		i = PINC & 0x0F;	//���� ������� ��� ��������� ��� 4�� LSB
		a = i & 0x01;		//��������� ��� ���������� ��� � ���� register a
		b = i & 0x02;		
		b = b >> 1;			//��������� ��� ���������� ��� B ���� register b
		c = i & 0x04;
		c = c >> 2;			//��������� ��� ���������� ��� C ���� register c
		d = i & 0x08;
		d = d >> 3;			//��������� ��� ���������� ��� D ���� register d
		o = (a&c)&(b|d);	//���������� ��� F1 ���� register o ��� ���������� ���� ��� ���� ���� �� ����� ���� ���� ��� 2�� LSB
		o = o << 1;
		PORTB = o | ((~((((~b)&c&d)|((~a)&b))))&0x01);	//���������� ��� F0, F1 ��� �������� ������������� ���� ���� PORTB
	}
	return 0;
}


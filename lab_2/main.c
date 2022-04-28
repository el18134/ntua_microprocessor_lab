#include <avr/io.h>
#include <avr/interrupt.h>

int i;
char out, pinb, pa2, cnt, y, w;

ISR(INT0_vect)					//������� ������������ �������� INT0
{
	pa2 = PINA & 0x04;			//���� ������� � ��� ��������� ��� PA2
	pinb = PINB;				//���� ������� ��� ��� ���� PINB
	cnt = 0;
	
	for(i = 0; i < 8; i++)		//������� �������� led ��� PINB ��� ���������� ������� ���� cnt
	{
		y = pinb & 0x01;		//���������� ������� 8 ����� ��� ������� ��� LSB ���� ����	
		pinb = pinb >> 1;
		if (y == 0x01)			//�� �� LSB ����� 1 ��������� ��� ���������� cnt
			cnt = cnt + 1;
	}
	
	if (pa2 == 0x04)			
	{
		PORTC = cnt;			//�� �� PA2 ����� ON ����������� ��� ������ �������� led �� ������� �����
	}
	else						//�� �� PA2 ����� OFF �������� ��� ������ led ���������� ��� �� LSB
	{
		out = 0x00;
		for (i = 0; i < cnt; i++)
		{
			out = out << 1;		//����������� �������� led ���� ����� (������ 1) ����� �� ������ ��� led ������ �������� ��� cnt
			out = out | 0x01;
		}
		PORTC = out;			//�������� ������ ���� ���� PORTC
	}
	
}

int main(void)
{
	DDRC = 0xFF;				// ������������ PORTB �� output
	DDRB = 0x00;				// ������������ PORTC �� input
	DDRA = 0x00;				// ������������ PORTA �� input
	GICR = 0x40;				//�������� �������� INT0
	MCUCR = 0x03;				//������������ �������� INT0 �� ���������� ����
	sei();						//������������ ��������
    
	while (1)					// Infinite loop ��� ��������� ��� ������������ ��������
    {
		w = w + 1;
	}
}
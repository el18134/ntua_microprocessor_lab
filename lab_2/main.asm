		.DEF icnt = r15
		.DEF a = r16
		.DEF cnt = r17

		.org 0x0
		rjmp reset
		.org 0x3
		rjmp ISR1

ISR1:	inc icnt				;������� ������� ��� ��������� ���� icnt
		in a, PINA
		andi a, 0xC0			;���� ������� ��� ��������� ��� PA6, PA7
		cpi a, 0xC0				;�� �� PA6, PA7 ����� ON ���� ����������� ��� ������ ��� �������� ���� ���� PORTB
		brne exit				;������ ��������� ��� ��� ������� �������� ����� �� ������� ����
		out PORTB, icnt
exit:	clr r18
		out PORTB, r18
		reti

reset:	ldi	r18, LOW(RAMEND)	;������������ stack pointer
		out	spl, r18
		ldi	r18, HIGH(RAMEND)
		out	sph, r18	

		ldi r18, 0x0C			
		out MCUCR, r18			;������������ �������� INT1 �� ���������� ����
		ldi r18, 0x80
		out GICR, r18			;�������� �������� INT1
		sei						;������������ ��������

		clr r18
		out DDRA, r18			;������������ PORTA �� input
		ser r18
		out DDRB, r18			;������������ PORTB �� output
		out DDRC, r18			;������������ PORTC �� output

		ldi cnt, 0x00			;A����������� ������� cnt
start:	out PORTC, cnt			;�������� �������� ���� ���� ������ PORTC
		inc cnt					;������ ��� ������� ��� ���������
		rjmp start
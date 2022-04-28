		.DEF i = r15
		.DEF a = r16
		.DEF b = r17
		.DEF c = r18
		.DEF d = r19
		.DEF na = r20
		.DEF nb = r21
		.DEF o1 = r22
		.DEF o2 = r23
		.DEF o3 = r24
		.DEF o4 = r25
		
		clr r31
		out DDRC, r31	;������������ PORTC �� input
		ser r31
		out DDRB, r31	;������������ PORTB �� output

start:	in i, PINC		;���� ������� ��� �� ���� PINC

		ldi r30, 0x01
		mov a, i
		and a, r30		;��������� ��� � ��� ���������� ��� register a
		ror i			;���������� ���� �� � �� ����� ���� ���� LSB
		mov b, i
		and b, r30		;��������� ��� B ��� ���������� ��� register b
		ror i			;���������� ���� �� C �� ����� ���� ���� LSB
		mov c, i
		and c, r30		;��������� ��� C ��� ���������� ��� register c
		ror i			;���������� ���� �� D �� ����� ���� ���� LSB
		mov d, i
		and d, r30		;��������� ��� D ��� ���������� ��� register d
		mov na, a
		com na			;���������� �� ���� 1 ��� � ��� ���������� ��� register na
		mov nb, b
		com nb			;���������� �� ���� 1 ��� B ��� ���������� ��� register nb

		mov o1, na
		and o1, b		;���������� ��� ���������� ��� ������� ������ (A'B) ��� register o1
		mov o2, nb		
		and o2, c
		and o2, d		;���������� ��� ���������� ��� ������� ������ (B'CD) ��� register o2		
		or o1, o2		;���������� ��� ���������� ��� ������� ������ (A'B + B'CD) ��� register o1
		com o1			;���������� �� ���� 1 ��� register o1 ��� ���������� ��� ������� ������ F0=(A'B + B'CD)'
		andi o1, 0x01	;��������� ��� ������������� F0 ��� register o1

		mov o3, a		
		and o3, c		;���������� ��� ���������� ��� ������� ������ (AC) ��� register o3
		mov o4, b
		or o4, d		;���������� ��� ���������� ��� ������� ������ (B+D) ��� register o4
		and o3, o4		;���������� ��� ���������� ��� ������� ������ F1=(AC)(B+D) ��� register o3

		rol o3			;���������� ��� ������������� F0 ���� �� ����� ���� ���� ��� 2�� LSB ��� ���������
		andi o3, 0x02	
		or o1, o3		;���������� ��� F0, F1 ��� register o1

		out PORTB, o1	;�������� ������������� ���� ���� PORTB
		rjmp start


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
		out DDRC, r31	;Αρχικοποίηση PORTC ως input
		ser r31
		out DDRB, r31	;Αρχικοποίηση PORTB ως output

start:	in i, PINC		;Λήψη εισόδου από τη θύρα PINC

		ldi r30, 0x01
		mov a, i
		and a, r30		;Απομόνωση του Α και αποθήκευση στο register a
		ror i			;Περιστροφή ώστε το Β να έρθει στην θέση LSB
		mov b, i
		and b, r30		;Απομόνωση του B και αποθήκευση στο register b
		ror i			;Περιστροφή ώστε το C να έρθει στην θέση LSB
		mov c, i
		and c, r30		;Απομόνωση του C και αποθήκευση στο register c
		ror i			;Περιστροφή ώστε το D να έρθει στην θέση LSB
		mov d, i
		and d, r30		;Απομόνωση του D και αποθήκευση στο register d
		mov na, a
		com na			;Συμπλήρωμα ως προς 1 του Α και αποθήκευση στο register na
		mov nb, b
		com nb			;Συμπλήρωμα ως προς 1 του B και αποθήκευση στο register nb

		mov o1, na
		and o1, b		;Δημιουργία και αποθήκευση της λογικής πράξης (A'B) στο register o1
		mov o2, nb		
		and o2, c
		and o2, d		;Δημιουργία και αποθήκευση της λογικής πράξης (B'CD) στο register o2		
		or o1, o2		;Δημιουργία και αποθήκευση της λογικής πράξης (A'B + B'CD) στο register o1
		com o1			;Συμπλήρωμα ως προς 1 στο register o1 και δημιουργία της λογικής πράξης F0=(A'B + B'CD)'
		andi o1, 0x01	;Απομόνωση του αποτελέσματος F0 στο register o1

		mov o3, a		
		and o3, c		;Δημιουργία και αποθήκευση της λογικής πράξης (AC) στο register o3
		mov o4, b
		or o4, d		;Δημιουργία και αποθήκευση της λογικής πράξης (B+D) στο register o4
		and o3, o4		;Δημιουργία και αποθήκευση της λογικής πράξης F1=(AC)(B+D) στο register o3

		rol o3			;Περιστροφή του αποτελέσματος F0 ώστε να έρθει στην θέση του 2ου LSB και απομόνωση
		andi o3, 0x02	
		or o1, o3		;Συνδυασμός των F0, F1 στο register o1

		out PORTB, o1	;Εμφάνιση αποτελέσματος στην θύρα PORTB
		rjmp start


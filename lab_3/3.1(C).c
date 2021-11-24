#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

char reg[2], mem[2], fd, sd;

char swap(char x) { //Υλοποίηση εντολής swap
	return ((x & 0x0F) << 4 | (x & 0xF0) >>4);
}

char scan_row(int r) { //Υλοποίηση ρουτίνας scan_row_sim
	char o = 0x08;
	o = (o << r);	//Ολίσθηση r φορές του o = 00001000
	PORTC = o;	//Θέτουμε την γραμμή που σκανάρουμε σε 1
	_delay_us(500);	//Καθυστέρηση 500ms για απομακρυσμένη λειτουργία
	return PINC & 0x0F; //Επιστροφή τεσσάρων  LSB της PORTC (στήλες πληκτρολογίου)
}

void scan_keypad() { //Υλοποίηση ρουτίνας scan_keypad_sim
	char c;
			
	c = scan_row(1); //Σκανάρισμα πρώτης σειράς
	reg[1] = swap(c); //Αποθήκευση στα 4 MSB των πρώτων 8 bit του 16-bit register reg
	
	c = scan_row(2); //Σκανάρισμα δεύτερης σειράς
	reg[1] = reg[1] + c; //Αποθήκευση στα 4 LSB των πρώτων 8 bit του 16-bit register reg
	
	c = scan_row(3); //Σκανάρισμα τρίτης σειράς
	reg[0] = swap(c); //Αποθήκευση στα 4 MSB των δεύτερων 8 bit του 16-bit register reg
	
	c = scan_row(4); //Σκανάρισμα τέταρτης σειράς
	reg[0] = reg[0] + c; //Αποθήκευση στα 4 LSB των δεύτερων 8 bit του 16-bit register reg
	
	PORTC = 0x00;
}

int scan_keypad_rising_edge() {	//Υλοποίηση ρουτίνας scan_row_rising_edge_sim
	char tmp[2];
	
	scan_keypad();	//Σκανάρισμα πληκτρολογίου 
	tmp[0] = reg[0]; //Προσωρινή αποθήκευση του αποτελέσματος
	tmp[1] = reg[1];
	
	_delay_ms(15);	//Αναμονή 15ms λόγω σπινθηρισμών
	
	scan_keypad();	//Δεύτερο σκανάρισμα πληκτρολογίου
	reg[0] = reg[0] & tmp[0]; //Απόρριψη πλήκτρων που εμφάνισαν σπινθηρισμό
	reg[1] = reg[1] & tmp[1];
	
	tmp[0] = mem[0]; //Λήψη προηγούμενης κατάστασης διακοπτών από την  RAM
	tmp[1] = mem[1];
	
	mem[0] = reg[0]; //Αποθήκευση τωρινής κατάστασης διακοπτών από την  RAM
	mem[1] = reg[1];
	
	reg[0] = reg[0] & (~tmp[0]); //Εύρεση διακοπτών που έχουν μόλις πατηθεί
	reg[1] = reg[1] & (~tmp[1]);
	
	return (reg[0] || reg[1]); //Επιστροφή των διακοπτών που μόλλις πατήθηκαν (0 αν δεν έχει πατηθεί πλήκτρο)
}

char keypad_to_ascii() { //Υλοποίηση ρουτίνας keypad_to_ascii_sim
	if ((reg[0]&0x01) == 0x01) 
	return '*';	//Εύρεση πατημένου πλήκτρου και επιστροφή του κωδικού ASCII που του αντιστοιχεί
	
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
	
	return 0;	//Εάν δεν έχει πατηθεί κάποιο πλήκτρο επιστρέφει 0
}

void welcome() {	//Υλοποίηση συνάρτησης welcome που ανάβει τα led για 4sec(σωστός κωδικός)
	char i;
	PORTB = 0xFF;	//Ανάβουμε τα led της PORTB
	for (i = 1; i <= 160; i++) { //160 επαναλήψεις διάρκειας 25ms: συνολική καθυστέρηση 4sec
		scan_keypad_rising_edge(); //Διάβασμα πληκτρολογίου κατά την διάρκεια ανάμματος των led (19ms)
		_delay_ms(6); //Πρόσθετη καθυστέρηση 6ms ώστε να έχουμε συνολική καθυστέρηση 25ms
	}
	PORTB = 0x00;	//Σβήνουμε τα  led της PORTB
}

void alarm() {		//Υλοποίηση συνάρτησης alarm που αναβοσβήνει τα led για 4sec(λάθος κωδικός)
	char i, j;
	for (j = 1; j <= 4; j++) { //4 επαναλήψεις διάρκειας 1sec: συνολική καθυστέρηση 4sec
		PORTB = 0xFF;	//Ανάβουμε τα led της PORTB
		for (i = 1; i <= 20; i++) { //20 επαναλήψεις διάρκειας 25ms: συνολική καθυστέρηση 0.5sec
			scan_keypad_rising_edge(); //Διάβασμα πληκτρολογίου κατά την διάρκεια που αναβοσβήνουν τα led (19ms)
			_delay_ms(6); //Πρόσθετη καθυστέρηση 6ms ώστε να έχουμε συνολική καθυστέρηση 25ms
		}
		PORTB = 0x00;	//Σβήνουμε τα  led της PORTB
		for (i = 1; i <= 20; i++) { //20 επαναλήψεις διάρκειας 25ms: συνολική καθυστέρηση 0.5sec
			scan_keypad_rising_edge(); //Διάβασμα πληκτρολογίου κατά την διάρκεια που αναβοσβήνουν τα led (19ms)
			_delay_ms(6); //Πρόσθετη καθυστέρηση 6ms ώστε να έχουμε συνολική καθυστέρηση 25ms
		}
	}
}

int main(void) {
	DDRB = 0xFF;	//Αρχικοποίηση PORTB ως έξοδο
	DDRC = 0xF0;	//Αρχικοποίηση 4ων MSB της PORTB ως έξοδο και 4ων LSB της PORTB ως είσοδο
	
	while (1) {
		mem[0] = 0x00;	//Αρχικοποίηση μεταβλητής μνήμης RAM
		mem[1] = 0x00;
		PORTB = 0x00;
		
		while (1) {
			if (scan_keypad_rising_edge() != 0) { //Διάβασμα πληκτρολογίου μέχρι να πατηθεί το 1ο πλήκτρο
				fd = keypad_to_ascii();	//Μετατροπή σε ψηφίο ASCII
				break;
			}
		}
		
		while (1) {
			if (scan_keypad_rising_edge() != 0) { //Διάβασμα πληκτρολογίου μέχρι να πατηθεί το 2ο πλήκτρο
				sd = keypad_to_ascii();	//Μετατροπή σε ψηφίο ASCII
				scan_keypad_rising_edge(); 
				break;
			}
		}
		
		if (fd != '4' || sd != '8') {
			alarm(); //Εάν δεν έχουν πατηθεί τα σωστά πλήκτρα καλούμε την alarm
		}
		else {
			welcome(); //Εάν έχουν πατηθεί τα σωστά πλήκτρα καλούμε την welcome
		}
		
	}
	return 0;
}

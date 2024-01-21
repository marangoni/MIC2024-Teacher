
//https://www.visualmicro.com/page/Timer-Interrupts-Explained.aspx

#define   led1  (1<<PB0) //led1 no pino 14(PB0)

int compareMatchReg;
volatile int interrupts;

void setup()
{
	Serial.begin(115200);
	
    
    // initialize timer1 
	noInterrupts();           // disable all interrupts
	
    DDRB  |=   led1;    //configura saída para o led1
  
    TCCR2A = 0;
	TCCR2B = 0;

	// Set compareMatchReg to the correct value for our interrupt interval
	// compareMatchReg = [16, 000, 000Hz / (prescaler * desired interrupt frequency)] - 1
	
	/* E.g. 1Hz with 1024 Pre-Scaler:
		compareMatchReg = [16, 000, 000 / (prescaler * 1)] - 1
		compareMatchReg = [16, 000, 000 / (1024 * 1)] - 1 = 15624
	
	    As this is > 256 Timer 1 Must be used for this..
	
    Freq = (16000000/(64*249))-1 = 1003,01
  
  */


	compareMatchReg = 249;   // preload timer from calc above - frequencia 1khz
	
	

	/*
	Prescaler:
		(timer speed(Hz)) = (Arduino clock speed(16MHz)) / prescaler
			So 1Hz = 16000000 / 1 --> Prescaler: 1
			Prescaler can equal the below values and needs the relevant bits setting
			1    [CS10]
			8    [CS11]
			64   [CS11 + CS10]
			256  [CS12]
			1024 [CS12 + CS10]
	*/
	
   TCCR2A  =  0x00;    //timer2 operando em modo normal
   TCCR2B  =  0x07;    //timer2 normal e prescaler (64)
   TCNT2   =  compareMatchReg;    //inicializa timer2
   TIMSK2  =  0x01;    //liga interrupção do timer2
   
    
    //TCCR1B |= (1 << CS12) ;    // 256 prescaler 
    //TCCR1B |=  (1 << CS11)  ;    // 8 prescaler 

    //TCCR1B  =  0x07;    //timer1 normal e prescaler
    //TCNT1   =    56;    //inicializa timer1
  
	//TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
	interrupts();          // enabl1 all interr64s
}

ISR(TIMER2_OVF_vect)        // interrupt service routine - Tick de 1ms (1khz)
{
	TCNT2 = compareMatchReg;   // preload timer - recarrega para nova interrupção
	PORTB ^= led1;             // pisca o led
}

void loop()
{
	// your program here...
}
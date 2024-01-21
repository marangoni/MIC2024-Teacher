/* ============================================================

   Curso Microcontroladores - IFSP - Engenharia de Controle e Automação
   
   Timer0 - Configuração em Modo Comparação

   Compilador: Arduino IDE 2.2.1

   MCU: Atmega328p
   Autor: D.Eng. Luiz Marangoni
   Data:  Janeiro de 2024


============================================================ */

// ============================================================
// --- Mapeamento de Hardware ---
#define   led1  (1<<PB0) //led1 no pino 14(PB0)

// ============================================================
// --- Protótipo das Funções ---



// ============================================================
// --- Variáveis Globais ---

int compareMatchReg; //Armazena valor utilizado no registro TCNT2 para definição da temporização de T2

// ============================================================
// --- Programa principal ---

void setup()
{
	Serial.begin(115200); //Configura comunicação serial
	  
  cli();                //desliga interrupção global
                        //garante que durante a execução do setup não irá ocorrer nenhuma interrupção  

  //---Configuração das portas de I/O

  DDRB  |=   led1;    //configura saída para o led1
  
  //---Calculo do valor do prescaler e TCNT2 para o valor desejado

  /*
	1) Definição do Prescaler -- (pág. 165 - datasheet ATMEGA328)
		
    (timer speed(Hz)) = (Arduino clock speed(16MHz)) / prescaler
			Exemplos:
      (timer speed(Hz) = 16000000 / 1 --> Prescaler: 16000000 (16 MHz -T=62.5ns)
      (timer speed(Hz) = 16000000 / 8 --> Prescaler: 2000000  (2 MHz - T=0.5us)
      (timer speed(Hz) = 16000000/ 64 --> Prescaler: 250000)  (250 kHz T=4us)
      E assim por diante

			Seleção do prescaler conforme bits de controle no registro TCCR2B
      Bit 2:0 - Seleção de clock

      Prescaler      [CS22]  [CS21]  [CS20]   Valor Hexa
      Timer parado    0       0       0          0x00
             1        0       0       1          0x01
             8        0       1       0          0x02
            32        0       1       1          0x03
            64        0       1       0          0x04
           128        1       0       1          0x05
           256        1       1       0          0x06
          1024        1       1       1          0x07
			
	*/
 
  /*
	2) Definição do valor de comparação no registro TCNT2 - Timer/Counter2

   TCNT2 - registro de 8 bits - valor maximo 255
   Obs: Os temporizadores 0 e 1 operam com 16 bits (TCNT0 e TCNT1 max 65536)

  // Defina compareMatchReg parao valor adequado para definição da frequencia de interrupção
  // pela equacao abaixo (seção 15.7.2 - pag.107 - datasheet):
	
  // compareMatchReg = [16, 000, 000Hz / (prescaler * desired interrupt frequency)] - 1
	
	/* Exemplo1: 1000 Hz with 64 Pre-Scaler:
		compareMatchReg = [16, 000, 000 / (2*prescaler * 1)] - 1
		compareMatchReg = [16, 000, 000 / (2*64 * 1000)] - 1 = 124
	  TCNT2 = 124 (ok)
	  
    Exemplo2: 200Hz com 32 Prescaler:
    compareMatchReg = [16, 000, 000 / (2*32 * 200)] - 1 = 1249 (! Maior que 256)

    Nesse caso como valor > 256 Timer outro valor de preescaler deve ser usado (prescaler de 256)
    
    compareMatchReg = [16, 000, 000 / (2*256 * 200)] - 1 = 156 (OK)

    Exemplo3: 20Hz com 1024 Prescaler:
    compareMatchReg = [16, 000, 000 / (2 * 1024 * 20)] - 1 = 389 (! Maior que 256)

    Nesse caso como valor > 256 Timer e o valor máximo do prescaler está sendo usado (1024) deve-se utilizar
    outro timer contador de 16 bits T0 ou T1...

    Planilha de calculo: https://docs.google.com/spreadsheets/d/1vszGjz69zTOy55wNSTbtfnCtlGTIg1VlipB1hBRfTmg/edit?usp=sharing
    
	*/
	
  // Configuração da interrupção de timer 2 - Metodo 1

  //Frequencia desejada: 1000 Hz - Preescaler: 64

   
   compareMatchReg = 124;          // preload do timer2 conforme calculo acima

   TCCR2A  =  0x00;                //timer2 operando em modo normal
   TCCR2B  =  0x04;                //timer2 normal e prescaler (64)
   TCNT2   =  compareMatchReg;     //inicializa timer2
   TIMSK2  =  0x01;                //liga interrupção do timer2

	
  // Configuração da interrupção de timer 2 - Metodo 2 - Usando os bits de controle
  //Frequencia desejada: 1000 Hz - Preescaler: 64
    
  //TCCR2B |=  (1 << CS21)  ;       // 64 prescaler 
  //TCNT2   =   compareMatchReg;    //inicializa timer2
	//TIMSK2 |= (1 << TOIE2);         // enable timer overflow interrupt

  sei();          // habilita as interrupções

 while(1)
   {
      //loop idle não faz nada
   } //end while

	
}


// ============================================================
// --- Interrupções ---

ISR(TIMER2_OVF_vect)        // Timer2 - interrupção - 1000Hz - 1ms
                            // TIC - clock da máquina de estados 
                            // Vetor de interrupção Timer2
{
	TCNT2 = 124;   // preload timer
	PORTB ^= led1;
} //end ISR


// ============================================================
// --- Loop Idle ---
void loop()
{
	// Não faz nada - programa baseado em interrupções
}

// ============================================================
// --- Desenvolvimento das Funções ---

  
 
  

// ============================================================
// --- FINAL DO PROGRAMA ---


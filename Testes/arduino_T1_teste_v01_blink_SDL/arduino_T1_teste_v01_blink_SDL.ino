/* ============================================================

   Curso Microcontroladores - IFSP - Engenharia de Controle e Automação
   
   Blink SDL - Implementação do blink utilizando interrupção de timer2 e modelagem em SDL - Specification and
   Description Language

   Compilador: Arduino IDE 2.2.1

   MCU: Atmega328p
   Autor: D.Eng. Luiz Marangoni
   Data:  Janeiro de 2024


============================================================ */

//=============================================================
// --- Recursos adicionais
//
// --- Modelagem do software em SDL
// https://online.visual-paradigm.com/community/share/blink-led-sdl-vpd-1m4ilt8yc6
//
// --- Simulação no TinkerCad
// https://www.tinkercad.com/things/ljKrEuSrNls-blinksdltimer
//

// ============================================================
// --- Mapeamento de Hardware ---

#define   tick  (1<<PB0) //led1 no pino 13(PB5) - led na placa do uno
#define   led1  (1<<PB5) //tick no pino 8 (PB0) - saida tick 1kHz


// ============================================================
// --- Protótipo das Funções ---

void control();
void timer_1s();
void led_1();

// ============================================================
// --- Variáveis Globais ---

volatile int count_T1s;
int compareMatchReg; //Armazena valor utilizado no registro TCNT2 para definição da temporização de T2

// Sinais utilizados nos blocos
int sinal_start_T1s = 0;
int sinal_done_T1s = 0;
int sinal_CMD_led1 = 0;

// Estados dos blocos
int estado_CONTROL = 1;
int estado_TIMER_1S = 1;
int estado_LED1 = 1;

// ============================================================
// --- Programa principal ---

void setup()
{
	Serial.begin(115200); //Configura comunicação serial
	  
  cli();                //desliga interrupção global
                        //garante que durante a execução do setup não irá ocorrer nenhuma interrupção  

  //---Configuração das portas de I/O

  DDRB  |=   led1;    //configura saída para o led1
  DDRB  |=   tick;    //configura saída para o tick
  	
  // Configuração da interrupção de timer 2 - Metodo 1

  //Frequencia desejada: 1000 Hz - Preescaler: 64

   
   compareMatchReg = 124;          // preload do timer2 conforme calculo acima

   TCCR2A  =  0x00;                //timer2 operando em modo normal
   TCCR2B  =  0x04;                //timer2 normal e prescaler (64)
   TCNT2   =  compareMatchReg;     //inicializa timer2
   TIMSK2  =  0x01;                //liga interrupção do timer2

	
  sei();          // habilita as interrupções

 while(1)
   {
      //loop idle não faz nada - programa "travado aqui"
   } //end while

	
}


// ============================================================
// --- Interrupções ---

ISR(TIMER2_OVF_vect)        // Timer2 - interrupção - 1000Hz - 1ms
                            // TIC - clock da máquina de estados 
                            // Vetor de interrupção Timer2
{
	TCNT2 = 124;   // preload timer
	
  PORTB ^= tick; //Saida Tick 1ms (somente teste) - pino 8
  
  // chama as funções dos blocos do sistema
  control();
  timer_1s(2000);
  led_1();
  

} //end ISR


// ============================================================
// --- Loop Idle ---
void loop()
{
	// Não faz nada - programa baseado em interrupções
  
}

// ============================================================
// --- Desenvolvimento das Funções ---

void control()
{
switch (estado_CONTROL) {
  case 1:
    // Estado 1 -
    // envia sinal de inicio da temporização
    sinal_start_T1s = 1;
    // vai para estado 2
    estado_CONTROL=2;
    break;
  case 2:
    // Estado 2 - Aguarda sinal de termino da temporização
    if (sinal_done_T1s == 1){
      sinal_done_T1s = 0;          //Consome o sinal
      estado_CONTROL = 3;          //Vai para o estado 3
    }
    break;
  case 3:
    // Estado 3 - Envia sinal para comandar o led
    sinal_CMD_led1 = 1;        //Envia sinal para comandar o led
    estado_CONTROL = 1;
    break;
  }
}  
 
void timer_1s(int tempo_timer){
// Timer de 1s
// Tempo = tick x count = 1ms x 1000 = 1s

  switch (estado_TIMER_1S) {
  case 1:
    // Estado 1 - aguarda start para temporização
    count_T1s = tempo_timer; 			    //inicializa contador de temporização
    if (sinal_start_T1s == 1){
     sinal_start_T1s = 0;       //Consome o sinal
     estado_TIMER_1S=2;         // vai para estado 2

    }
    break;
  case 2:
    // Estado 2 - decrementa contador e verifica se zerou
    
    count_T1s--;
    if (count_T1s == 0){
      estado_TIMER_1S=3;   //vai para o estado 3 
    }
   break;
  case 3:
   sinal_done_T1s = 1;    //Envia sinal indicando fim da temporização
   estado_TIMER_1S=1;          //vai para o estado 1
   break;
  }
}

void led_1(){
// Funcao de controle do led

  switch (estado_LED1) {
  case 1:
    // Estado 1 - aguarda sinal para inversão do led
    if (sinal_CMD_led1 == 1){
     sinal_CMD_led1 = 0;          //Consome o sinal
     estado_LED1=2;               // vai para estado 2
    }
    break;
  case 2:
    // Estado 2 - inverte o estado do led
    PORTB ^= led1;
    estado_LED1=1;        //vai para o estado 1 
   break;
  }
}


// ============================================================
// --- FINAL DO PROGRAMA ---


/* ============================================================

   Curso Microcontroladores - IFSP - Engenharia de Controle e Automação
   
   Blink SDL 3 Leds - Implementação do blink utilizando interrupção de timer2 e modelagem em SDL - Specification and
   Description Language
   
   Funcionamento:
   Led 1 - piscando 1Hz
   Led 2 - piscando 3Hz
   Led 3 - piscando 5Hz

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
// https://www.tinkercad.com/things/8rHgwRO4f55-blink3ledssdl

// ============================================================
// --- Mapeamento de Hardware ---

#define   tick  (1<<PB0) //Sinal de tick no pino 8 (PB0) - led na placa do uno


#define   led1  (1<<PB2) //led no pino 10 (PD2) - 1Hz
#define   led2  (1<<PB3) //led no pino 11 (PD3) - 3Hz
#define   led3  (1<<PB4) //led no pino 12 (PD4) - 5Hz

// ============================================================
// --- Protótipo das Funções ---

void control();
void timer_T1();
void timer_T2();
void timer_T3();
void led_1();
void led_2();
void led_3();

// ============================================================
// --- Variáveis Globais ---

volatile int count_T1;
volatile int count_T2;
volatile int count_T3;

int compareMatchReg; //Armazena valor utilizado no registro TCNT2 para definição da temporização de T2

// Sinais utilizados nos blocos
int sinal_start_T1 = 0;
int sinal_done_T1 = 0;
int sinal_CMD_led1 = 0;

int sinal_start_T2 = 0;
int sinal_done_T2 = 0;
int sinal_CMD_led2 = 0;

int sinal_start_T3 = 0;
int sinal_done_T3 = 0;
int sinal_CMD_led3 = 0;

// Estados dos blocos
int estado_CONTROL = 1;

int estado_TIMER_1 = 1;
int estado_TIMER_2 = 1;
int estado_TIMER_3 = 1;

int estado_LED1 = 1;
int estado_LED2 = 1;
int estado_LED3 = 1;


// ============================================================
// --- Programa principal ---

void setup()
{
	Serial.begin(115200); //Configura comunicação serial
	  
  cli();                //desliga interrupção global
                        //garante que durante a execução do setup não irá ocorrer nenhuma interrupção  

  //---Configuração das portas de I/O

  DDRB  |=   led1;    //configura saída para o led1
  DDRB  |=   led2;    //configura saída para o led2
  DDRB  |=   led3;    //configura saída para o led3
  
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
      //loop idle não faz nada - programa fica "travado aqui"
   } 

	
}


// ============================================================
// --- Interrupções ---

ISR(TIMER2_OVF_vect)        // Timer2 - interrupção - 1000Hz - 1ms
                            // TIC - clock da máquina de estados 
                            // Vetor de interrupção Timer2
{
	TCNT2 = 124;   // preload timer
	
  //PORTB ^= tick; //Saida Tick 1ms (somente teste) - pino 8
  
  // chama as funções dos blocos do sistema
  control();
  timer_T1(1000);
  timer_T2(3000);
  timer_T3(5000);
  
  led_1();
  led_2();
  led_3();
  
} //end ISR


// ============================================================
// --- Loop Idle ---
void loop()
{
	// Não faz nada - programa baseado em interrupções
  /*Serial.print("estado timer T3: ");
  Serial.println(estado_TIMER_3);
  
  Serial.print("estado led 3: ");
  Serial.println(estado_LED3);
  */
  
}

// ============================================================
// --- Desenvolvimento das Funções ---

void control()
{
switch (estado_CONTROL) {
  case 1:
    // Estado 1 -
    // envia sinal de inicio da temporização
    sinal_start_T1 = 1;
    sinal_start_T2 = 1;
    sinal_start_T3 = 1;
    // vai para estado 2
    estado_CONTROL=2;
    break;
  case 2:
    // Estado 2 - Aguarda sinal de termino da temporização
    if (sinal_done_T1 == 1){
      sinal_done_T1 = 0;           //Consome o sinal
      estado_CONTROL = 3;          //Vai para o estado 3
    }
  	else if(sinal_done_T2 ==1){
    	sinal_done_T2 = 0;			 //Consome sinal
    	estado_CONTROL = 4;			 //Vai para estado 4
  	}
   else if(sinal_done_T3 ==1){
    	sinal_done_T3 = 0;			 //Consome sinal
    	estado_CONTROL = 5;			 //Vai para estado 5
  	}
    break;
  case 3:
    // Estado 3 - Envia sinal para comandar o led 1
    sinal_CMD_led1 = 1;        //Envia sinal para comandar o led
    estado_CONTROL = 1;
    break;
  
  case 4:
    // Estado 4 - Envia sinal para comandar o led 2
    sinal_CMD_led2 = 1;        //Envia sinal para comandar o led
    estado_CONTROL = 1;
    break;
  
  case 5:
    // Estado 5 - Envia sinal para comandar o led 1
    sinal_CMD_led3 = 1;        //Envia sinal para comandar o led
    estado_CONTROL = 1;
    break;
  }
}  
 
void timer_T1(int tempo_timer){
// Timer de 1s
// Tempo = tick x count = 1ms x 1000 = 1s

  switch (estado_TIMER_1) {
  case 1:
    // Estado 1 - aguarda start para temporização
    count_T1 = tempo_timer; 			    //inicializa contador de temporização
    if (sinal_start_T1 == 1){
     sinal_start_T1 = 0;       //Consome o sinal
     estado_TIMER_1 = 2;         // vai para estado 2
    }
    break;
  case 2:
    // Estado 2 - decrementa contador e verifica se zerou
    
    count_T1--;
    if (count_T1 == 0){
      estado_TIMER_1 = 3;   //vai para o estado 3 
    }
   break;
  case 3:
   sinal_done_T1  = 1;    //Envia sinal indicando fim da temporização
   estado_TIMER_1 =1;          //vai para o estado 1
   break;
  }
}

void timer_T2(int tempo_timer){
// Timer de x seg
// Tempo = tick x count = 1ms x tempo_timer = x seg

  switch (estado_TIMER_2) {
  case 1:
    // Estado 1 - aguarda start para temporização
    count_T2 = tempo_timer; 			    //inicializa contador de temporização
    if (sinal_start_T2 == 1){
     sinal_start_T2 = 0;         //Consome o sinal
     estado_TIMER_2 = 2;         // vai para estado 2

    }
    break;
  case 2:
    // Estado 2 - decrementa contador e verifica se zerou
    
    count_T2--;
    if (count_T2 == 0){
      estado_TIMER_2 = 3;   //vai para o estado 3 
    }
   break;
  case 3:
   sinal_done_T2  = 1;    //Envia sinal indicando fim da temporização
   estado_TIMER_2 =1;          //vai para o estado 1
   break;
  }
}

void timer_T3(int tempo_timer){
// Timer de x seg
// Tempo = tick x count = 1ms x tempo_timer = x seg

  switch (estado_TIMER_3) {
  case 1:
    // Estado 1 - aguarda start para temporização
    count_T3 = tempo_timer; 			    //inicializa contador de temporização
    if (sinal_start_T3 == 1){
     sinal_start_T3 = 0;         //Consome o sinal
     estado_TIMER_3 = 2;         // vai para estado 2

    }
    break;
  case 2:
    // Estado 2 - decrementa contador e verifica se zerou
    
    count_T3--;
    if (count_T3 == 0){
      estado_TIMER_3 = 3;   //vai para o estado 3 
    }
   break;
  case 3:
   sinal_done_T3  = 1;    //Envia sinal indicando fim da temporização
   estado_TIMER_3 =1;          //vai para o estado 1
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

void led_2(){
// Funcao de controle do led

  switch (estado_LED2) {
  case 1:
    // Estado 1 - aguarda sinal para inversão do led
    if (sinal_CMD_led2 == 1){
     sinal_CMD_led2 = 0;          //Consome o sinal
     estado_LED2 = 2;               // vai para estado 2
    }
    break;
  case 2:
    // Estado 2 - inverte o estado do led
    PORTB ^= led2;
    estado_LED2 = 1;        //vai para o estado 1 
   break;
  }
}

void led_3(){
// Funcao de controle do led

  switch (estado_LED3) {
  case 1:
    // Estado 1 - aguarda sinal para inversão do led
    if (sinal_CMD_led3 == 1){
     sinal_CMD_led3 = 0;          //Consome o sinal
     estado_LED3 = 2;               // vai para estado 2
    }
    break;
  case 2:
    // Estado 2 - inverte o estado do led
    PORTB ^= led3;
    estado_LED3 = 1;        //vai para o estado 1 
   break;
  }
}


// ============================================================
// --- FINAL DO PROGRAMA ---


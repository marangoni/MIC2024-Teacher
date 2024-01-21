/* ============================================================
   Curso Microcontroladores - IFSP - Engenharia de Controle e Automação
   
   Botao temporizado - conforme o tempo determina a ação - Acionamento rápido função 1, acionamento lento função 2

   Utilizada o shield multi-function - arduino - 

   Compilador: Arduino IDE 2.2.1

   MCU: Atmega328p
   Autor: D.Eng. Luiz Marangoni
   Data:  Janeiro de 2024
============================================================ */

//=============================================================
// --- Recursos adicionais
//
// --- Modelagem do software em SDL
// https://online.visual-paradigm.com/community/share/button-action-led-sdl-vpd2-1mi3d7bl63
//
// --- Simulação no TinkerCad
// https://www.tinkercad.com/things/9GgyTi11FjK-botaoledactionsdl
//

// ============================================================
// --- Mapeamento de Hardware ---

#define   tick  (1<<PB5) // pino 13 tick - frequencia de interrupção -  TM2 - 1 KHz

#define   led1  (1<<PB2) // led 1 no pino D10 (PB2)
#define   led2  (1<<PB3) // led 2 no pino D1  (PB3)
#define   bt_s1 (1<<PC1) // entrada do botao (A1)


// ============================================================
// --- Protótipo das Funções ---

void control();
void botao_1();
void led_1();
void led_2();
void timer_1(int tempo_timer);
void cronometro_1();
//void testes();

// ============================================================
// --- Variáveis Globais ---

volatile int CRON1_time = 0;      //armazena tempo em que o botao ficou pressionado
volatile int TIMER1_count = 0;    //armazena tempo decorrido de timer1

char compareMatchReg; //Armazena valor utilizado no registro TCNT2 para definição da temporização de T2

// Estados

char estado_CONTROL = 1;
char estado_BOTAO1 = 1;
char estado_LED1 = 1;
char estado_LED2 = 1;
char estado_CRON1 = 1;
char estado_TIMER1 = 1;


// Mensagens utilizadas nos blocos

char msg_BOTAO_1_on = 0;
char msg_BOTAO_1_off = 0;

char msg_LED1_cmd = 0;
char msg_LED1_on = 0;

char msg_LED2_cmd = 0;
char msg_LED2_on = 0;

char msg_CRON1_start = 0;
char msg_CRON1_stop = 0;
char msg_CRON1_done = 0;

char msg_T1_start = 0;
char msg_T1_done = 0;

// flags
char f_botao_1_on = 0;
char f_LED1_status = 0;
char f_LED2_status = 0;



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
  
  PORTB |= led1;     //led inicia desligado - ** Atenção:** Logica invertida do Shield Multifunçcoes - 0 apaga e 1 acende
  PORTB |= led2;     //led inicia desligado 

  DDRC  &=   ~bt_s1;  //configura bt_S1 como entrada
  PORTC |=   bt_s1;   //configura bt_S1 como pull-up
  	
  // Configuração da interrupção de timer 2 

  //Frequencia desejada: 1000 Hz - Preescaler: 64
   compareMatchReg = 124;          // preload do timer2 conforme calculo acima

   TCCR2A  =  0x00;                //timer2 operando em modo normal
   TCCR2B  =  0x04;                //timer2 normal e prescaler (64)
   TCNT2   =  compareMatchReg;     //inicializa timer2
   TIMSK2  =  0x01;                //liga interrupção do timer2

	
  sei();          // habilita as interrupções
 
 while(1)
   {
      
        
   } //end while
 
	

}


// ============================================================
// --- Interrupções ---

ISR(TIMER2_OVF_vect)        // Timer2 - interrupção - 1000Hz - 1ms
                            // TIC - clock da máquina de estados 
                            // Vetor de interrupção Timer2
                            // Função executada a cada 1ms
{
	TCNT2 = 124;   // preload timer
	
  //PORTB ^= tick; //Saida Tick 1ms (somente teste) - pino 13
  // chama as funções dos blocos do sistema
  control();
  botao_1();
  led_1();
  led_2();
  cronometro_1();
  timer_1(2000);
  

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
    // Estado 1 - Aguarda sinal de acioamento do botao
    

    if (msg_BOTAO_1_on == 1){
      msg_BOTAO_1_on = 0;          //Consome o sinal
      msg_CRON1_start = 1;         //Envia sinal para inicio do cronometro 1
      estado_CONTROL = 2;          //Vai para o estado 2
    }
    break;
  case 2:
    // Estado 2 - Aguarda sinal de liberação do botao
  if(msg_BOTAO_1_off == 1){
    msg_BOTAO_1_off = 0;	      //Consome o sinal
    
    msg_CRON1_stop = 1;         //Envia sinal para pararo cronometro 1   
    
    estado_CONTROL = 3;
  }
    break;

  case 3:
    // Estado 3 - Aguarda sinal de termino da temporização do botao
  if(msg_CRON1_done == 1){
    msg_CRON1_done = 0;	         //Consome o sinal
    
    if (CRON1_time > 1000){      //acionamento longo
      
      estado_CONTROL = 4;
    }       
    else if (CRON1_time <= 1000){
      
      estado_CONTROL = 5;
    } 
  }
    break;
  case 4:
    msg_LED1_cmd = 1;           //Envia sinal para comandar o led
    msg_T1_start = 1;           //Envia sinal para iniciar timer 1
    estado_CONTROL = 6;         //Vai para o estado 6
  break;

  case 5:
    msg_LED2_cmd = 1;           //Envia sinal para comandar o led
    msg_T1_start = 1;           //Envia sinal para iniciar timer 1
    estado_CONTROL = 6;         //Vai para o estado 6
  break;

  case 6:
    // verifica se recebeu sinal para termino da temporização de t1
    //Verifica status dos leds

    

    if(msg_T1_done == 1){
      msg_T1_done = 0;          //Consome sinal
      
      msg_LED1_cmd = 1;         //Envia sinal para apagar o led 1
      
     
      msg_LED2_cmd = 1;         //Envia sinal para apagar o led 2
      
      estado_CONTROL = 1;       //Volta para o estado 1
    }
   break;
  }
}  
 
void botao_1(){
  //função que verifica acionamento do botão

  switch (estado_BOTAO1){
    case 1:
      if ((PINC & bt_s1) == 0){     // Se botao pressionado (=0 pois é pull-up)
         msg_BOTAO_1_on = 1;        // envia sinal botao pressionado
         f_botao_1_on = 1;
         estado_BOTAO1 = 2;         //vai para o estado 2   
      }  
    break;
    
    case 2:
      if ((PINC & bt_s1) == bt_s1 && f_botao_1_on){     // Se botao liberado (=1 pois é pull-up) apos ter sido pressionado...
         f_botao_1_on = 0;       //  consome o flag para novo acionamento

         msg_BOTAO_1_off= 1;      // envia sinal botao liberado
         estado_BOTAO1 = 1;       //vai para o estado 2
      } 
    break;
  }
}

void led_1(){
// Funcao de controle do led

    
    

  switch (estado_LED1) {
  case 1:
    // Estado 1 - aguarda sinal para inversão do led
    //f_LED1_status = PINB & led1;
    if (msg_LED1_cmd == 1){
     msg_LED1_cmd = 0;             //Consome o sinal para novo acionamento
     PORTB &= ~led1;                //Acende led
     msg_LED1_on = 1;               //Envia sinal led aceso
     estado_LED1=2;                //vai para estado 2
    }
    break;
  case 2:
    // Estado 2 - aguarda sinal para apagar o led
    //f_LED1_status = PINB & led1;
    if (msg_LED1_cmd == 1){
      msg_LED1_cmd = 0;     //Consome o sinal para novo acionamento
      PORTB |= led1;       //Apaga o led
      msg_LED1_on = 0;      //Sinal led não está aceso
      estado_LED1=1;        //vai para o estado 1 
    }
   break;
  }
}

void led_2(){
// Funcao de controle do led
  
  

  switch (estado_LED2) {
  case 1:
    // Estado 1 - aguarda sinal para inversão do led
    //f_LED2_status = PINB & led2;
    if (msg_LED2_cmd == 1){
     msg_LED2_cmd = 0;             //Consome o sinal para novo acionamento
     PORTB &= ~led2;                //Acende led
     msg_LED2_on = 1;               //Envia sinal led aceso
     estado_LED2=2;                //vai para estado 2
    }
    break;
  case 2:
    // Estado 2 - aguarda sinal para apagar o led
    //f_LED2_status = PINB & led2;
    if (msg_LED2_cmd == 1){
      msg_LED2_cmd = 0;     //Consome o sinal para novo acionamento
      PORTB |= led2;       //Apaga o led
      msg_LED2_on = 0;      //Sinal led não está aceso
      estado_LED2=1;        //vai para o estado 1 
    }
   break;
  }
}


void cronometro_1(){
  /* Funcao cronometro 
    1) recebe o sinal start
    2) Conta o número de ticks - cada tick é 1ms
    3) após o sinal stop atualiza a variavel global CRON1_time
  */
switch (estado_CRON1) {
  case 1:
    // Estado 1 - aguarda sinal de start do cronometro
    if (msg_CRON1_start == 1){
     msg_CRON1_start = 0;              //Consome o sinal para novo acionamento
     CRON1_time = 0;                   //Inicializa tempo
     estado_CRON1=2;                   //vai para estado 2
    }
    break;
  case 2:
    // Estado 2 - aguarda sinal para apagar o led
    CRON1_time++;                     //incrementa o contador de ticks

    if (msg_CRON1_stop == 1){         //verifica se recebeu o sinal de stop, caso afirmativo vai para o estado 3
      msg_CRON1_stop = 0;             //Consome o sinal para novo acionamento
      msg_CRON1_done = 1;             //Envia sinal de término da temporização
      estado_CRON1=1;                 //vai para o estado 1
    }
   break;
  }

}


void timer_1(int tempo_timer){
// Timer de 1s
// Tempo = tick x count = 1ms x 1000 = 1s

  switch (estado_TIMER1) {
  case 1:
    // Estado 1 - aguarda start para temporização
    TIMER1_count = tempo_timer; 			    //inicializa contador de temporização
    if (msg_T1_start == 1){
     msg_T1_start = 0;       //Consome o sinal
     estado_TIMER1 = 2;         // vai para estado 2

    }
    break;
  case 2:
    // Estado 2 - decrementa contador e verifica se zerou
    
    TIMER1_count--;
    if (TIMER1_count == 0){
      estado_TIMER1 = 3;   //vai para o estado 3 
    }
   break;
  case 3:
   msg_T1_done = 1;    //Envia sinal indicando fim da temporização
   estado_TIMER1 = 1;          //vai para o estado 1
   break;
  }
}

/*
void testes(){
      // Função de testes
      Serial.print("Estado controle: ");
      Serial.println(int(estado_CONTROL));
      //Serial.print("Estado botao: ");   
      //Serial.println(int(estado_BOTAO1));
      //Serial.print("Estado LED1: ");   
      //Serial.println(int(estado_LED1));
      //Serial.print("Estado LED2: ");   
      //Serial.println(int(estado_LED2));
      //Serial.print("Estado Cronometro: ");   
      //Serial.println(int(estado_CRON1));
      Serial.print("Valor cronometro: ");
      Serial.println(CRON1_time);
      //Serial.print("Estado Timer1: ");   
      //Serial.println(int(estado_TIMER1)); 
      //delay(1000);
}
*/

// ============================================================
// --- FINAL DO PROGRAMA ---


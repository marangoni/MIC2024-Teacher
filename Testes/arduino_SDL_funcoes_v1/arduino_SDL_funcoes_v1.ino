/*
  Arquivo com as funções em SDL para utilização

  OBS: 

  1) Utiliza o Multishield arduino
  https://blog.eletrogate.com/guia-completo-do-shield-multi-funcoes-para-arduino/

  v0.3 - implementa a funcao led e seus sinais
  
*/

// ---- Mapeamento do Hardware - Shield Multifunções arduino -----

//LEDS
#define   led4  (1<<PB5) // led 1 no pino D13
#define   led3  (1<<PB4) // led 2 no pino D12
#define   led2  (1<<PB3) // led 2 no pino D11
#define   led1  (1<<PB2) // led 2 no pino D10

//Botoes
#define   bt_s1 (1<<PC1) // Botao no pino A1
#define   bt_s2 (1<<PC2) // Botao no pino A2
#define   bt_s3 (1<<PC3) // Botao no pino A3

//Buzzer
#define  buzzer (1<<PD3) //Buzzer no pino D3

//Potenciometro
#define  pot (1<<PC0)    //Potenciometro no pino A0

//Variáveis globais - podem ser acessadas por todas as funções

bool led1_status = 0;

//Prototipo das funcoes

void led_1();
void led_2();
void led_3();
void led_4();

// botoes
void botao_1();
void botao_2();
void botao_3();

// cronometro
void cronometro_1();

// timer
void timer_1(int tempo_timer);



//---- Definicao dos sinais (msg) e estados utilizados nos blocos

// LEDS
char msg_LED1_on = 0;
char msg_LED1_off = 0;
char estado_LED_1 = 1;

char msg_LED2_on = 0;
char msg_LED2_off = 0;
char estado_LED_2 = 1;

char msg_LED3_on = 0;
char msg_LED3_off = 0;
char estado_LED_3 = 1;

char msg_LED4_on = 0;
char msg_LED4_off = 0;
char estado_LED_4 = 1;

//BOTOES
char msg_BOTAO_1_on = 0;
char msg_BOTAO_1_off = 0;
char estado_BOTAO1 = 1;
char f_botao_1_on = 0;

char msg_BOTAO_2_on = 0;
char msg_BOTAO_2_off = 0;
char estado_BOTAO2 = 1;
char f_botao_2_on = 0;

char msg_BOTAO_3_on = 0;
char msg_BOTAO_3_off = 0;
char estado_BOTAO3 = 1;
char f_botao_3_on = 0;

//Cronometro
int CRON1_time = 0;      //armazena tempo em que o botao ficou pressionado
char estado_CRON1 = 1;
char msg_CRON1_start = 0;
char msg_CRON1_stop = 0;
char msg_CRON1_done = 0;

//Timer
 
int TIMER1_count = 0;    //armazena tempo decorrido de timer1
char estado_TIMER1 = 1;
char msg_T1_start = 0;
char msg_T1_done = 0;



void setup() {
  // equivalente em C ao void main()
  
  //--- Configuração da comunicação serial para depuração
  Serial.begin(115200);                    
  
  //---Configuração das portas de I/O

  //Configura os pinos dos leds 1 a 4 como saída (=1)
  DDRB  |=   led1;    
  DDRB  |=   led2;    
  DDRB  |=   led3;
  DDRB  |=   led4;
  
  // Inicializa os pinos leds 1 a 4 com o estado ligado (=1) - leds iniciam apagados no multishield
  PORTB |= led1;     
  PORTB |= led2;     
  PORTB |= led3;     
  PORTB |= led4;     

  // Configura os pinos dos botoes como entrada - pullup

  DDRC  &=   ~bt_s1;  //configura bt_S1 como entrada
  DDRC  &=   ~bt_s2;  //configura bt_S2 como entrada
  DDRC  &=   ~bt_s3;  //configura bt_S3 como entrada

  PORTC |=   bt_s1;   //configura bt_S1 como pull-up
  PORTC |=   bt_s2;   //configura bt_S2 como pull-up
  PORTC |=   bt_s3;   //configura bt_S3 como pull-up

  while(1){ //equivalente em C ao void loop()
    
  
  }
  
}

void loop() {
  //nao utilizado
  
}

// ------- Implementação das funções --------

// LEDS

void led_1(){
   //funcao que comando o led 1
   // ** Importante ** - Shield multifunção acende o led com sinal 0V (nivel logico 0) e apaga com 5V (nivel logico 1

   switch(estado_LED_1){

   case 1: //Aguarda um comando para ligar
    
    if (msg_LED1_on == 1){
       msg_LED1_on = 0;     //consome o sinal
       PORTB &= ~led1;      //acende o led
       estado_LED_1 = 2;     //vai para o estado 2
    }

   break;

   case 2: //aguarda comando para desligar
      
     if (msg_LED1_off == 1){
       msg_LED1_off = 0;    //consome o sinal
       PORTB |= led1;       //apaga o led
       estado_LED_1 = 1;     //vai para o estado 2
    }
   break;
   }
} //fim do bloco


void led_2(){
   //funcao que comando o led 2
   // ** Importante ** - Shield multifunção acende o led com sinal 0V (nivel logico 0) e apaga com 5V (nivel logico 1

   switch(estado_LED_2){

   case 1: //Aguarda um comando para ligar
    
    if (msg_LED2_on == 1){
       msg_LED2_on = 0;     //consome o sinal
       PORTB &= ~led2;      //acende o led
       estado_LED_2 = 2;     //vai para o estado 2
    }

   break;

   case 2: //aguarda comando para desligar
      
     if (msg_LED2_off == 1){
       msg_LED2_off = 0;    //consome o sinal
       PORTB |= led2;       //apaga o led
       estado_LED_2 = 1;     //vai para o estado 2
    }
   break;
   }
} //fim do bloco


void led_3(){
   //funcao que comando o led 2
   // ** Importante ** - Shield multifunção acende o led com sinal 0V (nivel logico 0) e apaga com 5V (nivel logico 1

   switch(estado_LED_3){

   case 1: //Aguarda um comando para ligar
    
    if (msg_LED3_on == 1){
       msg_LED3_on = 0;     //consome o sinal
       PORTB &= ~led3;      //acende o led
       estado_LED_3 = 2;     //vai para o estado 2
    }

   break;

   case 2: //aguarda comando para desligar
      
     if (msg_LED3_off == 1){
       msg_LED3_off = 0;    //consome o sinal
       PORTB |= led3;       //apaga o led
       estado_LED_3 = 1;     //vai para o estado 2
    }
   break;
   }
} //fim do bloco

void led_4(){
   //funcao que comando o led 2
   // ** Importante ** - Shield multifunção acende o led com sinal 0V (nivel logico 0) e apaga com 5V (nivel logico 1

   switch(estado_LED_4){

   case 1: //Aguarda um comando para ligar
    
    if (msg_LED4_on == 1){
       msg_LED4_on = 0;     //consome o sinal
       PORTB &= ~led4;      //acende o led
       estado_LED_4 = 2;     //vai para o estado 2
    }

   break;

   case 2: //aguarda comando para desligar
      
     if (msg_LED4_off == 1){
       msg_LED4_off = 0;    //consome o sinal
       PORTB |= led4;       //apaga o led
       estado_LED_4 = 1;     //vai para o estado 2
    }
   break;
   }
} //fim do bloco

// --- BOTOES

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

void botao_2(){
  //função que verifica acionamento do botão

  switch (estado_BOTAO2){
    case 1:
      if ((PINC & bt_s2) == 0){     // Se botao pressionado (=0 pois é pull-up)
         msg_BOTAO_2_on = 1;        // envia sinal botao pressionado
         f_botao_2_on = 1;
         estado_BOTAO2 = 2;         //vai para o estado 2   
      }  
    break;
    
    case 2:
      if ((PINC & bt_s2) == bt_s2 && f_botao_2_on){     // Se botao liberado (=1 pois é pull-up) apos ter sido pressionado...
         f_botao_2_on = 0;       //  consome o flag para novo acionamento

         msg_BOTAO_2_off= 1;      // envia sinal botao liberado
         estado_BOTAO2 = 1;       //vai para o estado 2
      } 
    break;
  }
}

void botao_3(){
  //função que verifica acionamento do botão

  switch (estado_BOTAO3){
    case 1:
      if ((PINC & bt_s3) == 0){     // Se botao pressionado (=0 pois é pull-up)
         msg_BOTAO_3_on = 1;        // envia sinal botao pressionado
         f_botao_3_on = 1;
         estado_BOTAO3 = 2;         //vai para o estado 2   
      }  
    break;
    
    case 2:
      if ((PINC & bt_s3) == bt_s3 && f_botao_3_on){     // Se botao liberado (=1 pois é pull-up) apos ter sido pressionado...
         f_botao_3_on = 0;       //  consome o flag para novo acionamento

         msg_BOTAO_3_off= 1;      // envia sinal botao liberado
         estado_BOTAO3 = 1;       //vai para o estado 2
      } 
    break;
  }
}

// --- CRONOMETRO

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

// --- TIMERS

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
/* ============================================================
   Curso Microcontroladores - IFSP - Engenharia de Controle e Automação
   
   Button 1 SDL - Leitura simples de 1 botão implementada em SDL

   Utilizada o shield multi-function - arduino - pressionando-se o botao inverte condição do led

   Compilador: Arduino IDE 2.2.1

   MCU: Atmega328p
   Autor: D.Eng. Luiz Marangoni
   Data:  Janeiro de 2024
============================================================ */

//=============================================================
// --- Recursos adicionais
//
// --- Modelagem do software em SDL
// https://online.visual-paradigm.com/community/share/button-led-sdl-vpd-1m5ypg44nu
//
// --- Simulação no TinkerCad
// https://www.tinkercad.com/things/ljKrEuSrNls-blinksdltimer
//

// ============================================================
// --- Mapeamento de Hardware ---

#define   led1  (1<<PB5) // led no pino 13 (PB5) - saida tick 1kHz
#define   bt_s1 (1<<PC1) // entrada do botao (PC1)


// ============================================================
// --- Protótipo das Funções ---

void control();
void botao_1();
void led_1();
void temporizador_bt1();

// ============================================================
// --- Variáveis Globais ---

volatile int tempo_botao1 = 0; //armazena tempo em que o botao ficou pressionado
char compareMatchReg; //Armazena valor utilizado no registro TCNT2 para definição da temporização de T2

// Sinais utilizados nos blocos

char sinal_botao_1_on = 0;
char sinal_botao_1_off = 0;
char sinal_CMD_led1 = 0;
bool f_botao_on = 0;

// Estados dos blocos
char estado_CONTROL = 1;
char estado_BOTAO1 = 1;
char estado_LED1 = 1;

// ============================================================
// --- Programa principal ---

void setup()
{
	Serial.begin(115200); //Configura comunicação serial
	  
  cli();                //desliga interrupção global
                        //garante que durante a execução do setup não irá ocorrer nenhuma interrupção  

  //---Configuração das portas de I/O

  DDRB  |=   led1;    //configura saída para o led1
  
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
	
  //PORTB ^= tick; //Saida Tick 1ms (somente teste) - pino 8
  
  // chama as funções dos blocos do sistema
  control();
  botao_1();
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
    // Estado 1 - Aguarda sinal de acioamento do botao
    if (sinal_botao_1_on == 1){
      sinal_botao_1_on = 0;          //Consome o sinal
      estado_CONTROL = 2;          //Vai para o estado 2
    }
    break;
  case 2:
    // Estado 2 - Envia sinal para comandar o led
    sinal_CMD_led1 = 1;        //Envia sinal para comandar o led
    estado_CONTROL = 1;
    break;
  }
}  
 
void botao_1(){
  //função que verifica acionamento do botão

  switch (estado_BOTAO1){
    case 1:
      if ((PINC & bt_s1) == 0){     // Se botao pressionado (=0 pois é pull-up)
         sinal_botao_1_on = 1;      // envia sinal botao pressionado
         f_botao_on = 1;            // indica que botao foi pressionado
         estado_BOTAO1 = 2;         //vai para o estado 2
         tempo_botao1++;
         
      }  
    break;
    
    case 2:
      if ((PINC & bt_s1) == bt_s1 && f_botao_on){     // Se botao liberado (=1 pois é pull-up) apos ter sido pressionado...
         f_botao_on = 0;         //  consome o flag para novo acionamento

         sinal_botao_1_off = 1;   // envia sinal botao liberado
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


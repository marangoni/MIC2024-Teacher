/*
  Blink - acesso  com portas
  Utiliza o Multishield arduino
  https://blog.eletrogate.com/guia-completo-do-shield-multi-funcoes-para-arduino/

  v0.3 - implementa a funcao led e seus sinais
  
*/
// Mapeamento do Hardware - Shield Multifunções arduino

//LEDS
#define   led4  (1<<PB5) // led 1 no pino D13
#define   led3  (1<<PB4) // led 2 no pino D12
#define   led2  (1<<PB3) // led 2 no pino D11
#define   led1  (1<<PB2) // led 2 no pino D10

//Botoes
#define   s1_bt (1<<PC1) // Botao no pino A1
#define   s2_bt (1<<PC2) // Botao no pino A2
#define   s3_bt (1<<PC3) // Botao no pino A3

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


//Definicao dos sinais (msg) utilizados nos blocos

char msg_LED1_on = 0;
char msg_LED1_off = 0;

//Definicao dos estados dos blocos

char estado_LED_1 = 1;


void setup() {
  // equivalente em C ao void main()
  
  //---Configuração das portas de I/O
  
  Serial.begin(115200);
  
  //Configura os pinos dos leds 1 e 2 como saída (=1)
  DDRB  |=   led1;    //configura saída para o led1
  DDRB  |=   led2;    //configura saída para o led2  
  
  // Inicializa os pinos leds 1 e 2 com o estado ligado (=0)
  PORTB |= led1;     //led inicia desligado (=0) - placa do shield leds ligados ao 5V precisam de nivel 0 para acionamento
  PORTB |= led2;     //led inicia desligado (=0)

  while(1){ //equivalente em C ao void loop()
    
    msg_LED1_on = 1;  //comanda acendimento do led
    msg_LED1_off = 0;
    led_1();
    delay(1000);
    
    bool flag_led1 = PINB & led1;
    Serial.print("leitura da porta led1: ");
    Serial.println(int(flag_led1));

    msg_LED1_on = 0;
    msg_LED1_off = 1;    
     
     led_1();
     delay(1000);
   
    flag_led1 = PINB & led1;
    Serial.print("leitura da porta led1: ");
    Serial.println(int(flag_led1));
  
  }
  
}

void loop() {
  //nao utilizado
  
}

void led_1(){
   //funcao que comando o led 1

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
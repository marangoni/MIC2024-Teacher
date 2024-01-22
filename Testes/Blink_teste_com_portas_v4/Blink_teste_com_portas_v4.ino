/*
  Blink - acesso  com portas
  Utiliza o Multishield arduino
  https://blog.eletrogate.com/guia-completo-do-shield-multi-funcoes-para-arduino/

  v0.4 - Implementa utilização dos sinais via registrador de sinais - otimiza o uso da memoria
  
*/

// ----- Mapeamento do Hardware - Shield Multifunções arduino
//LEDS

#define   led1  (1<<PB2) // led 2 no pino D10

// ------------ DEFINIÇÃO DO REGISTRO DE SINAIS (msg)  - 8 bits ----------------

uint8_t REG_SINAIS = 0;       //registro de 8 bits, inteiro sem sinal, 0 a 255
                              //inicializado com zero

// Bits indicadores dos sinais utilizados nos blocos

#define msg_LED1_on   (1<<0)         //bit 0 - msg_LED1_on
#define msg_LED1_off  (1<<1)         //bit 1 - msg_LED1_off

//----------------------------------------------------------------------------

//Variáveis globais - podem ser acessadas por todas as funções




//-------- Prototipo das funcoes

void led_1();

//void view_REG_SINAIS();





//Definicao dos estados dos blocos

char estado_LED_1 = 1;


void setup() {
  // equivalente em C ao void main()
  
  //---Configuração das portas de I/O
  
  Serial.begin(115200);
  
  //Configura os pinos dos leds 1 e 2 como saída (=1)
  DDRB  |=   led1;    //configura saída para o led1
  
  // Inicializa os pinos leds 1 e 2 com o estado ligado (=0)
  PORTB |= led1;     //led inicia desligado (=0) - placa do shield leds ligados ao 5V precisam de nivel 0 para acionamento

  

  while(1){ //equivalente em C ao void loop()
    
    
    REG_SINAIS |= msg_LED1_on;       //comanda acendimento do led
    
    //view_REG_SINAIS(); 

    led_1();
    delay(1000);
    
    bool flag_led1 = PINB & led1;
            
    REG_SINAIS |= msg_LED1_off;    //comanda apagamento do led

    //view_REG_SINAIS(); 
     
     led_1();
     delay(1000);   
    flag_led1 = PINB & led1;      //Leitura do status da porta PB2 onde está o led1 - led aceso?  
  
  }  
}

void loop() {
  //nao utilizado
  
}

void led_1(){
   //funcao que comando o led 1

   switch(estado_LED_1){

   case 1: //Aguarda um comando para ligar
    
    if (((REG_SINAIS & msg_LED1_on) == msg_LED1_on)){  //Testa se sinal foi enviado  
       
       REG_SINAIS &= ~msg_LED1_on;  //consome o sinal
       PORTB &= ~led1;              //acende o led
       estado_LED_1 = 2;            //vai para o estado 2
    }

   break; 

   case 2: //aguarda comando para desligar
      
     if((REG_SINAIS & msg_LED1_off) == msg_LED1_off){
        
       REG_SINAIS &= ~msg_LED1_off; //consome o sinal
       PORTB |= led1;               //apaga o led
       estado_LED_1 = 1;            //vai para o estado 2
    }
   break;
   }
} //fim do bloco

/*
void view_REG_SINAIS(){
  // Funcao para visualização do registrador com os sinais para depuração
   
  byte mbyte = REG_SINAIS;
  Serial.print("REG_SINAIS: ");

  for (int i = 0; i < 8; i++)  { 
    bool b = mbyte & B10000000;  //mascara os bits 0 a 6, somente o bit 7 será avaliado
    Serial.print(b); 
    mbyte = mbyte << 1;
  }
  Serial.println();
}//fim do bloco
*/

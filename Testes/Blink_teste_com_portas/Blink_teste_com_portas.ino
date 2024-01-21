/*
  Blink - acesso  com portas
  Utiliza o Multishield arduino
  https://blog.eletrogate.com/guia-completo-do-shield-multi-funcoes-para-arduino/

  

  
*/
// Mapeamento do Hardware - Shield Multifunções arduino

//LEDS
#define   led1  (1<<PB5) // led 1 no pino D13
#define   led2  (1<<PB4) // led 2 no pino D12
#define   led3  (1<<PB3) // led 2 no pino D11
#define   led4  (1<<PB2) // led 2 no pino D10

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
    
    
    PORTB |= led1;  //desliga led1
    PORTB &= ~led2; //liga led2
    delay(1000);
    
    PORTB &= ~led1;  //liga led1
    PORTB |=  led2; //desliga     led2
    delay(1000);

  
  }
  
}

 

void loop() {
  //nao utilizado
  
}

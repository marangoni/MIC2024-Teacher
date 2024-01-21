/*
  Blink - Teste com portas v2

  Escrita simultanea nos 4 bits da porta B - PB2 a PB5

  
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
  
  //Configura os pinos dos leds 1 a 4 como saida
  DDRB  |=   led1;    //configura saída para o led1
  DDRB  |=   led2;    //configura saída para o led2 
  DDRB  |=   led3;    
  DDRB  |=   led4;

  
  // Inicializa os pinos leds 1 e 2 com o estado ligado (=0)
  PORTB |= led1;     //led inicia desligado (=0) - placa do shield leds ligados ao 5V precisam de nivel 0 para acionamento
  PORTB |= led2;     //led inicia desligado (=0)
  PORTB |= led3;
  PORTB |= led4;
  
  // Organizacao dos pino PortaB
  //
  // PB7  PB6  PB5  PB4  PB3  PB2  PB1  PB0 
  //           led4 led3 led2 led1
  //
  // numero sequencial - n - 0 a 15
  //  0   0    0    0  bit4 bit3 bit2 bit1
  //
  // rotacionando n a esquerda 2x (n = n<<2)
  //  0   0  bit4  bit3 bit2 bit1 0 0
  // 
  // os bits pb7, pb6, pb1 e pb0 devem permanecer inalterados - (mascarados)
  //
  // mask
  // 1   1   0   0   0   0   1   1 - 0xE3
  //
  // PORTB = PORTB & B11000011 - mascara os bits
  // PORTB = PORTB | (n<<2) - aciona os bits de B

  while(1){ //equivalente em C ao void loop()
    
    for (byte n=0;n<16;n++){//Conta de 0 a 15
      
      PORTB = PORTB & B11000011; //mascara os bits
      PORTB = PORTB | ~(n<<2);   //aciona a saida zerando os bits referentes ao numero n rotacionado em 2 casas
      
      delay(1000);
      Serial.println(int(n), DEC);  
    }
  }
}

void loop() {
  //nao utilizado - necessario na IDE do Arduino 
}

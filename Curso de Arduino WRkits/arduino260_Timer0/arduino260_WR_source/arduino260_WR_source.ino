/* ============================================================

   Curso de Arduino
   
   Timer0 - Configuração em Modo Comparação

   Compilador: Arduino IDE 1.8.4

   MCU: Atmega328p
   Autor: Eng. Wagner Rambo
   Data:  Abril de 2020


============================================================ */


// ============================================================
// --- Mapeamento de Hardware ---
#define   pulse (1<<PD3)
#define   bt1   (1<<PD4)
#define   bt2   (1<<PD5)
#define   bt3   (1<<PD6)
#define   bt4   (1<<PD7)
#define   led1  (1<<PB0)
#define   led2  (1<<PB1)


// ============================================================
// --- Protótipo das Funções ---
void readKeyboard();


// ============================================================
// --- Variáveis Globais ---
unsigned char bt_press = 0;


// ============================================================
// --- Interrupções ---
ISR (TIMER0_COMPA_vect)       //vetor para interrupção por overflow do Timer0
{                     
   PORTB ^= led1;
   PORTB ^= led2;
   
    
} //end ISR


// ============================================================
// --- Programa Principal ---
void setup()
{
   cli();              //desliga interrupção global
  
   DDRD  &=  ~bt1;     //configura entrada para o botão 1
   DDRD  &=  ~bt2;     //configura entrada para o botão 2
   DDRD  &=  ~bt3;     //configura entrada para o botão 3
   DDRD  &=  ~bt4;     //configura entrada para o botão 4
   
   PORTD |=   bt1;     //habilita o pull-up interno para o botão 1
   PORTD |=   bt2;     //habilita o pull-up interno para o botão 2
   PORTD |=   bt3;     //habilita o pull-up interno para o botão 3
   PORTD |=   bt4;     //habilita o pull-up interno para o botão 4
   
   DDRD  |=   pulse;   //configura saída para o pulso de interrupção externa
   DDRB  |=   led1;    //configura saída para o led1
   DDRB  |=   led2;    //configura saída para o led2

   PORTD &= ~pulse;    //inicializa pulse em LOW
 

   TCCR0A  =  0x00;    //timer0 operando em modo normal
   TCCR0B  =  0x00;    //timer0 normal desligado
   TCNT0   =  0x00;    //inicializa timer0
  
                  //   f = fclk/(2xPSx(1+OCR0A))
                  
   OCR0A   = 243;       //configura valor de comparação
 
   TCCR0A |= (1 << WGM01);   //habilita o modo CTC
 
   TCCR0B |= (1 << CS01) | (1 << CS00); //prescaler 1:64   
  
   TIMSK0 |= (1 << OCIE0A);  //habilita interrupção por comparação
   
   sei();                                    //liga interrupção global


   while(1)
   {
      readKeyboard();
    
 
   } //end while
    
} //end setup


// ============================================================
// --- Desenvolvimento das Funções ---
void readKeyboard()
{

static boolean  bt1_f = 0x00,
                bt2_f = 0x00,
                bt3_f = 0x00,
                bt4_f = 0x00;

  
   if(!(PIND&bt1)) bt1_f = 0x01;
   if(!(PIND&bt2)) bt2_f = 0x01;
   if(!(PIND&bt3)) bt3_f = 0x01;
   if(!(PIND&bt4)) bt4_f = 0x01;

   if((PIND&bt1) && bt1_f)
   {
      bt1_f = 0x00;
      PORTD |= pulse;
      bt_press = 1;
      PORTD &= ~pulse;
        
   } //end if bt1  

   if((PIND&bt2) && bt2_f)
   {
      bt2_f = 0x00;
      PORTD |= pulse;
      bt_press = 2;
      PORTD &= ~pulse;
        
   } //end if bt2  

   if((PIND&bt3) && bt3_f)
   {
      bt3_f = 0x00;
      PORTD |= pulse;
      bt_press = 3;
      PORTD &= ~pulse;
        
   } //end if bt3 

   if((PIND&bt4) && bt4_f)
   {
      bt4_f = 0x00;
      PORTD |= pulse;
      bt_press = 4;
      PORTD &= ~pulse;
        
   } //end if bt4
  
  
} //end readKeyboard
  

// ============================================================
// --- FINAL DO PROGRAMA ---


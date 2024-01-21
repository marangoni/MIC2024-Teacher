/* ============================================================

   Curso de Arduino
   
   Função Avançada para Controle de Botão

   Efetua uma função ao pressionar e soltar
   Efetua outra função ao pressionar e manter por alguns segundos

   Compilador: Arduino IDE 1.8.4

   MCU: Atmega328p
   Autor: Eng. Wagner Rambo
   Data:  Junho de 2020


============================================================ */


// ============================================================
// --- Mapeamento de Hardware ---
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
unsigned char bt_press  = 0,
              ct2       = 0,
              timer_bt1 = 0;

boolean       func_bt1  = 0x00;


// ============================================================
// --- Interrupções ---
ISR (TIMER2_OVF_vect)       //vetor para interrupção por overflow do Timer0
{      
   TCNT2   =    56;    //inicializa timer2    
   //base de tempo 12,8ms

   ct2+=1;

   if(ct2 == 78)
   {
     //aprox. 1seg

     ct2 = 0;

     if(func_bt1) timer_bt1+=1;

     if(timer_bt1 == 3)
     {
        timer_bt1 = 0; 
        PORTB |= led1;
      
     } //end if timer_bt1
          
     
   } //end if ct2
    
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
    
   DDRB  |=   led1;    //configura saída para o led1
   DDRB  |=   led2;    //configura saída para o led2

   PORTB &=  ~led1;    //led1 inicia desligado
   PORTB &=  ~led2;    //led2 inicia desligado
 
 

   TCCR2A  =  0x00;    //timer2 operando em modo normal
   TCCR2B  =  0x07;    //timer2 normal e prescaler 1:1024
   TCNT2   =    56;    //inicializa timer2
   TIMSK2  =  0x01;    //liga interrupção do timer2
   
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

  
   if(!(PIND&bt1)) 
   {
      func_bt1 = 0x01;
      bt1_f    = 0x01;
   
   } //end if bt1 press
   
   if(!(PIND&bt2)) bt2_f = 0x01;
   if(!(PIND&bt3)) bt3_f = 0x01;
   if(!(PIND&bt4)) bt4_f = 0x01;

   if((PIND&bt1) && bt1_f)
   {
      bt1_f    = 0x00;
      func_bt1 = 0x00;
      timer_bt1 = 0x00;
    
      PORTB |= led2;
      
       
   } //end if bt1 release



   if((PIND&bt2) && bt2_f)
   {
      bt2_f = 0x00;
       
        
   } //end if bt2  

   if((PIND&bt3) && bt3_f)
   {
      bt3_f = 0x00;
     
        
   } //end if bt3 

   if((PIND&bt4) && bt4_f)
   {
      bt4_f = 0x00;
       
        
   } //end if bt4
  
  
} //end readKeyboard
  

// ============================================================
// --- FINAL DO PROGRAMA ---


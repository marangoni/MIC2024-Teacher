/* ============================================================

   Curso Microcontroladores - IFSP - Engenharia de Controle e Automação
   
   Timer0 - Configuração em Modo Comparação

   Compilador: Arduino IDE 2.2.1

   MCU: Atmega328p
   Autor: D.Eng. Luiz Marangoni
   Data:  Janeiro de 2024


============================================================ */


// ============================================================
// --- Mapeamento de Hardware ---
#define   pulse (1<<PD3)
#define   bt1   (1<<PD4) //botao1 no pino 6 (PD4)
#define   led1  (1<<PB0) //led1 no pino 14(PB0)


// ============================================================
// --- Protótipo das Funções ---
void control();
void timer_2s();
void led_1();


// ============================================================
// --- Variáveis Globais ---
unsigned char bt_press = 0;

unsigned char timer_2s_start = 0;
unsigned char timer_2s_pause = 0;
unsigned char timer_2s_stop = 0; 

unsigned char led_1_on = 0;
unsigned char led_1_off = 1;



// ============================================================
// --- Interrupções ---


ISR (TIMER2_OVF_vect)       
                      // TIC - clock da máquina de estados 
                      // vetor para interrupção por overflow do Timer2
{      
   TCNT2   =    56;    //inicializa timer2    
   //aprox 12ms

   ct2+=1;

   if(ct2 == 78)
   {
     //aprox. 1seg

     ct2 = 0;
     PORTB ^= led1;
     PORTB ^= led2;
   }
    
} //end ISR


// ============================================================
// --- Programa Principal ---
void setup()
{
   cli();              //desliga interrupção global
  
   DDRD  &=  ~bt1;     //configura entrada para o botão 1
                       //DDR = DDR & bt1 - Zera o bit bt1
   
   PORTD |=   bt1;     //habilita o pull-up interno para o botão 1
                       //PORTD = PORTD | bt1 - Seta o bit 
      
   DDRD  |=   pulse;   //configura saída para o pulso de interrupção externa
   DDRB  |=   led1;    //configura saída para o led1
   

   PORTD &= ~pulse;    //inicializa pulse em LOW
 

   TCCR2A  =  0x00;    //timer2 operando em modo normal
   TCCR2B  =  0x07;    //timer2 normal e prescaler
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

static boolean  bt1_f = 0x00;
              
   if(!(PIND&bt1)) bt1_f = 0x01;
   
   if((PIND&bt1) && bt1_f)
   {
      bt1_f = 0x00;
      PORTD |= pulse;
      bt_press = 1;
      PORTD &= ~pulse;
        
   } //end if bt1  
  
} //end readKeyboard
  

// ============================================================
// --- FINAL DO PROGRAMA ---


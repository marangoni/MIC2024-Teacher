#include <LiquidCrystal.h>
  //LiquidCrystal lcd(RS, E, D4, D5, D6, D7);   //declaration of LCD wiring function
    LiquidCrystal lcd (5, A0, A1, A2, A3, A4);      //definition of LCD wiring function

  void setup()
    {
      //Task of L11 is done by declaring ISR(INT0_vect) function at the end of loop() function
      
      lcd.begin(16, 2);         //L12: 16x2 LCD ; 2-line; each line has 
      lcd.setCursor(1, 0);        //L12: DP1 postion of TopLine
    
//----- the following single function does all tasks of----------------------------
      attachInterrupt(digitalPinToInterrupt(2), ISRINT0, FALLING);  //L13-L16     
    // interrupts();
//---------------------------------------------------
pinMode(13, OUTPUT);    //L17:
pinMode(4, OUTPUT);     //L18:

lcd.print(EIMSK, 16);     //check INT0-bit for 1 before the MLP is interrupted
lcd.setCursor(5, 0);        //
lcd.print(SREG, 16);      // check I-bit for 1 before MLP is interrupted

  }

    void loop()
{
      digitalWrite(13, HIGH);   //L21:
      delay(1000);          //L22:
      digitalWrite(13, LOW);    //L23:
      delay(1000);          //L24
  }

void  ISRINT0()
{
    
 //   EIMSK = 0x00;
    lcd.setCursor(1, 1);      //cursor at DP1 bottomLine
    lcd.print(EIMSK, 16);     //check INT0-bit for 1 after interruption
    lcd.setCursor(5, 1);        
    lcd.print(SREG, 16);      //check I-bit for 0 after interruption
    //--------------------------------------------------
interrupts();            //L31: Set I-bit of SREG-register; global I-switch is closed
 //   lcd.setCursor (9, 1);
  //  lcd.print(EIMSK, 16);
    
    for (int n=0; n<=4; n++)
    {
      digitalWrite(4, HIGH);      //L32:
      delay(1000);          //L33:
      digitalWrite(4, LOW);     //L34:
      delay(1000);          //L35;  
    }
    reti();                 // must be here for the return to work
  }
  

 


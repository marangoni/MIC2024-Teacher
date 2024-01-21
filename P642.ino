#include <LiquidCrystal.h>
  //LiquidCrystal lcd(RS, E, D4, D5, D6, D7);   //declaration of LCD wiring function
    LiquidCrystal lcd (5, A0, A1, A2, A3, A4);      //definition of LCD wiring function

  void setup()
    {
      //Task of L11 is done by declaring ISRINT0(INT0_vect) function at the end of loop() function
      
      lcd.begin(16, 2);       //L12: 16x2 LCD ; 2-line; each line has 
      lcd.setCursor(1, 0);      //L12: DP1 postion of TopLine
    
//----- the following six lines connects internal pull-up with PD2-line------------------
      DDRD = B11111011;     //L13:  PD2 as input; value has come from data sheet
      bitSet(PORTD, 2);       //L13: 
          bitClear(MCUCR, 4);     //L13; PU-bit is 0
      
//---------------------------------------------------
      EICRA = B00000010;      //L13: falling edge trigger level for of INT0; value comes from data sheet
      EIMSK = B00000001;      //L14: local INT0-switch is closed; value has come from data sheet
      bitSet(SREG, 7);        //L15: Global I-switch is closed; alternate code : sei() ; opposite : cli()
      
//---------------------------------------------------
pinMode(13, OUTPUT);    //L16:
pinMode(4, OUTPUT);     //L17:

lcd.print(EIMSK, 16);   //check for INT0-bit for 1 before interruption
lcd.setCursor(5, 0);
lcd.print(SREG, 16);    //chcek I-bit for 1before interruption

  }

    void loop()
{
      digitalWrite(13, HIGH);   //L21:
      delay(1000);          //L22:
      digitalWrite(13, LOW);    //L23:
      delay(1000);          //L24
  }

  ISR(INT0_vect)
{
    lcd.setCursor (1, 1);
    lcd.print(EIMSK, 16);   //check for INT0-bit for 1 after interruption
lcd.setCursor(5, 1);
lcd.print(SREG, 16);    //chcek I-bit for 0 after interruption
    sei();            //L31: Set I-bit of SREG-register; global I-switch is closed
    
    for (int n=0; n<=4; n++)
    {
      digitalWrite(4, HIGH);      //L32:
      delay(1000);          //L33:
      digitalWrite(4, LOW);     //L34:
      delay(1000);          //L35;  
    }

      //--------------------------------------
      reti();           //L37: Return to MLP from ISRINT0
}




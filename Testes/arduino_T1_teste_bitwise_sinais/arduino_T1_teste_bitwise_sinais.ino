#define msg_T1_start 0
#define msg_T1_done  1
#define msg_LED1_cmd 2

int reg_sinais = 0xA0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("reg_sinais: ");
  Serial.println(reg_sinais, BIN);

  reg_sinais |= (1<<msg_T1_start); 

  Serial.print("reg_sinais: ");
  Serial.println(reg_sinais, BIN);  
  
  if (reg_sinais & (1<<msg_T1_start) == (1<<msg_T1_start))
  {
    Serial.println("Sinal msg_T1_start recebido");
    
    reg_sinais &= ~(1<<msg_T1_start);   
    
    Serial.println(reg_sinais, BIN);
    Serial.println("Sinal msg_T1_start consumido");
  }


  

}

void loop() {
  // put your main code here, to run repeatedly:

}

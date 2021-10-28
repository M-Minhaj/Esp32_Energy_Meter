// AC Voltmeter
// This is a free software with NO WARRANTY.
// USE AT YOUR OWN RISK!
 

#include <EEPROM.h>
#include <Arduino.h>  
#include <U8g2lib.h>

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


#define CT 35
#define VT 34












 


float I,U,ret_zero;

float R1 = 130000;

float R2 = 576;



float Vin_rms,Vout_rms;

float I_rms;

float kW;

char buffer_vrms[10];
char buffer_irms[10];
char buffer_kW[10];


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);






void setup(void) 
{
 
 u8g2.begin();
 Serial.begin(115200);

 
 u8g2.begin(/*Select=*/ 18, /*Right/Next=*/ 5, /*Left/Prev=*/ 17, /*Up=*/ U8X8_PIN_NONE, /*Down=*/ U8X8_PIN_NONE, /*Home/Cancel=*/ 19); 
 u8g2.enableUTF8Print();
 u8g2.setFont(u8g2_font_helvB12_tr);
 u8g2.clear();

 emon1.voltage(34, 0.97933, 1);  // Voltage: input pin, calibration, phase_shift
 emon1.current(35, 1);       // Current: input pin, calibration.

 //digitalWrite(5,HIGH); // Relay
 //delay(1500);
 pinMode(5, OUTPUT);
 pinMode(18, OUTPUT); 


 



 
  
 
 


  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
  u8g2.drawStr(15,25,"Power");  // write something to the internal memory
  u8g2.drawStr(15,50,"Solution");
  u8g2.sendBuffer();          // transfer internal memory to the display

  for (int j = 0; j <= 3220; j++) 
  
   {
    digitalWrite(18,HIGH);
    delayMicroseconds(244);

    digitalWrite(18,LOW);
    delayMicroseconds(244);
   }


  delay(5000);

  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
  u8g2.drawStr(10,35,"Loading...");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(5000);


  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
  u8g2.drawStr(10,35,"Starting...");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(5000);

  
 
digitalWrite(5,HIGH); // Relay


 

}
 






void loop() 
{


     
  
   emon1.calcVI(40,3000);         // Calculate all. No.of half wavelengths (crossings), time-out
   emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
   float realPower       = emon1.realPower;        //extract Real Power into variable
   float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
   float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
   float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
   float Irms            = emon1.Irms;             //extract Irms into Variable    

   U = emon1.Vrms;
   
   Vin_rms = ( U * R1 ) / R2;

   Vin_rms = Vin_rms  ;

   
   I = emon1.Irms;
   
   
   I_rms = I  / 33.0;

   I_rms = I_rms * 1000.0  ;



  kW= (I_rms * Vin_rms)/1000.0;

 
  Serial.println("Voltage(V)");
  Serial.println(Vin_rms,1);
  
  Serial.println("Current(A)");
  Serial.println(I_rms,1);

  Serial.println("Power(kW)");
  Serial.println(kW,4);



  sprintf(buffer_vrms, "%-5.1f",Vin_rms);
  sprintf(buffer_irms, "%-4.2f",I_rms);
  sprintf(buffer_kW, "%-4.2f",kW);


  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
  
  u8g2.drawStr(2,11,"Volts(V):");  // write something to the internal memory
  u8g2.drawStr(80,11,buffer_vrms);  // write something to the internal memory
 
  u8g2.drawStr(2,27,"Amp(I):");  // write something to the internal memory
  u8g2.drawStr(80,27,buffer_irms);  // write something to the internal memory
 
  u8g2.drawStr(2,43,"PWR(kW):");  // write something to the internal memory
  u8g2.drawStr(80,43,buffer_kW);  // write something to the internal memory


  u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font

  u8g2.drawStr(2,60,"Status:");
 
 // Vin_rms = 200;
  
  if( Vin_rms > 250 || Vin_rms < 170 )
    {
    
     digitalWrite(5,LOW);
     
     u8g2.drawStr(60,60,"Cut Off");
 
     
    }
   else if (Vin_rms < 245  || Vin_rms > 175)
    {
     digitalWrite(5,HIGH);
     u8g2.drawStr(60,60,"Normal");
 
     
    }

 //I_rms=16;


if( I_rms > 15.0  )
{
  digitalWrite(5,LOW); 
  
  for (int i = 0; i <= 20; i++) 
  
  {
   
//  digitalWrite(18,HIGH);
   
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB18_tr); // choose a suitable font
  u8g2.clear();
  
 // u8g2.drawStr(25,25,"HIGH");  // write something to the internal memory
  
 // u8g2.setFont(u8g2_font_ncenB10_tr);
  
 // u8g2.drawStr(25,55,"LOAD");  // write something to the internal memory
  
   
 
  //u8g2.sendBuffer();          // transfer internal memory to the display

  do{
        u8g2.setCursor(25, 25);
        u8g2.print("High");
        
        u8g2.setCursor(25, 55);
        u8g2.print("Load");
  
  //      u8g2.clear();
  
       } 
     while (u8g2.nextPage());

   for (int j = 0; j <= 1000; j++) 
  
    {
     digitalWrite(18,HIGH);
     delayMicroseconds(244);

     digitalWrite(18,LOW);
     delayMicroseconds(244);
    } 
 



       
     delay(500);
     u8g2.clear();
    do{
        u8g2.setCursor(10, 25);
        u8g2.print("Reduce");
        
        u8g2.setCursor(25, 55);
        u8g2.print("Load");
  
  //      u8g2.clear();
  
       } 
     while (u8g2.nextPage());  
     delay(500);
     u8g2.clear();
  
  
  
  
  
  
  
   
  }


}

       


  u8g2.sendBuffer();          // transfer internal memory to the display





  
//  delay(1000);  
}
      

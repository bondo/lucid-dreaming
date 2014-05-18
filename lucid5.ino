// Source: http://www.instructables.com/id/15-Dream-to-Reality-interfacer-control-objects-i/

#include <EEPROM.h>
#include <avr/sleep.h>

boolean startTime=false;
volatile int sleep_count = 0;
int interval= 1;   //minutes
unsigned long sleep_total = (((float)interval*60)/8.1714);
int led1 = 3;
int led2 = 5;
int led3 = 6;
int led4 = 9;
int led5 = 10;
int led6 = 11;
const int button1  = 4;
const int buttonPad = 2;
boolean initialize =0;
int buttonState = 0;
int buttonStatePad = 1;
unsigned long timer1;
unsigned long timer2;
unsigned long currentTime;
unsigned long previousTime;
boolean TC1 = 0;
boolean TC2 = 0;
boolean WC1 = 0;
boolean RC1 = 0;
boolean repeat = 1;
boolean wipeCheck = 0;
boolean startMenu =0;
int menuItem = 0;
byte brightness;
byte patternSpeed;
byte pattern;
byte waitTime;
byte triggersPerNight;
byte patternsPerTrigger;
unsigned long lastFadeTime = 0;
boolean patternFinished = false;
int patternPlayed=0;
int triggersPlayed = 0;
boolean fadeupA = 1;
boolean fadeupB = 0;
boolean fadeupC = 0;
boolean fadeupD = 0;
boolean fadeupE = 0;
boolean fadeupF = 0;
int fadeTime = 100;
int ledFadeA = 0;
int ledFadeB = 0;
int ledFadeC = 0;
int ledFadeD = 0;
int ledFadeE = 0;
int ledFadeF = 0;
boolean patternStartBit = false;
int sensorValue;
float numTriggers = 0;
float triggerDefault;
boolean TC4 = false;
boolean TC3 =  false;
long timer3;
long timer4;
int X1= 0;
boolean sensorInitialized = 0;
boolean sensorDefault = 1;
int triggerState1;
int triggerState2;
int triggerState3;

//________________________________________________________________________________

void sleepNow(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0,wakeUpNow, LOW);
  sleep_mode();             //program sleeps here
  sleep_disable();         //program resumes from here first
  detachInterrupt(0);     //disables interupt so it wont be run during normal runtime
}

void watchdogOn() {
  
 MCUSR = MCUSR & B11110111;                         // Clear the reset flag, the WDRF bit (bit 3) of MCUSR.  
                                                    // Set the WDCE bit (bit 4) and the WDE bit (bit 3) 
                                                    // of WDTCSR. The WDCE bit must be set in order to 
                                                    // change WDE or the watchdog prescalers. Setting the 
                                                    // WDCE bit will allow updtaes to the prescalers and 
                                                    // WDE for 4 clock cycles then it will be reset by 
                                                    // hardware.
 WDTCSR = WDTCSR | B00011000; 

                                                    // Set the watchdog timeout prescaler value to 1024 K 
 WDTCSR = B00100001;                                // which will yeild a time-out interval of about 8.0 s.


                                                    // Enable the watchdog timer interupt.
 WDTCSR = WDTCSR | B01000000;
 MCUSR = MCUSR & B11110111;

 }
 ISR(WDT_vect)
 {
 sleep_count ++;                                    // keep track of how many sleep cycles have been completed.
 }
void startMenuPattern(){
  digitalWrite(led1, HIGH);   
  delay(100);               
  digitalWrite(led2, HIGH);    
  delay(100); 
  digitalWrite(led1, LOW);
  digitalWrite(led3, HIGH);
  delay(100);
  digitalWrite(led2, LOW);
  digitalWrite(led4, HIGH);
  delay(100); 
  digitalWrite(led3, LOW);
  digitalWrite(led5, HIGH);
  delay(100);
  digitalWrite(led4, LOW);
  digitalWrite(led6, HIGH);
  delay(100); 
  digitalWrite(led5, LOW);
  delay(100);
  digitalWrite(led6, LOW);
  delay(200);
  
  
}

void registerHold(){
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
digitalWrite(led1, HIGH);   
  delay(50);               
digitalWrite(led1, LOW);   
  delay(50);  
digitalWrite(led1, HIGH);   
  delay(50);               
digitalWrite(led1, LOW);

menuItem++;
if (menuItem ==6){
  menuItem=0;
}
if (menuItem ==0){
  digitalWrite(led1, HIGH);   
  delay(500);               
  digitalWrite(led1, LOW);     
}
if (menuItem ==1){
  digitalWrite(led2, HIGH);   
  delay(500);               
  digitalWrite(led2, LOW);     
} 
if (menuItem ==2){
  digitalWrite(led3, HIGH);   
  delay(500);               
  digitalWrite(led3, LOW);     
} 
if (menuItem ==3){
  digitalWrite(led4, HIGH);   
  delay(500);               
  digitalWrite(led4, LOW);     
} 
if (menuItem ==4){
  digitalWrite(led5, HIGH);   
  delay(500);               
  digitalWrite(led5, LOW);     
} 
if (menuItem ==5){
  digitalWrite(led6, HIGH);   
  delay(500);               
  digitalWrite(led6, LOW);     
}
}

void registerTap(){
  digitalWrite(led1, HIGH);   
  delay(25);               
digitalWrite(led1, LOW);   
  delay(25);  
digitalWrite(led1, HIGH);   
  delay(25);               
digitalWrite(led1, LOW);

if(menuItem==0){
  if (brightness > 27 ){
      brightness=8;
      wipeCheck = 1;
      
  }
  else if (brightness < 27 ){
      brightness = brightness + 4;
  } 
}
if(menuItem==1){
  if (patternSpeed > 5 ){
      patternSpeed=1;
      wipeCheck = 1;
      
  }
  else if (patternSpeed < 6 ){
      patternSpeed = patternSpeed + 1;
  } 
}
if(menuItem==2){
  if (pattern > 7 ){
      pattern=1;
      wipeCheck = 1;
      repeat = 1;
      
    }
  else if (pattern < 8 ){
      pattern++;
      wipeCheck = 1;
      repeat = 1;
  } 
 }
if(menuItem==3){
  if (waitTime > 5 ){
      waitTime=1;
      wipeCheck = 1;
      
  }
  else if (waitTime < 6 ){
      waitTime = waitTime + 1;
  } 
}
if(menuItem==4){
  if (triggersPerNight > 5 ){
      triggersPerNight=1;
      wipeCheck = 1;
      
  }
  else if (triggersPerNight < 6 ){
      triggersPerNight = triggersPerNight + 1;
  } 
}
if(menuItem==5){
  if (patternsPerTrigger > 5 ){
      patternsPerTrigger=1;
      wipeCheck = 1;
      
  }
  else if (patternsPerTrigger < 6 ){
      patternsPerTrigger = patternsPerTrigger + 1;
  } 
}

}

void ledWipe(){
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
}
  
void displayBrightness(){
    if (brightness==8){
    digitalWrite(led1, HIGH);
  }
  if (brightness==12){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  if (brightness==16){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  }
  if (brightness==20){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }
  if (brightness==24){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (brightness==28){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
}
void displaySpeed(){
  if (patternSpeed==1){
    digitalWrite(led1, HIGH);
  }
  if (patternSpeed==2){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  if (patternSpeed==3){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  }
  if (patternSpeed==4){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }
  if (patternSpeed==5){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (patternSpeed==6){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
}
void displayPattern(){
  if (pattern==1){
    
    if (repeat == 1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
    fadeupA = 1;
    fadeupB = 0;
    fadeupC = 0;
    fadeupD = 0;
    fadeupE = 0;
    fadeupF = 0;
    repeat = 0;
    }
    pattern1();
  }
  if (pattern==2){
    
    if (repeat == 1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
    fadeupA = 1;
    fadeupB = 0;
    fadeupC = 0;
    fadeupD = 0;
    fadeupE = 0;
    fadeupF = 0;
    repeat = 0;
    }
    pattern2();
  }
  if (pattern==3){
    
    if (repeat ==1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
      fadeupA = 1;
      fadeupB = 1;
      fadeupC = 1;
      fadeupD = 1;
      fadeupE = 1;
      fadeupF = 1;
      repeat = 0;
    }
    pattern3();
  }
  if (pattern==4){
    if (repeat ==1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
      fadeupA = 1;
      fadeupB = 0;
      fadeupC = 1;
      fadeupD = 1;
      fadeupE = 0;
      fadeupF = 1;
      repeat = 0;
    }
    pattern4();
  }
  if (pattern==5){
    if (repeat ==1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
      fadeupA = 0;
      fadeupB = 1;
      fadeupC = 0;
      fadeupD = 0;
      fadeupE = 1;
      fadeupF = 0;
      repeat = 0;
    }
    pattern5();
  }
  if (pattern==6){
    if (repeat ==1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
      fadeupA = 1;
      fadeupB = 0;
      fadeupC = 0;
      fadeupD = 1;
      fadeupE = 0;
      fadeupF = 0;
      repeat = 0;
    }
    pattern6();
  }
  if (pattern==7){
    if (repeat ==1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
      fadeupA = 0;
      fadeupB = 0;
      fadeupC = 1;
      fadeupD = 0;
      fadeupE = 0;
      fadeupF = 1;
      repeat = 0;
    }
    pattern7();
  }
  if (pattern==8){
    if (repeat ==1){
      ledFadeA = 0;
      ledFadeB = 0;
      ledFadeC = 0;
      ledFadeD = 0;
      ledFadeE = 0;
      ledFadeF = 0;
      fadeupA = 1;
      fadeupB = 0;
      fadeupC = 0;
      fadeupD = 0;
      fadeupE = 0;
      fadeupF = 0;
      repeat = 0;
    }
    pattern8();
  }
}
void displayWaitTime(){
  if (waitTime==1){
    digitalWrite(led1, HIGH);
  }
  if (waitTime==2){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  if (waitTime==3){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  }
  if (waitTime==4){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }
  if (waitTime==5){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (waitTime==6){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
}

void displayTPN(){
  if (triggersPerNight==1){
    digitalWrite(led1, HIGH);
  }
  if (triggersPerNight==2){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  if (triggersPerNight==3){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  }
  if (triggersPerNight==4){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }
  if (triggersPerNight==5){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (triggersPerNight==6){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
}

void displayPPT(){
  if (patternsPerTrigger==1){
    digitalWrite(led1, HIGH);
  }
  if (patternsPerTrigger==2){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  if (patternsPerTrigger==3){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
  }
  if (patternsPerTrigger==4){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }
  if (patternsPerTrigger==5){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (patternsPerTrigger==6){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
  }
}

//_____________________________________________________________________________________
void patternWipe(){
  lastFadeTime = millis();
  fadeupA = 0;
  fadeupB = 0;
  fadeupC = 0;
  fadeupD = 0;
  fadeupE = 0;
  fadeupF = 0;
}



void pattern1(){
  
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led1, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
           fadeupB = 1;                                 // turn on ledb
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led1, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;                                    //dont repeat        
     }} 
     if(fadeupB == 1) {                                 
        analogWrite(led2, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
           fadeupC = 1;                                      
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led2, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;                              
     }}   
     if(fadeupC ==1) {                                 
        analogWrite(led3, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
           fadeupD = 1;                                        
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led3, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }} 
     if(fadeupD == 1) {                                  
         analogWrite(led4, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;                            
           fadeupE = 1;                                       
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led4, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led5, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
           fadeupF = 1;                                       
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led5, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led6, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
           
              
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led6, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0;
        }
        }  
     lastFadeTime = millis();                       
   } 
}



void pattern2(){
  
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led6, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
           fadeupB = 1;                                 // turn on ledb
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led6, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;                                    //dont repeat        
     }} 
     if(fadeupB == 1) {                                 
        analogWrite(led5, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
           fadeupC = 1;                                      
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led5, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;                              
     }}   
     if(fadeupC ==1) {                                 
        analogWrite(led4, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
           fadeupD = 1;                                        
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led4, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }} 
     if(fadeupD == 1) {                                  
         analogWrite(led3, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;                            
           fadeupE = 1;                                       
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led3, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led2, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
           fadeupF = 1;                                       
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led2, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led1, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
           
                      
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led1, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0;
        }
            }  
     lastFadeTime = millis();                       
   } 
}


void pattern3(){
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led1, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led1, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;         //dont repeat
       } 
            }
     if(fadeupB == 1) {                                 
        analogWrite(led2, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led2, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;                              
     }}   
     if(fadeupC ==1) {                                 
        analogWrite(led3, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led3, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }} 
     if(fadeupD == 1) {                                  
         analogWrite(led4, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;                            
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led4, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led5, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led5, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led6, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led6, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0;  
     }}  
     lastFadeTime = millis();                       
   } 
}


void pattern4(){
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led1, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
           fadeupB = 1;                                 // turn on ledb
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led1, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;                                    //dont repeat        
     }} 
     if(fadeupB == 1) {                                 
        analogWrite(led2, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
                                                 
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led2, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;
          }
          }   
     if(fadeupC ==1) {                                 
        analogWrite(led3, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
                                                   
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led3, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }} 
     if(fadeupD == 1) {                                  
         analogWrite(led4, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;                            
           fadeupE = 1;                                       
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led4, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led5, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
                                                  
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led5, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led6, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
                 
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led6, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0;
     }}  
     lastFadeTime = millis();                       
   }
}

void pattern5(){
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led1, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
                                            // turn on ledb
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led1, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;                                    //dont repeat        
     }
        } 
     if(fadeupB == 1) {                                 
        analogWrite(led2, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
           fadeupA = 1;
           fadeupC = 1;           
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led2, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;                              
     }}   
     if(fadeupC ==1) {                                 
        analogWrite(led3, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
                                                   
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led3, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }} 
     if(fadeupD == 1) {                                  
         analogWrite(led4, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;                            
                                                  
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led4, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led5, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
           fadeupD = 1;
           fadeupF = 1;           
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led5, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led6, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
                
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led6, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0; 
        }}  
     lastFadeTime = millis();                       
   } 
}

void pattern6(){
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led1, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
           fadeupB = 1;                                 // turn on ledb
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led1, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;                                    //dont repeat        
     }} 
     if(fadeupB == 1) {                                 
        analogWrite(led2, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
           fadeupC = 1;                                      
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led2, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;                              
     }}   
     if(fadeupC ==1) {                                 
        analogWrite(led3, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
                                                   
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led3, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }
        } 
     if(fadeupD == 1) {                                  
         analogWrite(led4, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;                            
           fadeupE = 1;                                       
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led4, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led5, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
           fadeupF = 1;                                       
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led5, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led6, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
              
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led6, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0;
                
     }}  
     lastFadeTime = millis();                       
   } 
}

void pattern7(){
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led1, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
                                            // turn on ledb
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led1, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;                                    //dont repeat        
     }
        } 
     if(fadeupB == 1) {                                 
        analogWrite(led2, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
           fadeupA = 1;                                      
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led2, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;                              
     }}   
     if(fadeupC ==1) {                                 
        analogWrite(led3, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
           fadeupB = 1;                                        
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led3, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }} 
     if(fadeupD == 1) {                                  
         analogWrite(led4, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;                            
                                                  
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led4, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led5, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
           fadeupD = 1;                                       
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led5, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led6, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
           fadeupE = 1;
              
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led6, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0;
                
     }}  
     lastFadeTime = millis();                       
   } 
}
  

void pattern8(){
  if(millis() - lastFadeTime > fadeTime) {             // if it has been x miliseconds    
     if(fadeupA == 1) {                                 // if a is fading up
       analogWrite(led1, ledFadeA);                     // keep fading up
       ledFadeA += ((brightness/8)*patternSpeed);
         if(ledFadeA >= brightness) {                   // unitl ledfade = 42           
           fadeupA = 0;                                 //now fade down
           fadeupF = 1;                                 // turn on ledb
     }}        
     if(fadeupA == 0) {                                 //if fading down leda
       analogWrite(led1, ledFadeA);                     //keep fading down
       ledFadeA -= ((brightness/8)*patternSpeed);               
       if(ledFadeA <= 0) {                              // until 0
         ledFadeA = 0;                                    //dont repeat        
     }} 
     if(fadeupB == 1) {                                 
        analogWrite(led2, ledFadeB);                 
        ledFadeB += ((brightness/8)*patternSpeed);
        if(ledFadeB >= brightness) {                          
           fadeupB = 0;                            
           fadeupE = 1;                                      
     }}
     if (fadeupB == 0) {                                    
          analogWrite(led2, ledFadeB);      
          ledFadeB -= ((brightness/8)*patternSpeed);               
          if(ledFadeB <= 0) {                          
             ledFadeB = 0;                              
     }}   
     if(fadeupC ==1) {                                 
        analogWrite(led3, ledFadeC);                  
        ledFadeC += ((brightness/8)*patternSpeed);
        if(ledFadeC >= brightness) {                          
           fadeupC = 0;                           
                                                   
     }} 
     if(fadeupC ==0) {                                      
         analogWrite(led3, ledFadeC);     
         ledFadeC -= ((brightness/8)*patternSpeed); 
         if(ledFadeC <= 0) {                          
            ledFadeC = 0;                               
     }
       } 
     if(fadeupD == 1) {                                  
         analogWrite(led4, ledFadeD);                  
         ledFadeD += ((brightness/8)*patternSpeed);
         if(ledFadeD >= brightness) {
           fadeupD = 0;
           fadeupB = 1;           
                                                  
     }}
     if(fadeupD == 0) {                                      
         analogWrite(led4, ledFadeD);                 
         ledFadeD -= ((brightness/8)*patternSpeed);
         if(ledFadeD <= 0) {                          
         ledFadeD = 0;                               
     }}
   
     if(fadeupE ==1) {                                  
        analogWrite(led5, ledFadeE);                  
        ledFadeE += ((brightness/8)*patternSpeed);
        if(ledFadeE >= brightness) {                          
           fadeupE = 0;                           
           fadeupC = 1;                                       
     }} 
     if(fadeupE ==0){                                      
        analogWrite(led5, ledFadeE);                 
        ledFadeE -= ((brightness/8)*patternSpeed); 
        if(ledFadeE <= 0) {                           
          ledFadeE = 0;                               
     }}
     if(fadeupF ==1) {                                  
        analogWrite(led6, ledFadeF);                  
        ledFadeF += ((brightness/8)*patternSpeed);
        if(ledFadeF >= brightness) {                          
           fadeupF = 0;
           fadeupD = 1;        
     }}
     if(fadeupF ==0) {                                      
        analogWrite(led6, ledFadeF);                  
        ledFadeF -= ((brightness/8)*patternSpeed);
        if(ledFadeF <= 0) {                           
        ledFadeF = 0;
                
     }}  
     lastFadeTime = millis();                       
   } 
}

void patternStart(){
  displayPattern();
    
    if (ledFadeA==0){
      if(ledFadeB==0){
        if (ledFadeC==0){
          if (ledFadeD==0){
            if (ledFadeE==0){
              if (ledFadeF==0){
                if (fadeupA==0){
                  if(fadeupB==0){
                    if (fadeupC==0){
                      if (fadeupD==0){
                        if (fadeupE==0){
                          if (fadeupF==0){
      ledWipe();
      delay(2500);   //time between patterns
      patternPlayed = patternPlayed + 1;
      
      if (patternPlayed == patternsPerTrigger){
        patternStartBit = 0;
        patternPlayed =0;
        triggersPlayed = triggersPlayed + 1;
        sleep_total = (sleep_total+220); //37 = 5 minutes 75= 10 minutes  220=30 minutes
        //wake up in triggersPerNight amount of time(interval + time)
        if (triggersPlayed == triggersPerNight){
          patternStartBit = 0; 
          sleep_total = 10000; // set to max interval
          sleepNow();
        }
        sleepNow();
      }
      repeat = 1;
    }}}}}}}}}}}}     
    }

void sleeping(){
  //digitalWrite(led1, HIGH);
  //delay(1000);
  //digitalWrite(led1, LOW);
  //delay(1000);
  sleepNow();
}
void wakeUpNow()        // here the interrupt is handled after wakeup
{
  digitalWrite(led1, HIGH);
  delay(1000);
  digitalWrite(led1, LOW);
  //startTime = 1;
  sleep_count = 3080;
  
  
}
 
  
  
  
  //_____________________________________________________________________________
  
  

void setup() {                
 
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(button1, INPUT);  
  pinMode(buttonPad, INPUT);
    
 pinMode (led1, OUTPUT);
 watchdogOn();                               // Turn on the watch dog timer.
 ADCSRA = ADCSRA & B01111111;                // Disable the ADC by setting the ADEN bit (bit 7) to zero.
 ACSR = B10000000;                           // Disable the analog comparator by setting the ACD bit (bit 7) to one.
 DIDR0 = DIDR0 | B00111111;
 attachInterrupt(0, wakeUpNow, LOW);
}


void loop() {
  detachInterrupt(0);
  
  if (patternStartBit ==1){
    patternStart();
  }
  
  if (startTime==1){
    sensorValue = analogRead(5);
    if (sensorValue>2){ 
        numTriggers= numTriggers+1;
    }
    if( TC4 == 0){
      timer4 = millis();
      TC4=1;
    }
    if (millis() - timer4 > 1500){
        if (numTriggers > (triggerDefault*1.3)){
          patternStartBit=1;
          startTime =0;
          TC4 = 0;
          numTriggers = 0;
          }else{
          sleep_total = (sleep_total+73);
          startTime = 0;
          TC4 = 0;
          numTriggers = 0;
          sleepNow();
          }
    } 
  }
  
  
    if (sleep_count > sleep_total) {      //if waittime has elapsed
      if (patternStartBit == 0){
    
         startTime=1;
      } 
    }
    if (sensorInitialized== 0){
      if (sleep_count > 660) {       //if it has been an hour
      sensorDefault = 0;
         if(TC3 == 0){                        //start timer
            timer3 = millis(); 
            TC3 = 1;
            }
         if (TC3 == 1){
             if (millis() - timer3 > 1500){
                if(X1==0){
                  triggerState1 = numTriggers;
                  numTriggers = 0;
                  X1=1;
                  }}
              if (millis() - timer3 > 3000){
                  if(X1 ==1){
                    triggerState2 = numTriggers;
                    numTriggers = 0;
                    X1=2;
                    }}
              if (millis() - timer3 > 4500){
                  if (X1==2){
                    triggerState3 = numTriggers;
                    numTriggers = 0;
                    triggerDefault = ((triggerState1+triggerState2+triggerState3)/3);
                    sensorInitialized= 1;
                    sensorDefault= 1;
                    
                    }}
              }   
              if (sensorValue>3){ 
              numTriggers= numTriggers+1; 
              }
           }
    }
  if (initialize == 0) {                  //if device has just started:- initalize
    if(TC1 == 0){                        //start timer
    timer1 = millis(); 
    TC1 = 1;
    }
    buttonState = digitalRead(button1); //read button1 value
    
   if (millis() - timer1 > 50){         //if it has been longer than 50ms
    initialize = 1;                      //initializing is over
   }
   if(buttonState == HIGH){             //if button has been pressed
      initialize = 1;                   //initializeing is over           
      startMenuPattern();
      startMenu = 1;                    //device is now in startMenu
    }
  }
  if (startMenu == 0){                             //load settings
    if(RC1 == 0){
     brightness = EEPROM.read(1);
     patternSpeed = EEPROM.read(2);
     pattern = EEPROM.read(3);
     waitTime = EEPROM.read(4);
     triggersPerNight = EEPROM.read(5);
     patternsPerTrigger = EEPROM.read(6);
     interval=((60* waitTime)+300);                //minutes to sleep
     sleep_total = (((float)interval*60)/8.1714);  //# of wakings per minutes of sleep
     delay(100);
     RC1 = 1; 
    }
      if (initialize == 1){                          //blink to signify wake up (NOT IN FINAL CODE
        if (startTime == 0){
          if(sensorDefault ==1){
            if (patternStartBit==0){
          //digitalWrite(led1, HIGH);
          //delay(500);
          //digitalWrite(led1, LOW);
          sleeping();
        }}}
          }
      }
//______________________________________________________________________________
 if (startMenu == 1){                   // In Start Menu 
 
     if(RC1 == 0){
     brightness = EEPROM.read(1);
     patternSpeed = EEPROM.read(2);
     pattern = EEPROM.read(3);
     waitTime = EEPROM.read(4);
     triggersPerNight = EEPROM.read(5);
     patternsPerTrigger = EEPROM.read(6);
     delay(100);
     RC1 = 1; 
     
     }
    
   buttonStatePad = digitalRead(buttonPad);
   buttonState = digitalRead(button1);  // reading button1 value
   if(buttonState == 1){                //press btn1 starts timer
     if (TC2 ==0){
       timer2 = millis();
       TC2 = 1;                         //timer is started
       }
     if(millis() - timer2 > 500){      //if held for longer than 1 second then
       registerHold();                  //execute this
       TC2 = 0;                         //then reset timer
       }
        
   }
   if (buttonState == 0){     //when button is released
      if(millis()-timer2 < 499){
        registerTap();
      }
       TC2=0;
       timer2 = 0;
     } 
   //_________________________________________________________________________
   if (menuItem==0){      //menu item 0
        displayBrightness();
     if (buttonStatePad == LOW){
       EEPROM.write(1, brightness);
       startMenuPattern();
       startMenuPattern();
     }
   }
   if (menuItem==1){
     displaySpeed();
     if (buttonStatePad == LOW){
       EEPROM.write(2, patternSpeed);
       startMenuPattern();
       startMenuPattern();
     }
   }
     
   if (menuItem==2){     
     if (pattern ==1){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
     if (pattern ==2){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
     if (pattern ==3){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
     if (pattern ==4){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
     if (pattern ==5){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
     if (pattern ==6){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
     if (pattern ==7){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
     if (pattern ==8){
         displayPattern();
         if (buttonStatePad == LOW){
             EEPROM.write(3, pattern);
             startMenuPattern();
             startMenuPattern();
         }
     }
    
   }
   
   if (menuItem==3){
     displayWaitTime();
     if (buttonStatePad == LOW){
       EEPROM.write(4, waitTime);
       //interval=((60*waitTime)+300);   //when device turns off this doesnt matter
       startMenuPattern();
       startMenuPattern();
     }
   }
   if (menuItem==4){
     displayTPN();
     if (buttonStatePad == LOW){
       EEPROM.write(5, triggersPerNight);
       startMenuPattern();
       startMenuPattern();
     }
   }
   if (menuItem==5){
       displayPPT();
     if (buttonStatePad == LOW){
       EEPROM.write(6, patternsPerTrigger);
       startMenuPattern();
       startMenuPattern();
     }
    }
   
   
   if (wipeCheck ==1){
   ledWipe();
   wipeCheck=0;
 }    
 }
}

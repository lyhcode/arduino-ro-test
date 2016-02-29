#include "limits.h"
  
/*
 * SENSOR1: 低壓
 * SENSOR2: 高壓
 */
#define SENSOR1 3
#define SENSOR2 4
    
/* LED1: 水源
 * LED2: 滿水
 * LED3: 製水
 * LED4: 沖洗
 */
#define LED1_PIN 8
#define LED2_PIN 9
#define LED3_PIN 10
#define LED4_PIN 11

#define Gate 5
#define Motor 6

int debugmode = 0; 
int Wt588pin=7;

unsigned long previous_time;

void blink_led1(unsigned long time_passed) {
  static unsigned long time = 0;
  time += time_passed;
  if ((time / 200) % 2 == 0){
    digitalWrite(LED1_PIN, HIGH);
  }
  else {
    digitalWrite(LED1_PIN, LOW);
  }
}

void setup() {

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  previous_time = millis();
}

void loop() {

  unsigned long current_time = millis();
  unsigned long time_passed;

  if (current_time > previous_time){
    time_passed = current_time - previous_time;
  }
  else {
    time_passed = ULONG_MAX - previous_time + current_time;
  }

  /*
   * 低壓開關：防止無水的時候馬達不停運轉。
   * 高壓開關：高壓開關在滿水的時候會自動斷電，不再製水。
   */

  boolean status1 = digitalRead(SENSOR1); //高壓
  boolean status2 = digitalRead(SENSOR2); //低壓
  
  if (status1 == HIGH) {
    Serial.println("The water is coming");
    digitalWrite(LED1_PIN, HIGH) ;
  }
  else {
    Serial.println("The water is blocked");
    if (time_passed >= 100){
      blink_led1(time_passed);
    }
  }
  
  if (status2 == HIGH) {
    Serial.println("The RO water is not Full");
    digitalWrite(LED2_PIN,LOW) ;
  }
  else {
    Serial.println("The RO water is Full");
    digitalWrite(LED2_PIN,HIGH) ;
  }
  
  if ((status1 == HIGH) && (status2 == HIGH)) {
    Serial.println("The making-water of the RO Machin is Starting");
    digitalWrite(LED3_PIN, HIGH) ;
    //digitalWrite(Gate,HIGH) ;
    //digitalWrite(Motor,HIGH) ;
  }
  else {
    Serial.println("The making-water of the RO Machin is stopping");
    digitalWrite(LED3_PIN, LOW) ;
    //digitalWrite(Gate,LOW) ;
    //digitalWrite(Motor,LOW) ;
  }

  if (time_passed >= 100) {
    previous_time = current_time;
  }
}

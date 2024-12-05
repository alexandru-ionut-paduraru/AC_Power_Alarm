#include "Ticker.h"

#define LED_B 5    //D1
#define LED_R 16     //D0
#define BUZZER 4    //D2
#define V_BAT_IN 13 //D7
#define V_SRC_IN 14 //D5
#define PW_KEEP_ALIVE 12 //D6

#define BUZZER_ON 10
#define BUZZER_OFF 0

bool mains_nok=false;
bool mains_nok_prev=false;
bool timer_new_tick=false;

void led_task();
void buzzer_task();
void power_keepAlive_task();

uint8_t led_task_state;
uint8_t buzzer_task_state;
uint8_t keepAlive_state;

Ticker myTimer;

unsigned int counterStart = 34286; 

void onTimer(){
  timer_new_tick=true;
}

void setup() {
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_B, LOW);
  pinMode(LED_R, OUTPUT);
  digitalWrite(LED_R, LOW);
  pinMode(PW_KEEP_ALIVE, OUTPUT);
  digitalWrite(PW_KEEP_ALIVE, HIGH);
  // pinMode(BUZZER, OUTPUT);
  analogWrite(BUZZER, BUZZER_OFF);
  
  pinMode(V_SRC_IN, INPUT);

  myTimer.attach(0.1, &onTimer);
}

void loop() {
  if (digitalRead(V_SRC_IN)){
    mains_nok=false;
  }else{
    mains_nok=true;
  }

  //transition from mains ok to mains nok
  if (mains_nok_prev == false && mains_nok==true){
    led_task_state=0;
    buzzer_task_state=0;
  }
  mains_nok_prev=mains_nok;

  if (timer_new_tick){
    if (mains_nok){
      led_task();
      buzzer_task();
    }else{
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_R, LOW);
      analogWrite(BUZZER, BUZZER_OFF);
    }
    power_keepAlive_task();
    timer_new_tick=false;
  }
}

void led_task(){
  switch(led_task_state){
    case 0:{
      digitalWrite(LED_B, HIGH);
      digitalWrite(LED_R, LOW);
      led_task_state++;
      break;
    }
    case 1:{
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_R, HIGH);
      led_task_state++;
      break;
    }
    default:{
      led_task_state=0;
      break;
    }
  }
}

void buzzer_task(){
  switch(buzzer_task_state){
    case 0:{
      analogWrite(BUZZER, 0);
      buzzer_task_state++;
      break;
    }
    case 1:{
      analogWrite(BUZZER, 20);
      buzzer_task_state++;
      break;
    }
    case 2:{
      analogWrite(BUZZER, 40);
      buzzer_task_state++;
      break;
    }
    case 3:{
      analogWrite(BUZZER, 80);
      buzzer_task_state++;
      break;
    }
    case 4:{
      analogWrite(BUZZER, 100);
      buzzer_task_state++;
      break;
    }
    case 5:{
      analogWrite(BUZZER, 200);
      buzzer_task_state++;
      break;
    }
    case 6:{
      analogWrite(BUZZER, 80);
      buzzer_task_state++;
      break;
    }
    case 7:{
      analogWrite(BUZZER, 40);
      buzzer_task_state++;
      break;
    }
    case 8:{
      analogWrite(BUZZER, 20);
      buzzer_task_state++;
      break;
    }
    case 9:{
      analogWrite(BUZZER, 10);
      buzzer_task_state++;
      break;
    }

    default:{
      buzzer_task_state=0;
      break;
    }
  }
}

void power_keepAlive_task(){
  switch(keepAlive_state){
    static uint16_t time_counter=0;
    time_counter++;
    case 0:{
      digitalWrite(PW_KEEP_ALIVE, HIGH);
      if (time_counter>=200){ //20sec
        keepAlive_state=1;
        time_counter=0;
      }
      break;
    }
    case 1:{
      digitalWrite(PW_KEEP_ALIVE, LOW);
      if (time_counter>=2){ //20 msec
        keepAlive_state=0;
        time_counter=0;
      }
      break;
    }
    default:{
      led_task_state=0;
      break;
    }
  }
}
#define TRIGGER_0 2
#define TRIGGER_1 3
#define TRIGGER_2 4
#define TRIGGER_3 5

#define ECHO_0 9
#define ECHO_1 10
#define ECHO_2 11
#define ECHO_3 12

#define LED 6
#define BUZZER 8
#define MinDist 10

#include <Servo.h>

int angle ; 
int change;
Servo servo_test;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TRIGGER_0, OUTPUT); //TRIGGER PINS
  pinMode(TRIGGER_1, OUTPUT);
  pinMode(TRIGGER_2, OUTPUT);
  pinMode(TRIGGER_3, OUTPUT);
  
  pinMode(ECHO_0, INPUT);//ECHO PINS
  pinMode(ECHO_1, INPUT);
  pinMode(ECHO_2, INPUT);
  pinMode(ECHO_3, INPUT);
  
  servo_test.attach(7); //MOTOR
  
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

}
int countdist(int sensor){

  int trigger,echo;
  switch (sensor){
    case 0:
      trigger=TRIGGER_0;
      echo=ECHO_0;
      break;
    case 1:
      trigger=TRIGGER_1;
      echo=ECHO_1;
      break;
    case 2:
      trigger=TRIGGER_2;
      echo=ECHO_2;
      break;
    case 3:
      trigger=TRIGGER_3;
      echo=ECHO_3;
      break;
  }
  
  int dis;
  digitalWrite (trigger, LOW);
  delayMicroseconds (3);
  digitalWrite (trigger, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigger, LOW);
  dis = (pulseIn(echo, HIGH)/2)*0.0343;
  return dis;
}

void turn (int initial, int last){
  servo_test.write(initial);
  delay(10);
  servo_test.write(last);
}

bool istriggered(int sensor){
  if ((countdist(sensor)>0) && (countdist(sensor)<10))
    return true;
  else
    return false;
}

int watchdog(){

  if (istriggered(0)){
    Serial.print("sensor0 is "); 
    Serial.println(countdist(0));
    
    if (istriggered(1) && !istriggered(2) && !istriggered(3))
      return 0;
  }
  
  if (istriggered(3)){
    Serial.print("sensor3 is "); 
    Serial.println(countdist(3));
    
    if (istriggered(2) && !istriggered(1) && !istriggered(0))
      return 3;
  }

  return 99;

}

void loop() {

  if (watchdog()==0){
  
    turn(0,90);
  
    while (istriggered(0) || istriggered(1)|| istriggered(2) || istriggered(3)){
       digitalWrite (LED, HIGH);
       tone(BUZZER, 1000); // Send 1KHz sound signal...
       delay(1000);        // ...for 1 sec
       noTone(BUZZER);     // Stop sound...
       delay(500);
       digitalWrite (LED, LOW);
     }
     
     turn(90,0);
  }
  
  if (watchdog()==3){
  
    turn(0,90);
  
    while (istriggered(0) || istriggered(1)|| istriggered(2) || istriggered(3)){
       digitalWrite (LED, HIGH);
       tone(BUZZER, 1000); // Send 1KHz sound signal...
       delay(1000);        // ...for 1 sec
       noTone(BUZZER);     // Stop sound...
       delay(500);
       digitalWrite (LED, LOW);
     }
  
     turn(90,0);
  }
  
  else if (watchdog()==99){
    Serial.println("No train ha-ha!");
    turn(0,0);
  }

}


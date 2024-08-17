#include <L298NX2.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define FRONT_LED_PIN 10
#define REAR_LED_PIN 12
#define SERVO_SIG 11
#define HC_TXD 3
#define HC_RXD 4
#define ULTRA_TRIG 9
#define ULTRA_ECHO 2
#define MOTOR_ENA 5
#define MOTOR_ENB 6
#define IN1 A1
#define IN2 A2
#define IN3 A3
#define IN4 A4

L298NX2 motors(MOTOR_ENA,IN2,IN1,MOTOR_ENB,IN4,IN3);
SoftwareSerial mySerial(HC_TXD, HC_RXD);
bool hc05Flag = false;

//bluetooth commend by character.
typedef const char command;
command FORWARD = 'F';
command BACKWARD = 'B';
command LEFT = 'L';
command RIGHT = 'R';
command STOP = '0';
command FRONT_LEDS = '1';
command REAR_LEDS = '2';
command SERVO_RIGHT = '+';
command SERVO_LEFT = '-';
command CHECK_CONNECTION = 'C';

//leds variable.
int rearLedState = 0;
int frontLedState = 0;


void setup() {
 mySerial.begin(38400);

 pinMode(FRONT_LED_PIN,OUTPUT);
 pinMode(REAR_LED_PIN,OUTPUT);

 //init all motors pin.
  pinMode(MOTOR_ENA , OUTPUT);
  pinMode(MOTOR_ENB , OUTPUT);
  pinMode(IN1 , OUTPUT);
  pinMode(IN2 , OUTPUT);
  pinMode(IN3 , OUTPUT);
  pinMode(IN4 , OUTPUT);

  motors.setSpeed(200);

}

void loop() {
  char input;
  if(mySerial.available()){
    hc05Flag = true;
    input = mySerial.read();
  }
  if(hc05Flag){
    switchCommand(input);
    hc05Flag = false;
  }  
}

//switch between bluetooth command.
void switchCommand(char input){
  switch (input) {
    case FORWARD:{
      motors.forward();
      break;
    }
    case BACKWARD:{
      motors.backward();
      break;
    }
    case LEFT:{
      motors.forwardB();
      motors.backwardA();
      break;
    }
    case RIGHT:{
      motors.forwardA();
      motors.backwardB();
      break;
    }
    case FRONT_LEDS:{
      toglleFrontLed();
      break;
    }
    case REAR_LEDS:
    { 
      toggleRearLed();
      break;
    }
    case STOP:
    {
      motors.stop();
      break;
    }
    case CHECK_CONNECTION:
      mySerial.print('Y');
    default:
      break;
  }
}

//toggle the two rear leds.
void toggleRearLed(){
  rearLedState = (rearLedState == 0) ? 1 : 0;
  digitalWrite(REAR_LED_PIN,rearLedState);
}

//toggle the two front leds.
void toglleFrontLed(){
  frontLedState = (frontLedState == 0) ? 1 : 0;
  digitalWrite(FRONT_LED_PIN,frontLedState);
}










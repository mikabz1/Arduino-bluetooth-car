/*
what thie code do?

control the car with ch-05 from smartphone throuh app.
thre is 6 controls commend.
FORWARD ,BACKWARD , LEFT , RIGHT.
turn on front and back LEDs.

also has some logic for the servo and the ultrasonic sesor but realy at point.
there will be in comment.
*/


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

//bluetooth comment by character.
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


//sevo variable.
// Servo servo;
// int servoAngle = 90;

L298NX2 motors(MOTOR_ENA,IN2,IN1,MOTOR_ENB,IN4,IN3);

//hc-05 variables.
SoftwareSerial mySerial(HC_TXD, HC_RXD);
bool hc05Flag = false;

//leds variable.
int rearLedState = 0;
int frontLedState = 0;

//ultrasonic sensor variables.
// unsigned long previusUltrasonicTrigger = millis();
// unsigned long ultrasonicTriggerDelay = 100;
// volatile unsigned long echoTimerBegin;
// volatile unsigned long echoTimerEnd;
// volatile bool ultrasonicFlag = false;


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  mySerial.begin(9600);

  // Serial.println("Initializing...");
  // Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(FRONT_LED_PIN,OUTPUT);
  pinMode(REAR_LED_PIN,OUTPUT);


  // pinMode(ULTRA_TRIG,OUTPUT);
  // pinMode(ULTRA_ECHO,INPUT);

  //attachInterrupt(digitalPinToInterrupt(ULTRA_ECHO),echoPinInterrupt,CHANGE);

  //init all motors pin.
  pinMode(MOTOR_ENA , OUTPUT);
  pinMode(MOTOR_ENB , OUTPUT);
  pinMode(IN1 , OUTPUT);
  pinMode(IN2 , OUTPUT);
  pinMode(IN3 , OUTPUT);
  pinMode(IN4 , OUTPUT);

  motors.setSpeed(200);

  // servo.attach(SERVO_SIG);
  // servo.write(servoAngle);
}

// bool obstable = false;

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
 
  //  unsigned long currentTime = millis();
  // //trigger ultrasonic sensor every 'ultrasonicTriggerDelay'. 
  // if(currentTime - previusUltrasonicTrigger > ultrasonicTriggerDelay){
  //   previusUltrasonicTrigger += ultrasonicTriggerDelay; 
  //   triggerUltrasonic();
  // }

  //if there a new 'sensor value' from interrupt.
  // if(ultrasonicFlag){
  //   ultrasonicFlag = false;
  //   double distance = (echoTimerEnd - echoTimerBegin)/58.0;//cm , (148.0 fo inches.)
  //   if((distance < 20) && !obstable){
  //     motors.stop();
  //     obstable = true;
  //   }
  //   if((distance > 20) && obstable){
  //     obstable = false;
  //   }
  // }

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
      break;
    }
    case RIGHT:{
      motors.forwardA();
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
    case SERVO_RIGHT:
    {
      if(servoAngle < 180){
        servoAngle += 15;
        servo.write(servoAngle);
      }
      break;
    }
    case SERVO_LEFT:
    {
      if(servoAngle > 0){
        servoAngle -= 15;
        servo.write(servoAngle);
      }
      break;
    }
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

//trigger the ultrasonic trigger pin.
void triggerUltrasonic(){
  digitalWrite(ULTRA_TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRA_TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_TRIG,LOW);
}

//interrupt logic.
void echoPinInterrupt(){
  if(digitalRead(ULTRA_ECHO) == HIGH){
    echoTimerBegin = micros();
  }
  else{
    echoTimerEnd = micros();
    ultrasonicFlag = true;
  }
}

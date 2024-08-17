#include <SoftwareSerial.h>
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#define JOY_X A5
#define JOY_Y A4
#define JOY_BTN A3
#define RIGHT_BTN 6
#define LEFT_BTN 7
#define cs   10
#define dc   9
#define rst  8
// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

SoftwareSerial mySerial(3,2);

//joystick variables.
int xValue;
int yValue;

//buttons variables.
byte rightBtnState = LOW;
byte leftBtnState = LOW;
unsigned long previusRightBtnChange = millis();
unsigned long previusLeftBtnChange = millis();
unsigned long debounceRightBtn = 500;
unsigned long debounceLeftBtn = 500;

//connection status variable.
unsigned long previusConectionCheck = millis();
unsigned long connectionCheckFreq = 1000;
char connectionCheck;

//leds variable.
int rearLedState = 0;
int frontLedState = 0;

void setup() {
  Serial.begin(38400);
  mySerial.begin(38400);
  initTFTdisplay();
  joystickCommand();
  pinMode(RIGHT_BTN , INPUT);
  pinMode(LEFT_BTN , INPUT);
  
  //check the connection -> turn the tft connection status 
  //to green if conection established.
  while(!checkConection()){}
  TFTscreen.fillRoundRect(110,10, 15,15,4,ST7735_GREEN);
  connectionCheck = ' ';
}

void loop() {
  scanJoystick();
  joystickCommand();
  unsigned long currentTime = millis();
  leftBtnSample(currentTime);
  rightBtnSample(currentTime);
  currentTime = millis();
  checkConnectionSample(currentTime);
  
}
bool checkConection(){
  mySerial.print('C');
  if(mySerial.available()){
    connectionCheck = mySerial.read();
  }
  return (connectionCheck == 'Y') ? true : false;
}
void checkConnectionSample(unsigned long currentTime){
  if(currentTime - previusConectionCheck > connectionCheckFreq){
    int i = 0;
    while(!checkConection()){
      i++;
      if(i > 5){
        TFTscreen.fillRoundRect(110,10, 15,15,4,ST7735_BLUE);
      }
    }
    TFTscreen.fillRoundRect(110,10, 15,15,4,ST7735_GREEN);
    connectionCheck = ' ';
    previusConectionCheck = millis();
  }
}
void rightBtnSample(unsigned long currentTime){
  if(currentTime - previusRightBtnChange > debounceRightBtn){
    byte currentBtnState = digitalRead(RIGHT_BTN);
    if(currentBtnState != rightBtnState){
      previusRightBtnChange += debounceRightBtn;
      rightBtnState = currentBtnState;
      mySerial.print('1');
    }
  }
}
void leftBtnSample(unsigned long currentTime){
  if(currentTime - previusLeftBtnChange > debounceLeftBtn){
    byte currentBtnState = digitalRead(LEFT_BTN);
    if(currentBtnState != leftBtnState){
      previusLeftBtnChange += debounceLeftBtn;
      leftBtnState = currentBtnState;
      //toglleLedDisplay('L');
      mySerial.print('2');
    }
  }
}
void scanJoystick(){
  xValue = analogRead(JOY_X); 
  yValue = analogRead(JOY_Y);
}
void joystickCommand(){
  if(xValue > 1000){
    mySerial.print('B');
    TFTscreen.fillTriangle(8 , 96 , 42, 96, 25, 113, ST7735_GREEN);
  }
  else if(xValue < 10){
    mySerial.print('F');
    TFTscreen.fillTriangle(8 , 89 , 42, 89, 25, 72, ST7735_GREEN);
  }
  if(yValue > 1000){
    mySerial.print('L');
    TFTscreen.fillTriangle(96 , 73 , 96, 110, 78, 92, ST7735_GREEN);
  }
  else if(yValue < 10){
    mySerial.print('R');
    TFTscreen.fillTriangle(103 , 73 , 103, 110, 121 , 92, ST7735_GREEN);
  }
  if(xValue > 470 && xValue < 520 && yValue > 490 && yValue < 530){
    mySerial.print('0');
    TFTscreen.fillTriangle(8 , 96 , 42, 96, 25, 113, ST7735_CASET);
    TFTscreen.fillTriangle(8 , 89 , 42, 89, 25, 72, ST7735_CASET);
    TFTscreen.fillTriangle(96 , 73 , 96, 110, 78, 92, ST7735_CASET);
    TFTscreen.fillTriangle(103 , 73 , 103, 110, 121 , 92, ST7735_CASET);
  }
}
void initTFTdisplay(){
  TFTscreen.begin();
  TFTscreen.setRotation(2);
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);

  //connection status.
  TFTscreen.textSize(1);
  TFTscreen.text("connection status:", 0, 14);
  TFTscreen.fillRoundRect(110,10, 15,15,4,ST7735_BLUE);
  // TFTscreen.fillRoundRect(110,10, 15,15,4,ST7735_GREEN);

  //separator.
  TFTscreen.drawLine(0 , 35 , 127 , 35, ST7735_BLUE);

  //distance display.
  TFTscreen.text("Distance:", 0,40);
  TFTscreen.text("xxx", 55,40);

  //servo display.
  TFTscreen.text("servo angle:", 0,55);
  TFTscreen.text("xxx", 74, 55);

  //separator.
  TFTscreen.drawLine(0 , 65 , 127 , 65 , ST7735_BLUE);

  //init joystick display.
  TFTscreen.text("joystick",40,70);
  //back triangle.
  TFTscreen.drawTriangle(5 , 95 , 45, 95, 25, 115, ST7735_WHITE);
  //on
  TFTscreen.fillTriangle(8 , 96 , 42, 96, 25, 113, ST7735_GREEN);
  //off
  TFTscreen.fillTriangle(8 , 96 , 42, 96, 25, 113, ST7735_CASET);
  //forward triangle.
  TFTscreen.drawTriangle(5 , 90 , 45, 90, 25, 70, ST7735_WHITE);
  //on
  TFTscreen.fillTriangle(8 , 89 , 42, 89, 25, 72, ST7735_GREEN);
  //off
  TFTscreen.fillTriangle(8 , 89 , 42, 89, 25, 72, ST7735_CASET);
  //left triangle.
  TFTscreen.drawTriangle(97 , 72 , 97, 112, 77 , 92, ST7735_WHITE);
  //on.
  TFTscreen.fillTriangle(96 , 73 , 96, 110, 78, 92, ST7735_GREEN);
  //off
  TFTscreen.fillTriangle(96 , 73 , 96, 110, 78, 92, ST7735_CASET);
  //right triangle.
  TFTscreen.drawTriangle(102 , 72 , 102, 112, 122 , 92, ST7735_WHITE);
  //on.
  TFTscreen.fillTriangle(103 , 73 , 103, 110, 121 , 92, ST7735_GREEN);
  //off.
  TFTscreen.fillTriangle(103 , 73 , 103, 110, 121 , 92, ST7735_CASET);
  
  //separator.
  TFTscreen.drawLine(0 , 118 , 127 , 118, ST7735_BLUE);

  //LEDs status display.
  TFTscreen.textSize(2);
  TFTscreen.text("front led", -2, 123);
  TFTscreen.text("back led", 0, 143);
  //toggle on off led display.
  TFTscreen.drawCircle(118, 130, 7,ST7735_WHITE);
  // TFTscreen.fillCircle(118, 130, 5, ST7735_GREEN);
  TFTscreen.drawCircle(118, 150, 7,ST7735_WHITE);
  // TFTscreen.fillCircle(118, 150, 5, ST7735_GREEN);
  TFTscreen.fillCircle(118, 130, 5,ST7735_CASET);
  TFTscreen.fillCircle(118, 150, 5, ST7735_CASET);
}


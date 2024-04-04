#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

bool isExtended = false;

/*servo classes*/
Servo servo1;  // create servo object to control the first servo
Servo servo2;  // create servo object to control the second servo

/*output pins*/
int lampPin = 11;
int servo1Output = 6;
int servo2Output = 3;

/*input pins*/
int potPin = 1; // pot //analog
int photoResPin = 0; // photoresistor //analog
int stateButton = 8; // digital pin

/*LCD call*/
LiquidCrystal_I2C lcd(0x27,  16, 2);

/*servo positions*/
int servo1Pos = 0; // servo1 pos
int servo2Pos = 0; // servo2 pos
bool servo1Finished = false;

/*button states*/
int buttonState = 0;
int prvButtonState = 0;
bool isPressed = false;

/*button press counter*/
int buttonCount = 0;

/*potValue and photoresistor values*/
int potValue = 0;
int pResValue = 0;

/*setups*/
void setup() {
  Serial.begin(9600);
  /*LCD setup*/
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("OFF");
  retractArm();
  /*pin modes*/
  pinMode(stateButton, INPUT);
  pinMode(photoResPin, INPUT);
  pinMode(lampPin, OUTPUT);
}

/*loop*/
void loop() {
  // put your main code here, to run repeatedly:
  control();
  delay(100);
}

/*mode handling, display binary LED here as well*/
void control() {
  buttonState = digitalRead(stateButton);
  if (buttonState != prvButtonState) {
    if (buttonState == HIGH) {
      buttonCount++;
      analogWrite(lampPin, 0); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    }
  }
  switch(buttonCount) {
    case 0: //off
      //light off, stow away, if not already
      //if(isExtended)
        
      //display state
      if (buttonState == 1 && prvButtonState == 1) {
        retractArm();
        lcd.clear();
        lcd.print("OFF");
      }
      isPressed = false;
      break;
    case 1: //time
      //time function
      
      //display state
      if (buttonState == 1 && prvButtonState == 1) {
        lcd.clear();
        lcd.print("TIME MODE");
        
      }
      isPressed = false;
      break; 
    case 2: //manual
      //potentiometer brightness
      
      potValue = analogRead(potPin);  // read the input pin
      analogWrite(lampPin, potValue/4); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      Serial.println(potValue);
      //display state
      
      if (buttonState == 1 && prvButtonState == 1) {
        lcd.clear();
        lcd.print("MANUAL MODE");
        extendArm();
      }
      
      isPressed = false;
      break;
    case 3: //ambient
      //photoresitor code
      
      pResValue = analogRead(photoResPin);
      if (pResValue < 500)
        analogWrite(lampPin, 255);
      else
        analogWrite(lampPin, 0);
      //display state
      
      if (buttonState == 1 && prvButtonState == 1) {
        lcd.clear();
        lcd.print("AMBIENT MODE");
      }
      isPressed = false;
      break;
    default:
      buttonCount = 0;
      break;
  }
  prvButtonState = buttonState;
}

/*extend*/
void extendArm() {
  servo1.attach(servo1Output);  // attaches the servo on servo1output to the servo object
  servo1Activate(); // Activates Servo 1

  delay(250);  // Wait after activation sequence is complete

  servo2.attach(servo2Output); // attaches the servo on servo2output to the servo object
  servo2Activate(); // Activates Servo 2
}

/*retract*/
void retractArm() {
  servo2.attach(servo2Output); // attaches the servo on servo2output to the servo object
  servo2Deactivate(); // Activates Servo 2

  delay(250);

  servo1.attach(servo1Output);  // attaches the servo on servo1output to the servo object
  servo1Deactivate(); // Activates Servo 1
}

// Activates Servo 1 by making it rotate 180 degrees
void servo1Activate() {
  for (servo1Pos = servo1.read(); servo1Pos < 180; servo1Pos++) { // goes from 0 degrees to 180 degrees
    servo1.write(servo1Pos);              // tell servo1 to go to position in variable 'pos1'
    delay(10);                    
  }
}

void servo1Deactivate() {
  for (servo1Pos = servo1.read(); servo1Pos > 100; servo1Pos--) { // goes from 0 degrees to 180 degrees
    servo1.write(servo1Pos);              // tell servo1 to go to position in variable 'pos1'
    delay(15);                
  }
}

/*servo 2 extend*/
void servo2Activate() {
  for (servo2Pos = servo2.read(); servo2Pos < 90; servo2Pos++) { // goes from 0 degrees to 90 degrees
    servo2.write(servo2Pos);              // tell servo2 to go to position in variable 'pos2'
    delay(15);                    
  }
}

/*servo 2 retract*/
void servo2Deactivate() {
  for (servo2Pos = servo2.read(); servo2Pos > 0; servo2Pos--) { // goes from 0 degrees to 90 degrees
    servo2.write(servo2Pos);              // tell servo2 to go to position in variable 'pos2'
    delay(15);                
  }
}
// define the libraries used and pins

#include <LiquidCrystal_I2C.h>
#define trigPin 7
#define echoPin 6
#define LEDlampRed 9
#define  LEDlampBlue 10
#define LEDlampGreen 11 
#define soundbuzzer 3
#define buttonPin 2
#define buttonPin1 4
#define buzzerOnTime 5000   // added with LCD

// Set the LCD address to 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

int sound  = 500;

void setup() {

  // setup the inputs and outputs
  // button (with pullup) and echo are input while the rest are output
  Serial.begin (9600);
  pinMode(trigPin,  OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDlampRed, OUTPUT);
  pinMode(LEDlampBlue,  OUTPUT);
  pinMode(LEDlampGreen, OUTPUT);
  pinMode(soundbuzzer, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // initialize LCD
  lcd.init();
  lcd.setCursor(0, 0);
}
void  loop() {

  // clear LCD and turn backlights off as long as the system is not on
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.noBacklight();

  // check the state of the button
  int buttonState = digitalRead(buttonPin);
  if(buttonState == LOW){
    // if Button is on, get the system running
    // turn LCD backlights on
    lcd.backlight();

    // keep in loop with a break condition (button is pressed again)
    while(true){
    long durationindigit, distanceincm;
    digitalWrite(trigPin, LOW);  
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // measure distance and convert into cm
    durationindigit = pulseIn(echoPin, HIGH);
    distanceincm = (durationindigit/2) / 29.1;

    // first threshold turns the green LED on
    if (distanceincm < 50) {
        digitalWrite(LEDlampGreen, HIGH);
    }
    else {
        digitalWrite(LEDlampGreen,  LOW);
    }

    // second threshold turns the blue LED on
    if (distanceincm < 20) {
      digitalWrite(LEDlampBlue,  HIGH);
    }
    else {
      digitalWrite(LEDlampBlue,LOW);
    }

    // third threshold turns the red LED on
    if (distanceincm  < 10) {
      digitalWrite(LEDlampRed, HIGH);
      sound = 1000;
    }
    else  {
      digitalWrite(LEDlampRed,LOW);
    }

    // if no object is within threshold
    if (distanceincm > 50 ||  distanceincm <= 0){
      Serial.println("Outside the range of distances");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Outside range");
      noTone(soundbuzzer);
    }

    // sound buzzer if within any threshold and display distance
    else {
      lcd.clear();
      Serial.print("Object at ");
      Serial.print(distanceincm);
      Serial.println("  cm");
      tone(soundbuzzer, sound);
      lcd.print("Object at ");
      lcd.print(distanceincm);
      lcd.print(" cm");
      lcd.setCursor(0, 1);
      lcd.print("Intruder Alert");
      lcd.blink();
    }
    
    delay(200);

    // check if button is pressed, in which case the system is turned off
    int buttonState1 = digitalRead(buttonPin);
    if(buttonState1==LOW){
      Serial.println("off");
//      lcd.noBacklight();
      delay(200);
      break;
      }
    }
  }
  else{
    Serial.println("System off");
    }
}

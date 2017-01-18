/* Created by Raphango ~ 14/feb/2014
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
"Come to me, all you who are weary and burdened, and I will give you rest.
Take my yoke upon you and learn from me, for I am gentle and humble in heart,
and you will find rest for your souls.For my yoke is easy and my burden is light."
Matthew 11:28-30
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#########Breadboard circuit mounting:########
 * LCD VSS pin to ground
 * LCD VDD pin to +5V
 * 2K2Ohm resistor: ends to LCD VO pin and ground
 * LCD RS pin to digital pin 9
 * LCD R/W pin to ground
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD A pin to +5V
 * LCD K pin to ground
 * Buzzer + to 220Ohm Resistor leg and - to ground. The other resistor leg goes to pin 12.
 * HC-SCR GND to ground
 * HC-SCR Echo to pin 11
 * HC-SCR Trig to pin 10
 * HC-SCR Vcc to +5V
##############################################
*/

//Defining pins
#define trigPin 9
#define echoPin 8
#define buzPin 12

//Including the LCD and sound library:
#include <LiquidCrystal.h>
#include "pitches.h"
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Initialize the LCD library

/////Definition of the LCD special characters///
uint8_t cc0[8] = {0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07};
uint8_t cc1[8] = {0x00,0x00,0x00,0x00,0x00,0x10,0x18,0x1C};
uint8_t cc2[8] = {0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00};
uint8_t cc3[8] = {0x1C,0x18,0x10,0x00,0x00,0x00,0x00,0x00};
uint8_t cc4[8] = {0x00,0x00,0x00,0x10,0x18,0x0C,0x04,0x06};
uint8_t cc5[8] = {0x06,0x04,0x0C,0x18,0x10,0x00,0x00,0x00};
uint8_t cc6[8] = {0x10,0x08,0x0C,0x06,0x02,0x03,0x03,0x03};
uint8_t cc7[8] = {0x03,0x03,0x03,0x02,0x06,0x0C,0x08,0x10};

////Sound notes setup
int melody0[] = {NOTE_B3, NOTE_B3,NOTE_B3, NOTE_G3, NOTE_B3,0, NOTE_D4, NOTE_D3};
int melody1[] = {NOTE_C6,NOTE_C6,NOTE_C6,NOTE_C6};
int melody2[] = {NOTE_C6,NOTE_C6,NOTE_C6};
int melody3[] = {NOTE_C6,NOTE_C6};
int melody4[] = {NOTE_C6};

//Add melodies here, and at the end of the "music" array.
int* music[6/*number of melodies + 1*/] = {melody0, melody1, melody2, melody3, melody4 /*,add here*/};

////Sound notes durations
int notesMelody0[] = {8,4,4,8,8,8,2,4};
int notesMelody1[] = {8,8,8,8};
int notesMelody2[] = {8,8,8};
int notesMelody3[] = {8,8};
int notesMelody4[] = {4};

//Add durations of the melodies here, and at the end of the "noteDurations" array.
int* noteDurations[6/*number of melodies + 1*/] = {notesMelody0, notesMelody1, notesMelody2 , notesMelody3 , notesMelody4  /*,add here*/};

////Sound sizes
int sizeMelody0 = sizeof(melody0)/sizeof(int);
int sizeMelody1 = sizeof(melody1)/sizeof(int);
int sizeMelody2 = sizeof(melody2)/sizeof(int);
int sizeMelody3 = sizeof(melody3)/sizeof(int);
int sizeMelody4 = sizeof(melody4)/sizeof(int);
//Add sizes here, and at the end of the "sizes" array.
int sizes[] = {sizeMelody0,sizeMelody1,sizeMelody2,sizeMelody3,sizeMelody4 /*,add here*/};

////Tone Function - Plays the tone
int playSong(int track, float durConst){
    for (int thisNote = 0; thisNote < sizes[track]; thisNote++) {
    int noteDuration = 1000/noteDurations[track][thisNote];
    tone(buzPin, music[track][thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * durConst;
    delay(pauseBetweenNotes);
    noTone(buzPin);
  }
}

//Declaring a variable that will count the millis so the blinking is independent of "delays"
unsigned long millisCounter = 0;
int displaying = 0;

////////////////////////////////////
//////////////SETUP/////////////////
////////////////////////////////////
void setup(){
  // Set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  // Set up the LCD custom chars (8 maximum)
  lcd.createChar(0, cc0);
  lcd.createChar(1, cc1);
  lcd.createChar(2, cc2);
  lcd.createChar(3, cc3);
  lcd.createChar(4, cc4);
  lcd.createChar(5, cc5);
  lcd.createChar(6, cc6);
  lcd.createChar(7, cc7);
  //Initializes HC-SCR04 pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //Plays intro song
  playSong(0,1.30);
}

////////////////////////////////////
//////////////LOOP//////////////////
////////////////////////////////////

void loop()
{
  unsigned long millisNow = millis();
  long duration;
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //Measures the distance
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  if (distance >= 400 || distance <= 0){//Evaluate the distance value.
    lcd.setCursor(0, 0);
    lcd.print("Fora do alcance    ");
    lcd.setCursor(0, 1);
    lcd.print("                  ");
    }
  else {

    Serial.print(distance);
    Serial.println(" cm");
    lcd.setCursor(0, 0);
    lcd.print("UZAKLIK  :");
    lcd.setCursor(2, 1);
    lcd.print(distance);
    lcd.print(" cm.  ");
    lcd.setCursor(0, 3);
    lcd.print("www.arduinom.org");
    //Distance animation
    if(distance <= 10)
    {
      if(millisNow-millisCounter>50 && displaying == 0){//This part is used for the blinking while at 10 cm less.
      //Each of these "lcd.setCursor" and "lcd.write" are responsible for the drawings.
      lcd.setCursor(10, 0);//This positionates the cursor on the desired slot
      lcd.write(byte(0));//This writes the prefab symbols
      lcd.write(byte(1));
      lcd.write(byte(4));
      lcd.write(" ");
      lcd.write(" ");
      lcd.write(" ");
      lcd.setCursor(10, 1);
      lcd.write(byte(2));
      lcd.write(byte(3));
      lcd.write(byte(5));
      lcd.write(" ");
      lcd.write(" ");
      lcd.write(" ");
      playSong(1,1.30);//This plays the "beep"
      millisCounter = millisNow;
      displaying = 1;
      } else {lcd.setCursor(10, 0);lcd.write("      ");lcd.setCursor(10, 1);lcd.write("      ");playSong(1,1.30);displaying=0;millisCounter = millisNow;}
    }
    else if(distance <= 30)
    {
    lcd.setCursor(10, 0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.write(byte(4));
    lcd.write(byte(6));
    lcd.write(" ");
    lcd.write(" ");
    lcd.setCursor(10, 1);
    lcd.write(byte(2));
    lcd.write(byte(3));
    lcd.write(byte(5));
    lcd.write(byte(7));
    lcd.write(" ");
    lcd.write(" ");
    playSong(2,2.07);
    }
    else if(distance <= 70){
    lcd.setCursor(10, 0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.write(byte(4));
    lcd.write(byte(6));
    lcd.write(byte(6));
    lcd.write(" ");
    lcd.setCursor(10, 1);
    lcd.write(byte(2));
    lcd.write(byte(3));
    lcd.write(byte(5));
    lcd.write(byte(7));
    lcd.write(byte(7));
    lcd.write(" ");
    playSong(3,3.60);
    }
    else if(distance <= 400){
    lcd.setCursor(10, 0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.write(byte(4));
    lcd.write(byte(6));
    lcd.write(byte(6));
    lcd.write(byte(6));
    lcd.setCursor(10, 1);
    lcd.write(byte(2));
    lcd.write(byte(3));
    lcd.write(byte(5));
    lcd.write(byte(7));
    lcd.write(byte(7));
    lcd.write(byte(7));
    playSong(4,3.60);
    }
  }
}

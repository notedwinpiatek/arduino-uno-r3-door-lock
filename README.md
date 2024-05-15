# Arduino Uno R3 Door Lock System

This project demonstrates a simple yet effective door lock system built with an Arduino Uno R3, a 4x4 keypad for user input, a 16x2 I2C LCD for visual feedback, and a servo motor to act as the locking mechanism. The system allows users to enter a pin code via the keypad to unlock the door, providing a practical introduction to integrating various electronic components and coding them to work together.

## Features

- **Keypad Input**: Enter the pin code using a 4x4 keypad.
- **LCD Feedback**: Displays messages to the user on a 16x2 I2C LCD screen.
- **Servo Motor**: Controls the locking mechanism.
- **LED Indicator**: Lights up when an incorrect pin code is entered.

## Components

- **Arduino Uno R3**: The microcontroller used to control the system.
- **4x4 Keypad**: Used for entering the pin code.
- **16x2 I2C LCD**: Displays messages such as welcome and error notifications.
- **Servo Motor**: Acts as the physical lock.
- **LED**: Indicates incorrect pin code entry.
- **Jumper Wires**: Used to connect components.

## Circuit Diagram

![Circuit Diagram](screenshot.png)

## Code

The Arduino code handles the input from the keypad, compares it to a preset pin code, and operates the servo motor based on whether the inputted pin code is correct or incorrect. The LCD provides visual feedback during this process.

```cpp
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {11, 10, 9, 8};
byte colPins[numCols] = {7, 6, 5, 4};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

String pinCode = "C0D3";
String codeInput1 = "";
String codeInput2 = "";
bool codeDone = false;
const int ledPin = 13;

void setup()
{
  myservo.attach(12);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void codeClear()
{
  lcd.clear();
  lcd.setCursor(0,0);
  codeInput1 = "";
}

void loop()
{
  myservo.write(0);
  char key = myKeypad.getKey();
  if (key != NO_KEY)
  {    
    if (!codeDone && codeInput1.length() < 4) 
    {
      codeInput1 += key;
      lcd.print(key);
    }
    if (codeInput1.length() == 4)
    {
      codeInput2 = codeInput1;
      if (codeInput2 == pinCode)
      {    
        lcd.clear();
        delay(500);
        myservo.write(180);
        lcd.print("Welcome :)");
        delay(5000);
        myservo.write(-180);
        codeClear();
        
      }
      else
      {
        lcd.clear();
        lcd.print("Wrong Code :(");
        digitalWrite(ledPin, HIGH);
        if (key == '#')
        {
            codeClear();
            digitalWrite(ledPin, LOW);
        }
      }
    }    
  }
}

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

//Variables
String pinCode = "C0D3";
String codeInput1 = "";
String codeInput2 = "";
bool codeDone = false;
const int ledPin = 13;

//Setup
void setup()
{
  myservo.attach(12);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

//Clear Input
void codeClear()
{
  lcd.clear();
  lcd.setCursor(0,0);
  codeInput1 = "";
}

//Main loop
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

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2, 6, 7, 8, 9);

const int changeButton = 23;
int state = 0;
bool buttonPressed = false;

const char message[] = "SALE 90% OFF";
int msgLength = sizeof(message) -1;

void setup() {
  // put your setup code here, to run once:
  pinMode(changeButton, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print(message);
  delay(1000);
  

}

void loop() {
  checkButtonPress();

  switch(state){
    case 0:
      scrollLeft();
      break;
    case 1:
      scrollRight();
      break;
    case 2:
      waveText();
      break;
    case 3:
      flashText();
      break;
  }
}


void checkButtonPress(){
  if(digitalRead(changeButton) == LOW){
    if(!buttonPressed){
      buttonPressed = true;
      state = (state+1) % 4 ;
      lcd.clear();
      delay(300);
    }
  }
  else{
    buttonPressed = false;
  }
  
}

void scrollLeft() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  
  for (int i = 0; i < msgLength; i++) {
    delay(300);
    lcd.scrollDisplayLeft();
    checkButtonPress();
  }
}


void scrollRight() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  
  for (int i = 0; i < msgLength; i++) {
    delay(300);
    lcd.scrollDisplayRight();
    checkButtonPress();
  }
}
void waveText() {
  lcd.clear();
  for (int i = 0; i < msgLength; i++) {
    lcd.setCursor(i, 0);
    lcd.print(message[i]);
    delay(300);
    checkButtonPress();
  }
}

void flashText() {
  for (int i = 0; i < 5; i++) {
    lcd.clear();
    delay(300);
    lcd.setCursor(2, 0);
    lcd.print(message);
    delay(300);
    checkButtonPress();
  }
}

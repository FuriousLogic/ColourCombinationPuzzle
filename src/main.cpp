#include <Arduino.h>

const byte interruptPinRed = 2 ;
const byte interruptPinGreen = 3 ;
volatile byte state = LOW ;
const int ledRed = 13;
const int ledGreen = 8;
const int buzzer = 9;
const String unlockCombo = "RRGRGGRR";
const String unlockMessage = "dvd dance your body thin";
const int pitch = 1800;
const int dotLen = 100;
const int dashLen = dotLen * 4;

bool redOn = false;
bool greenOn = false;
String inputCombo = "";
String morse[36][2]={
  {"a",".-"},
  {"b","-..."},
  {"c","-.-."},
  {"d","-.."},
  {"e","."},
  {"f","..-."},
  {"g","--."},
  {"h","...."},
  {"i",".."},
  {"j",".---"},
  {"k","-.-"},
  {"l",".-.."},
  {"m","--"},
  {"n","-."},
  {"o","---"},
  {"p",".--."},
  {"q","--.-"},
  {"r",".-."},
  {"s","..."},
  {"t","-"},
  {"u","..-"},
  {"v","...-"},
  {"w",".--"},
  {"x","-..-"},
  {"y","-.--"},
  {"z","--.."},
  {"0","-----"},
  {"1",".----"},
  {"2","..---"},
  {"3","...--"},
  {"4","....-"},
  {"5","....."},
  {"6","-...."},
  {"7","--..."},
  {"8","---.."},
  {"9","----."}
};

void redChange(){
  redOn=(redOn)?false:true;
  digitalWrite(ledRed,(redOn)?HIGH:LOW);
  if(redOn)inputCombo+="R";
}

void greenChange(){
  greenOn=(greenOn)?false:true;
  digitalWrite(ledGreen,(greenOn)?HIGH:LOW);
  if(greenOn)inputCombo+="G";
}

void morseChar(String m){
  for(int i=0;i<m.length();i++){
    
    if(m[i]=='.'){
      tone(buzzer, pitch);
      delay(dotLen);
    }
    else if(m[i]=='-'){
      tone(buzzer, pitch);
      delay(dashLen);
    } else{
      delay(dashLen*5);
    }

    noTone(buzzer);
    delay(dashLen);
  }
  delay(dashLen*3);
}

void morseMessage(String msg){
  for(int i=0;i<msg.length();i++){
    //plaintext char
    String c =(i<msg.length())?msg.substring(i,i+1):c=msg.substring(i);

    //get morse
    for(int m=0;m<36;m++){
      if(morse[m][0]==c){
        morseChar(morse[m][1]);
      }
    }
  }
}

void solved(){
  digitalWrite(ledRed,HIGH);
  digitalWrite(ledGreen,HIGH);

  morseMessage(unlockMessage);
}

void setup() {
  Serial.begin(115200);
  pinMode(interruptPinRed, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPinRed),redChange,CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPinGreen),greenChange,CHANGE);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // test leds
  digitalWrite(ledRed,HIGH);
  delay(1000);
  digitalWrite(ledRed,LOW);
  digitalWrite(ledGreen,HIGH);
  delay(1000);
  digitalWrite(ledGreen,LOW);

  // test buzzer
  tone(buzzer, pitch);
  delay(500);
  noTone(buzzer);
}

void loop() {
  if(inputCombo==unlockCombo){
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,LOW);
    solved();
    inputCombo="";
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledGreen,HIGH);
    delay(1000);
    digitalWrite(ledRed,LOW);
    digitalWrite(ledGreen,LOW);
  } else if(inputCombo.length()>=unlockCombo.length()){
    for(int i=0;i<10;i++){
      digitalWrite(ledRed,HIGH);
      digitalWrite(ledGreen,HIGH);
      delay(100);
      digitalWrite(ledRed,LOW);
      digitalWrite(ledGreen,LOW);
      delay(100);
    }
    inputCombo="";
  }
}



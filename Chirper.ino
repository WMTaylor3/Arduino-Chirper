#include <SoftwareSerial.h>

int buzzerPin = 8;
int sensorPin = A0;

SoftwareSerial BTSerial(3, 2); // RX | TX
int serialData = 0;

long selectedFrequency = 0;
long selectedDuration = 1000;

long sensorThreshold = 100;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  BTSerial.begin(9600);
  noTone(buzzerPin);
}

void loop() {
  if(BTSerial.available() > 0){
    BTSerial.println("");
    serialData = BTSerial.read();
    if(serialData == 'C'){
      chirp(selectedFrequency, selectedDuration);
    }
    else if(serialData == 'F'){
      setFrequency();
    }
    else if(serialData == 'D'){
      setDuration();
    }
    else if(serialData == 'L'){
      calibrateLDR();
    }
    else if(serialData == 'T'){
      calibrateHard();
    }
    else if(serialData == 'A'){
      automate();
    }
    else if(serialData == 'S'){
      printSettings();
    }
    else{
      BTSerial.println("");
      BTSerial.print("Command Not Accepted");
    }
  }
}

void chirp(long freq, long dur){
  BTSerial.println("Chirping...");
  tone(buzzerPin, freq); 
  delay(dur);
  noTone(buzzerPin);
}

void setFrequency(){
  BTSerial.println("");
  BTSerial.println("Enter New Frequency...");
  String inString = "";
  while(true){
    if(BTSerial.available() > 0){
      int inChar = BTSerial.read();
      if(isDigit(inChar)) {
        inString += (char)inChar;
      }
      else
      {
        break;
      }
    }
  }
  selectedFrequency = inString.toInt();
  BTSerial.println("");
  BTSerial.print("New Frequency: ");
  BTSerial.println(selectedFrequency);
  return;
}

void setDuration(){
  BTSerial.println("");
  BTSerial.println("Enter New Duration...");
  String inString = "";
  while(true){
    if(BTSerial.available() > 0){
      int inChar = BTSerial.read();
      if(isDigit(inChar)) {
        inString += (char)inChar;
      }
      else
      {
        break;
      }
    }
  }
  selectedDuration = inString.toInt();
  BTSerial.println("");
  BTSerial.print("New Duration: ");
  BTSerial.println(selectedDuration);
  return;
}

void calibrateLDR(){
  BTSerial.println("");
  BTSerial.println("Calibrating LDR In Dark State...");
  unsigned long total = 0;
  for(int i=0; i<3000; i++){
    total += analogRead(sensorPin);
    delay(3);
  }
  sensorThreshold = (total/3000)+25;
  BTSerial.println("");
  BTSerial.print("New Threshold: ");
  BTSerial.println(sensorThreshold);
}

void calibrateHard(){
  BTSerial.println("");
  BTSerial.println("Enter New Threshold...");
  String inString = "";
  while(true){
    if(BTSerial.available() > 0){
      int inChar = BTSerial.read();
      if(isDigit(inChar)) {
        inString += (char)inChar;
      }
      else
      {
        break;
      }
    }
  }
  sensorThreshold = inString.toInt();
  BTSerial.print("New Threshold: ");
  BTSerial.println(sensorThreshold);
  return;
}

bool darkEnough(){
  if(analogRead(sensorPin) <= sensorThreshold){
    return true;
  }
  return false;
}

void automate(){
  BTSerial.println("");
  BTSerial.println("Automating...");
  unsigned long startTime = millis();
  unsigned long waitTime = 0;
  while(BTSerial.available() == 0){
    if(millis() >= (startTime + waitTime)){
      if(darkEnough()){
        chirp(0, random(500, 1300));
      }
      startTime = millis();
      waitTime = random(15000, 40000);
      BTSerial.print("Delay: ");
      BTSerial.println(waitTime);
    }
  }
  BTSerial.println("");
  BTSerial.println("Stopping...");
  while(BTSerial.available() > 0){
    int purge = BTSerial.read();
  }
}

void printSettings(){
  BTSerial.print("Frequency: ");
  BTSerial.println(selectedFrequency);
  BTSerial.print("Duration: ");
  BTSerial.println(selectedDuration);
  BTSerial.print("Sensor Threshold: ");
  BTSerial.println(sensorThreshold);
  BTSerial.print("Sensor Level: ");
  BTSerial.println(analogRead(sensorPin));
}

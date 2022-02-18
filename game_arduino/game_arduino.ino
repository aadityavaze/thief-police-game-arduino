#include <Adafruit_CircuitPlayground.h>

#define SENSOR_TIME_INCREMENT  50
#define POLICE_TIME_INCREMENT  500
#define THIEF_TIME_INCREMENT   500
#define NUM_OF_LIGHTS 10
#define VOLUME_THRESHOLD 65
#define DIR_CHANGE_MARGIN_TIME 250

int micPin = A0;

long int thiefCounter = 7, policeCounter = 0;
long int startSensorTime, startPoliceTime, startThiefTime, startDirChangeTime, startRandomizerTime;
float volume;
bool isRecent = true;
int thiefDirection = 1, policeDirection = 1;

int randomTime = random(2000,5000);

//return non-negative modulo
int mod(long int a, int b){
    return (a % b + b) % b;
  }

void togglePoliceDirection(){
      policeDirection = -policeDirection;
      policeCounter+=policeDirection;
    }


void toggleThiefDirection(){
      thiefDirection = -thiefDirection;
      thiefCounter+=thiefDirection;
    }
    

void blinkAllLeds(){
    lightAll(80, 0, 0);
    delay(200); 
    lightAll(0, 0, 80);
    delay(200); 
    lightAll(80, 0, 0);
    delay(200);
    lightAll(0, 0, 80);
    delay(100);
    lightAll(80, 0, 0);
    delay(200);
    lightAll(80, 0, 0);
    delay(200); 
    lightAll(0, 0, 80);
    delay(200); 
    lightAll(80, 0, 0);
    delay(200);
    lightAll(0, 0, 80);
    delay(100);
    lightAll(80, 0, 0);
    delay(200);
    lightAll(0, 0, 0);
  }

//reinitialize positions
void resetPositions(){
    policeCounter = random(0,9);
    thiefCounter = policeCounter + 4;
  }
  
void lightAll(int a, int b, int c) {
  for(int i=0; i<10; i++){
    CircuitPlayground.setPixelColor(i, a,   b,   c);
    }
}

    
void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  
  startSensorTime = millis();
  startPoliceTime = millis();
  startThiefTime = millis();
  startRandomizerTime = millis();
}

void loop() {

   
  //read microphone values
  if(millis() - startSensorTime >= SENSOR_TIME_INCREMENT){
      volume = CircuitPlayground.mic.soundPressureLevel(10);
      startSensorTime = millis();
    }
    

  //move police
  if(millis() - startPoliceTime >= POLICE_TIME_INCREMENT){
    CircuitPlayground.setPixelColor(mod(policeCounter-policeDirection, NUM_OF_LIGHTS), 0, 0, 0);
    CircuitPlayground.setPixelColor(mod(policeCounter, NUM_OF_LIGHTS), 0, 0, 255);

    if(mod(policeCounter, NUM_OF_LIGHTS) == mod(thiefCounter, NUM_OF_LIGHTS)){
        blinkAllLeds();
        resetPositions();
      }
      
    policeCounter+=policeDirection;
    startPoliceTime = millis();
    }

  //move thief
  if(millis() - startThiefTime >= THIEF_TIME_INCREMENT){
    CircuitPlayground.setPixelColor(mod(thiefCounter-thiefDirection, NUM_OF_LIGHTS), 0, 0, 0);
    CircuitPlayground.setPixelColor(mod(thiefCounter, NUM_OF_LIGHTS), 80, 80, 80);

     if(mod(policeCounter, NUM_OF_LIGHTS) == mod(thiefCounter, NUM_OF_LIGHTS)){
        blinkAllLeds();
        resetPositions();
      }

    thiefCounter+=thiefDirection;
    startThiefTime = millis();
    }
    
  //change thief's direction when loud sound detected
  if(volume > VOLUME_THRESHOLD && isRecent){
      Serial.println("Direction change");
      if(isRecent){
          startDirChangeTime = millis();
        }
      isRecent = false;
      toggleThiefDirection();
    }
     if(!isRecent && millis() - startDirChangeTime >= DIR_CHANGE_MARGIN_TIME){
          isRecent = true;
        }


    
    if(millis() - startRandomizerTime >= randomTime){
        togglePoliceDirection();
        randomTime = random(2000,5000);
        startRandomizerTime = millis();
      }

 
}

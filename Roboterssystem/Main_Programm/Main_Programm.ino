#include <Thread.h>
#include <ThreadController.h>
#include <Servo.h>
#include <UltraschallSensor.h>

#define E1 4  // Enable Pin for motor 1
#define E2 5  // Enable Pin for motor 2
 
#define I1 2  // Control pin 1 for motor 1
#define I2 3  // Control pin 2 for motor 1
#define I3 6  // Control pin 1 for motor 2
#define I4 7 // Control pin 2 for motor 2
#define ESPStart 26  //Start and stopp ESP8266
//Servos Motoren
#define RichtungServoPin 8
#define UltraSchallServoPin 9
#define ArmServoPin1 10
#define ArmServoPin2 11
#define ArmServoPin3 11
#define ArmServoPin4 12
#define ArmServoPin5 13

//UltraschallSensor
#define echo 22
#define trigger 24

/*
ThreadController controll = ThreadController();
Thread fahrzeugControll = Thread();
Thread armControll = Thread();
Thread sensorControll = Thread();
*/

//Servos object
Servo RichtungServo;
Servo UltraSchallServo;
Servo ArmServo1;
Servo ArmServo2;
Servo ArmServo3;
Servo ArmServo4;

//UltraschallSensor object
UltraschallSensor sensor(trigger, echo);

bool befehlEmpfang = false;
int numberOfBefehl = 0;
String rohBefehlList; 
String befehlList[100];


void setup() {
  Serial.begin(115200);
  Serial3.begin(38400);
  
  //DC Motors Steuerungspins
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);
  pinMode(I3, OUTPUT);
  pinMode(I4, OUTPUT);
  pinMode(ESPStart, OUTPUT);
  

  //Set DC Motor Speed
  analogWrite(E1, 255); // Run in full speed
  analogWrite(E2, 255); // Run in full speed

  //analogWrite(E1, 153); // Run in half speed
  //analogWrite(E2, 153); // Run in half speed
  
  //Servos Steuerungspins
  //RichtungServo.attach(RichtungServoPin);
  //UltraSchallServo.attach(UltraSchallServoPin);
  /*ArmServo1.attach(ArmServoPin1);
  ArmServo2.attach(ArmServoPin2);
  ArmServo3.attach(ArmServoPin3);
  ArmServo4.attach(ArmServoPin4);*/
  //RichtungServo.write(30);
  //Sensor Init
  //sensor.setMode();

  //Thread attach method
  //sensorControll.onRun(readSendUltraschall);
  //sensorControll.setInterval(50);
  
  //add Thread to control
  //controll.add(&sensorControll);
  //RichtungServo.write(20);
  Serial.println("Setup");
}

void loop() {
  
  Serial.println(Serial3.readStringUntil(' '));
  if (befehlEmpfang){
    initRobot();
    do{
      Serial3.find('[');
      rohBefehlList = Serial3.readStringUntil(']');
    }while(rohBefehlList.length() < 5);
    Serial.println(rohBefehlList);
    /*splitString(rohBefehlList, ';');
    if(numberOfBefehl > 2){
      //controll.run();
      befehlEmpfang = false;
      Serial.print("Anzahl von Befehle: "); 
      Serial.println(numberOfBefehl);
      Serial.println("Befehle:");
      for(int t = 0; t < numberOfBefehl; t++){
        Serial.println(befehlList[t]);
        befehlAusfueren(befehlList[t]);
        if (t == numberOfBefehl - 1){
          initRobot();
          break;
        }
      }*/
  } 
  //BI:60;
   splitString("[FW:2000;RW:2000;M1:20;M2:35;M4:80;End]", ';');
   for(int t = 0; t < numberOfBefehl; t++){
        Serial.println(befehlList[t]);
        befehlAusfueren(befehlList[t]);
        if (t == numberOfBefehl - 1){
          //initRobot();
          break;
        }
      }
}

void initRobot(){
  numberOfBefehl = 0;
  befehlEmpfang = true;
  digitalWrite(ESPStart, LOW);
  digitalWrite(ESPStart, HIGH);
}

void readSendUltraschall(){
  int Beta = 0;
  Serial.print("Thread");
  for(int i = 0; i < 180; i++){
    UltraSchallServo.write(i);
    if((i%30) == 0){
      Serial3.print(i);
      Serial3.print(";");
      Serial3.println(sensor.getEntfernung());
      delay(50);
      Serial.println(sensor.getEntfernung());
    }
  }
    
  for(int i = 180; i > 0; i--){
    UltraSchallServo.write(i);
    if((i%30) == 0){
      Serial3.print(i);
      Serial3.print(";");
      Serial3.println(sensor.getEntfernung());
      delay(50);
      Serial.println(sensor.getEntfernung());
    }
  }
}

void befehlAusfueren(String befehl){
  if (befehl.length() > 3){
    String befehlCode = befehl.substring(0, 2);
    //Serial.print(" Befehl Code: ");
    //Serial.print(befehlCode);
    String befehlValue = befehl.substring(3, befehl.length());
    //Serial.print(" Value: ");
    //Serial.println(befehlValue);
    
    if (befehlCode == "FW"){
      Serial.print("FW-Value: ");
      Serial.println(befehlValue.toInt());
      //RichtungServo.write(10);
      analogWrite(E1, 255); // Run in full speed
      analogWrite(E2, 255); // Run in full speed
      digitalWrite(I1, HIGH);
      digitalWrite(I2, LOW);
      digitalWrite(I3, HIGH);
      digitalWrite(I4, LOW);

      delay(befehlValue.toInt());
      digitalWrite(E1, LOW);
      digitalWrite(E2, LOW);
    }
    else if(befehlCode == "RW"){
      Serial.print("RW-Value: ");
      Serial.println(befehlValue.toInt());
      //RichtungServo.write(10);
      analogWrite(E1, 255); // Run in full speed
      analogWrite(E2, 255); // Run in full speed
      digitalWrite(I1, LOW);
      digitalWrite(I2, HIGH);
      digitalWrite(I3, LOW);
      digitalWrite(I4, HIGH);
      delay(befehlValue.toInt());
      digitalWrite(E1, LOW);
      digitalWrite(E2, LOW);
    }
    else if(befehlCode == "BI"){
      Serial.print("BI-Value: ");
      Serial.println(befehlValue.toInt());
      //RichtungServo.write(befehlValue.toInt());
      delay(500);
      analogWrite(E1, 255); // Run in full speed
      analogWrite(E2, 255); // Run in full speed
      digitalWrite(I1, HIGH);
      digitalWrite(I2, LOW);
      digitalWrite(I3, HIGH);
      digitalWrite(I4, LOW);
      delay(5000);
      digitalWrite(E1, LOW);
      digitalWrite(E2, LOW);
      //RichtungServo.write(20);
      delay(500);
    }
    else if(befehlCode == "M1"){
      //ArmServo1.write(befehlValue.toInt());
    }
    else if(befehlCode == "M2"){
      //ArmServo2.write(befehlValue.toInt());
    }
    else if(befehlCode == "M3"){
      //ArmServo3.write(befehlValue.toInt());
    }
    else if(befehlCode == "M4"){
      //ArmServo4.write(befehlValue.toInt());
    }
    else{
      Serial.println("Befehl nicht erkannt!");
    }
  } 
}
void splitString(String befehle, char separator){
  int anzahlBefehl = 0;
  for (int i = 0; i < befehle.length(); i++){
    if (befehle[i] == separator){
      anzahlBefehl++;
    }
  }
  numberOfBefehl = anzahlBefehl;
  int j = 0, k = 0;
  for (int i = 0; i < befehle.length(); i++){
    if (befehle[i] == separator){
      befehlList[k] = befehle.substring(j + 1, i );
      //Serial.println(befehlList[k]);
      k++;
      j = i;
    }
    if (i == befehle.length() - 1){
      befehlList[k] = befehle.substring(j - 1, i - 1);
      //Serial.println(befehlList[k]);
    }
  }
}


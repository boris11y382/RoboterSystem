/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <ESPWIFI.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro(0x68);

uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)

int16_t ax, ay, az;
int16_t gx, gy, gz;

//char* ssid  = "WLAN-B83332"; 
//char* key = "4572623204171461"; 

//char* ssid  = "BFK-Network";
//char* key = "9GQ1382TSMA7HUB6";

char* ssid  = "HTC Portable Hotspot 2171";
char* key = "6f434b51d552";

int i = 0;
unsigned int udpPort = 4210;
ESPWIFI wifiController(udpPort);
unsigned long timer = 0;
float timeStep = 0.01;
void setup(){
  Serial.begin(38400);
  
  /*String connectData = "";
  
  //Serial.println("Init");
  //Serial.println("WLAN List: ");
  //Serial.println(wifiController.getWifiList());
  do{
    Serial.find('[');
    connectData = Serial.readStringUntil(']');
  }while(connectData.length() < 15);
  int i;
  for(i=0; i<connectData.length(); i++){
    if (connectData[i]==';'){
      break;
    }
  }
  connectData.substring(0, i ).toCharArray(ssid, connectData.length());
  connectData.substring(i+1, connectData.length()).toCharArray(key, connectData.length());
*/
  Serial.print("SSID:");
  Serial.println(ssid);
  Serial.print("key:");
  Serial.println(key);
  
  if(wifiController.tryConnect(ssid, key)){
    Serial.println("WiFi Data");
    Serial.println(wifiController.getWifiData());
  }
  wifiController.UDPStarten();
  
  Serial.println("Reading: ");
  String receiveData = wifiController.UDPReceiveData();
  while(true){
    receiveData = wifiController.UDPReceiveData();
    if (receiveData.length() > 1){
      Serial.print("[");
      Serial.print(receiveData);
      Serial.println("]");
      break;
    }
    
  }
  Wire.begin(0, 2);
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  devStatus = accelgyro.dmpInitialize();
  
  accelgyro.setDLPFMode(0x04);
  accelgyro.setFullScaleGyroRange(0x01);  // Set gyro full scale range to +/-500 degrees/s
  accelgyro.setFullScaleAccelRange(0x01); // Set accelerometer full scale range to +/-4g
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    accelgyro.setDMPEnabled(true);
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
  Serial.println("Sending data...");
}

void loop(){
  
  timer = millis();
  
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
   String data = "";
   data +=  ax;
   data +=  ";";
   data +=  ay;
   data +=  ";";
   data +=  az;
   data +=  ",";
   data +=  gx;
   data +=  ";";
   data +=  gy;
   data +=  ";";
   data +=  gz;
   data +=  ",";
   data +=  timeStep;
   data +=  "/";
   data +=  "3000.4;35.23";
  //Serial.println("Data is being sending...");
  wifiController.UDPSendData(data);
  //Serial.println(data);
  timeStep =  millis() - timer;
  //Serial.print("Time : ");
  //Serial.println(timeStep);
}



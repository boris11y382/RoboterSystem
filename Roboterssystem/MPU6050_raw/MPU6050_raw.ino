

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

//#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro(0x68);
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)

int16_t ax, ay, az;
int16_t gx, gy, gz;

double axc, ayc, azc;
double dax, dAy, daz;
double dgx, dgy, dgz;

double x = 0, y = 0, z = 0;
double vx = 0, vy = 0, vz = 0;
// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO

double timer = 0;
double timeStep = 0;

// Pitch, Roll and Yaw Angle values
double pitch = 0;
double roll = 0;
double yaw = 0;
double RDA = 39.24/32773;
double RDB = -39.24;

void setup() {
  
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
  
    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    //Serial1.begin(38400);
    Serial.begin(115200);
  
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = accelgyro.dmpInitialize();
  
    accelgyro.setDLPFMode(0x04);
    accelgyro.setFullScaleGyroRange(0x01);  // Set gyro full scale range to +/-500 degrees/s
    accelgyro.setFullScaleAccelRange(0x01); // Set accelerometer full scale range to +/-4g
    // supply your own gyro offsets here, scaled for min sensitivity
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    accelgyro.setZAccelOffset(1788); // 1688 factory default for my test chip

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
      
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    dax = RDA * ax + RDB, dAy = RDA * ay + RDB, daz = RDA * az + RDB;
}

void loop() {

    timer = millis();
   
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    String data = "('";
           data +=  ax;
           data +=  ";";
           data +=  ay;
           data +=  ";";
           data +=  az;
           data +=  "','";
           data +=  gx;
           data +=  ";";
           data +=  gy;
           data +=  ";";
           data +=  gz;
           data +=  "'),";
    Serial.print("RohData : ");          
    Serial.println(data); 
    //Serial1.println(data);
    //Serial.println(Serial1.readStringUntil('\n'));
    timeStep = (millis() - timer) / 1000;
    axc = RDA * ax + RDB;
    ayc = RDA * ay + RDB;
    azc = RDA * az + RDB;

    x = -0.5 *timeStep*timeStep* (axc - dax) + vx * timeStep + x;
    y = -0.5 *timeStep*timeStep* (ayc - dAy) + vy * timeStep + y;
    z = -0.5 *timeStep*timeStep* (azc - daz) + vz * timeStep + z;

    vx = timeStep* (axc - dax) + vx;
    vy = timeStep* (ayc - dAy) + vy;
    vz = timeStep* (azc - daz) + vz;
    String pos = "('";
               pos +=  x;
               pos +=  ";";
               pos +=  y;
               pos +=  ";";
               pos +=  z;
               pos +=  "')";
    
    String geschw = "('";
               geschw +=  vx;
               geschw +=  ";";
               geschw +=  vy;
               geschw +=  ";";
               geschw +=  vz;
               geschw +=  "')";
     Serial.print("Position : ");    
     Serial.println(pos); 
     Serial.print("Geschwindigkeit : ");  
     Serial.println(geschw); 
     Serial.print("Time : ");  
     Serial.println(timeStep, 10); 
     
    dax = axc, dAy = ayc, daz = azc;
}

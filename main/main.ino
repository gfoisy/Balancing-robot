#include <Wire.h>
#include "functionlist.h"


    #define EnA 10
    #define EnB 5
    #define In1 9
    #define In2 8
    #define In3 7
    #define In4 6

const int GyroChip=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ,t1,t2;
float GyroscopeScale, AccelerometerScale, GyXdegrees,GyYdegrees, runningaverage, angleChange, frequency, pitch, roll, pitchOffset, rollOffset;
float ax, ay, az,AcPitch,AcRoll, PID, PIDA, PIDB, P,D, ScalingA, ScalingB;
long error[5];
int i;


float PIDFREE, GyYnoRoll;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Wire.begin();
  GyroChipSetup();
  motorSetup();



  frequency=25; //hz
  t2=0;         //setting time to zero


 //troubleshooting
 GyYnoRoll=0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  timing();
  gatherData();
  getAngles();
  calculatePitchRoll();
  PIDValue();
  motorControl();      
        
                                                                      


/////   Troubleshooting and calibration //////
//  getAnglesTest();
//  calculatePitchRollTest();
   
// calibrateTheOffset(AcZ,AccelerometerScale);
// calibrateTheOffset(roll,1);

  
//   Serial.print("pitch:"); Serial.print(pitch); Serial.print("\t");
//   Serial.print("pitchAccel"); Serial.print(AcYdegrees);  Serial.print("\t");
//   Serial.print("no filter"); Serial.print(GyYnoPitch);  Serial.print("\t");
//    Serial.print("pitchNeg"); Serial.println(pitchNeg);
    
  
//  Serial.print("pitch:"); Serial.print(pitch); Serial.print("\t");
//  Serial.print("roll:"); Serial.print(roll);Serial.print("\t"); Serial.print("error:"); Serial.print(error[0]); Serial.println("\t");
//  Serial.print("gyY:"); Serial.print(GyYnoRoll); Serial.print("\t");
//   Serial.print("ARollY:"); Serial.println(AcRoll);  
  

  //https://diyrobocars.com/2020/05/04/arduino-serial-plotter-the-missing-manual/ how to serial plot better
  

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void timing(){ 
      //timing the loop so it is going a known frequency.
   
  t1=millis();
    while ((t1-t2)<(1/frequency)*1000){
      delay(1);
//        Serial.print("delay \n"); //checking to see if working
      t1=millis();
    }
   t2=t1;
 }

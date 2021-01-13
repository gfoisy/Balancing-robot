#include <Wire.h>

#include "functionlist.h"

//X==ROLL
//Y==PITCH
//Z==YAW

void GyroChipSetup(){ /////////////////////////////////////////////////////////////////////////////
  Wire.beginTransmission(GyroChip);
  Wire.write(0x6B);
  Wire.write(0x1); // hex for 000001 (should there be two more zeros? I am not sure)
  Wire.endTransmission(true);

  Wire.beginTransmission(GyroChip);
  Wire.write(0x1B);
  Wire.write(0); //setting to 250 deg/s
  Wire.endTransmission(false);
  Wire.write(0x1C);
  Wire.write(0); //setting it to 2gs
  Wire.endTransmission(true);

  //initializing and zeroing

  pitch=0;
  roll=0;

  //pitchOffset; //the offset from global zero and the balancing point of the robot
  rollOffset=0; 
  
  GyroscopeScale=131;  //131 for +-250 deg/s    65.5 for +-500 deg/s
  AccelerometerScale=16384;  //16384 for +-2g    8192 for +-4g

  
  

  // variables for "calibrateTheOffset(float angleChange)"
  runningaverage=0;
  i=0;
}


void gatherData(){ /////////////////////////////////////////////////////////////////////////////
  Wire.beginTransmission(GyroChip);
  Wire.write(0x3b);  //3b is where accel data starts
  Wire.endTransmission(false);
  Wire.requestFrom(GyroChip,14, true);

  //Acceleration data
      AcX=Wire.read()<<8|Wire.read();   // 0x3B (ACCEL_XOUT_H) 0x3C (ACCEL_XOUT_L)       // reads the first 8 bits of data, then shifts them down (<<8) 8 bits. Then it copys in the second 8 bits of data with the or (|) operator
      AcY=Wire.read()<<8|Wire.read();   // ACCEL_YOUT_H and ACCEL_YOUT_L
      AcZ=Wire.read()<<8|Wire.read();   // ectera 

  //Temperature data//
      Tmp=Wire.read()<<8|Wire.read();   //
    
  //Gyroscope data
      GyX=Wire.read();
      GyX=GyX<<8;
      GyX=GyX|Wire.read();
      GyY=Wire.read()<<8|Wire.read();   //
      GyZ=Wire.read()<<8|Wire.read();   //
      
   Wire.endTransmission(true);
}






void getAngles(){ /////////////////////////////////////////////////////////////////////////////
  roll=  roll+ (((float)GyY/GyroscopeScale+1.4)/frequency);         
  pitch= pitch+(((float)GyX/GyroscopeScale+3.5)/frequency);      //should it be GyYdegrees+stuff OR PITCH+Stuff, ya know? where to feed taht data back in
  //yaw= yaw+(((float)GyZ/GyroscopeScale+0)/frequency);        

  GyYnoRoll=GyYnoRoll+(((float)GyY/GyroscopeScale+1.4)/frequency);
 
  ax= (float)(AcX)/AccelerometerScale-.03;
  ay= (float)(AcY)/AccelerometerScale+0.01;
  az= (float)(AcZ)/AccelerometerScale+.08; //should calibrate to 1g

  
  AcRoll= (180/3.1415926)*atan2(-ax,az);                   // roll calculation
  AcPitch= (180/3.1415926)*atan(ay/sqrt((ax*ax)+(az*az)));   // pitch calculation

}


void calculatePitchRoll(){//////////////////////////////////////////////////////////////////////
     roll= AcRoll*.05+(roll*.95); //roll calculation, gyroscope data 95% filtered with 5% accel data
     pitch= AcPitch*.05+(pitch*.95); //pitch calculation

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Troubleshooting///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void getAnglesTest(){ /////////////////////////////////////////////////////////////////////////////
//  GyXdegrees=roll+(((float)GyX/GyroscopeScale+.53)/frequency);         
//  GyYdegrees=pitch-(((float)GyY/GyroscopeScale+.15)/frequency);      //should it be GyYdegrees+stuff OR PITCH+Stuff, ya know? where to feed taht data back in
//  
//  //GyZdegrees=Yaw+(((float)GyZ/GyroscopeScale+0)/frequency);        // very slow to respond, issue may not be here
//
// 
//
//  ax= (float)(AcX)/AccelerometerScale-.07;
//  ay= (float)(AcY)/AccelerometerScale+0.01;
//  az= (float)(AcZ)/AccelerometerScale+0;
//
//  
//  AcXdegrees=(180/3.1415926)* atan2(-ay,az);                   // roll calculation
//  AcYdegrees=(180/3.1415926)*atan(ay/sqrt((ax*ax)+(az*az)));   // pitch calculation
// 
}



void calculatePitchRollTest(){ ////////////////////////////////////////////////////////////////////////
//     roll=AcXdegrees*.05+(GyXdegrees*.95); //roll calculation (in radians), gyroscope data 95% filtered with 5% accel data
//     pitch= AcYdegrees*.05+GyYdegrees*.95; //pitch calculation
     
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Calibration///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void calibrateTheOffset(float angleChange, float factor){
  //This only works when the gyro data is NOT BEING INTEGRATED OVER TIME
      //angleChange=angleChange;
      
      if(i>99){
        Serial.print("\nTHIS IS IT \n");
        Serial.print("THIS IS IT \n");
        Serial.print("THIS IS IT \n");
        Serial.print("THIS IS IT \n");
    
         Serial.print("offset from Tested angle \n");     
         Serial.print(runningaverage/100/factor);

         
    
        Serial.print("\nThat was IT \n");
        Serial.print("That was IT \n");
        Serial.print("That was IT \n");
        delay(10000);
        i=0;
        runningaverage=0;
               }
  i++;
  Serial.print("         skew in angleChange scaled by factor ");Serial.print(factor);Serial.print("    "); Serial.print(angleChange);Serial.print("\n");
  runningaverage=angleChange+runningaverage;                         
}

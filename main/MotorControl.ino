#include "functionlist.h"


//////////////////////////////////////////////////////////////////////////////////////////////////
  void motorSetup(){
      pinMode(EnA, OUTPUT);
      pinMode(EnB, OUTPUT);
      pinMode(In1, OUTPUT);
      pinMode(In2, OUTPUT);
      pinMode(In3, OUTPUT);
      pinMode(In4, OUTPUT);
      
      pinMode(2, OUTPUT); // 5v power supply for motor driver
      digitalWrite(2,HIGH); //powering on the motor driver

       //pitchOffset and rollOffset are to offset the pitch and roll values from their true GCS values to be zero at the balance point of the robot
      rollOffset=.5; //2.4
      //    PitchOffset

        ScalingA=1;    //amount to scale the motor's reaction by
        ScalingB=1.2;  //amount to scale the motor's reaction by, B is a weaker motor, compensate for it more
        P=70;          //proportional gain
        D=5;          //differential gain
  }


///////////////////////////////////////////////////////////////////////////////////////////////////////
  void PIDValue(){   // expects Pitch/Roll between 25 and -25 
      
      for(byte i = 4; i > 0 ; i--){
        error[i] = error[i-1];
      }
      error[0] = (roll+rollOffset); //pitchOffset and rollOffset are to offset the pitch and roll values from their true GCS values to be zero at the balance point of the robot
      
      PID = error[0]*P;
      PID -= (error[0]-error[1])*D;
    //  PID+=
      PIDA=PID*ScalingA;
      PIDB=PID*ScalingB;
      PIDFREE=PID;
    
    
    //if pid value is too large or small, truncate it
      if (PID>250){
        PID=250;
      }
      if (PID<-250){
        PID=-250;
      }
//            Serial.print("PID:");Serial.print(PID); Serial.print("\n ");
//            Serial.print("PIDFree:");Serial.println(PIDFREE);
  }

//Motor conrtol////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void motorControl(){
        if(PID>0){
            digitalWrite(In1, LOW); 
            digitalWrite(In2, HIGH);
            analogWrite(EnA,PID); // Setting speed, lower is faster?
            
            digitalWrite(In3, HIGH); 
            digitalWrite(In4, LOW);
            analogWrite(EnB,PID); // Setting speed, lower is faster?
            
        }else if(PID<0){
             PID=abs(PID);
             
             digitalWrite(In1, HIGH); 
             digitalWrite(In2, LOW);
             analogWrite(EnA,PID); 

             digitalWrite(In3, LOW); 
             digitalWrite(In4, HIGH);
             analogWrite(EnB,PID); 
        }
  }

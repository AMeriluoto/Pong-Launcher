#include <Servo.h>
#include <stdlib.h>




/*Pin constants*/
const int DIR_A= 12;
const int DIR_B = 13;
const int BRAKE_A = 9;
const int BRAKE_B = 8;
const int GAS_A = 3;
const int GAS_B = 11;

/*Contains angles*/
int launch_angles[4]  = {0,25,85,135};

/*Contains the time in ms for the motors to wind up the arm. Variable times create variable power.*/
int power[3] = {0,1300,1800}; //contains the lengths of time in ms to wind up the arm for two different settings


Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo1.attach(10);
  servo2.attach(5);
  servo3.attach(7);
  


  pinMode(BRAKE_B,OUTPUT);
  pinMode(DIR_B,OUTPUT);
  
 
  pinMode(DIR_A, OUTPUT);
  pinMode(BRAKE_A, OUTPUT);
  

}



/*An instance of class Mode helps create variable functionality for the launcher*/
class Mode{
  public:
  bool isRotating;
  bool randomPower;
  int  loadTimer;
  int randomAngle;
  
  

  /*Creates a Mode for the launcher to run on. A mode determines if the launcher will rotate or launch with random power. */
  
  Mode(bool rotating, bool rPower,int index){
    isRotating = rotating;
    randomPower = rPower;
    
    if(!randomPower)
      loadTimer = power[index];
    else
      loadTimer = power[rand()%2+1];
  }

  /*Launch ball with variable power*/
  void launch(){
    servo1.write(0);
    analogWrite(GAS_B, 255);  //Spins the motor on Channels A and B at full speed 
    analogWrite(GAS_A,255);
    delay(loadTimer);
    servo1.write(90);//Disengages clutch
    analogWrite(GAS_A,0);//Stops motors
    analogWrite(GAS_B,0);
    delay(1000);
    servo2.write(25);//Lifts trigger pin
  }

  /*Rotate launcher if Mode calls for rotation*/
  void rotate(){
    if(!isRotating){
      return;
  }
    randomAngle = launch_angles[rand()%3+1];
    Serial.println(randomAngle);
    servo3.write(randomAngle);//Angles launcher to one of three randomized angles
    
  }

  /*Reset launcher to original state. PRECONDITION: Launcher has to be in a launched state.*/
  void reset(){
    delay(50);
    servo1.write(0);
    delay(500);
    servo2.write(45);//Resets trigger pin
    servo3.write(8);//Resets launcher to default angle
    delay(1500); 
    }

  
  

   
 };
 /*Set the motors to their intended states*/
 void resetMotors(){
  digitalWrite(DIR_B, LOW);//Establishes forward direction of Channels A and B
  digitalWrite(DIR_A,LOW);
  digitalWrite(BRAKE_B, LOW);//Disengages brakes on motors A and B
  digitalWrite(BRAKE_A,LOW);
  }
 

Mode* mode1 = new Mode(false,false,2);



void loop() {
  // put your main code here, to run repeatedly:
  
  resetMotors();
  mode1->launch();
  mode1->rotate();
  mode1->reset();
  
}

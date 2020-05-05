#include <PID_v1.h>
#define MotEnable 6 
#define MotFwd  4 
#define MotRev  7

//# define home_status 9

String readString; 
int User_Input = 0; 
int encoderPin1 = 2; 
int encoderPin2 = 3; 
   volatile int lastEncoded = 0; 
   volatile long encoderValue = 0; 
int PPR = 40000;  // Encoder Pulse per revolution.
int angle = 360; 

int REV = 0;          
int lastMSB = 0;
int lastLSB = 0;
double kp = 2 , ki = 1 , kd = 0.01;                       
double input = 0, output = 0, setpoint = 0;

PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);  

void setup() {
  pinMode(MotEnable, OUTPUT);
  pinMode(MotFwd, OUTPUT); 
  pinMode(MotRev, OUTPUT); 
  
  Serial.begin(9600); 

   pinMode(encoderPin1, INPUT_PULLUP); 
  pinMode(encoderPin2, INPUT_PULLUP);

  digitalWrite(encoderPin1, HIGH); 
  digitalWrite(encoderPin2, HIGH); 
  
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

    
 TCCR1B = TCCR1B & 0b1111100 | 1;  
  myPID.SetMode(AUTOMATIC);   
  myPID.SetSampleTime(1);  
  myPID.SetOutputLimits(-220, 220); // this is the MAX PWM value to move motor, here change in value reflect change in speed of motor.
}

void loop() {

 while (Serial.available()) { 
    delay(3);                  
    char c = Serial.read();  
    readString += c;         
  }
 
  if (readString.length() >0) { 
  
   //Serial.println(readString.toInt());  
    User_Input = readString.toInt();   
}
  

REV = map (  270, 0, 360, 0, 40000);
//Serial.print("this is Set Value "); 
//Serial.println(REV);               

setpoint = REV;                    
  input = encoderValue ;           
 //Serial.print("encoderValue ");
 //Serial.println(encoderValue);
Serial.print(" ");
Serial.println(output);

  myPID.Compute();                 
  pwmOut(output);
  
 
 
}
void pwmOut(int out) {                               
  if (out > 0) {                          
    analogWrite(MotEnable, out);         
    forward();                          
  }
  else {
    analogWrite(MotEnable, abs(out));                               
    reverse();                            
  }

  
 readString="";
}
void updateEncoder(){
  int MSB = digitalRead(encoderPin1); 
  int LSB = digitalRead(encoderPin2); 

  int encoded = (MSB << 1) |LSB; 
  int sum  = (lastEncoded << 2) | encoded; 

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; 

}

void forward () {
  digitalWrite(MotFwd, HIGH); 
 digitalWrite(MotRev, LOW); 
  
}

void reverse () {
  digitalWrite(MotFwd, LOW); 
 digitalWrite(MotRev, HIGH); 
  
}
void finish () {
  digitalWrite(MotFwd, LOW); 
 digitalWrite(MotRev, LOW); 
  
}

#include <AFMotor.h>
#include <EEPROM.h>
#define bt_serial Serial1
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command = ' ';
char mapping = 'N';

byte seq = 0;
int fwd_Counter = -1;
int lft_Counter = -1;
int rgt_Counter = -1;
int bwd_Counter = -1;
int stp_Counter = -1;

unsigned long int current_Time0 = 0;
unsigned long int current_Time1 = 0;
unsigned long int current_Time2 = 0;
unsigned long int current_Time3 = 0;
unsigned long int current_Time4 = 0;

unsigned long int total_Fwd_Time[10];
unsigned long int total_Lft_Time[10];
unsigned long int total_Rgt_Time[10];
unsigned long int total_Bwd_Time[10];
unsigned long int total_Stp_Time[10];

void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(9600);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  EEPROM.read(0) != 0 ? Saved_mapping() : movement();  
}

void loop()
{  
  if(bt_serial.available())
  {
    command = bt_serial.read();
    Serial.println(command);
    if(command != 'D')
    {
      command == 'M' ? mapping = 'Y' : command == 'E' ? mapping = 'N': command == 'A' ? mapping = 'C': command == 'C' ? mapping = 'S' : command == 'L' ? mapping = 'Q' : ' ';
      mapping == 'C' ? Clear_mapping() : mapping == 'Y' ? Mapping() : mapping == 'S' ? Start_cleaning() : mapping == 'Q' ? Stop_cleaning() : movement();
    }
    else
    {
      Disconnected();
    }
  }
}

void movement()
{
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  switch(command){
    case 'F':  
      forward();
      break;
    case 'B':  
       backward();
      break;
    case 'L':  
      left();
      break;
    case 'R':
      right();
      break;
    case 'S':
      Stop();
    }
}
void forward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void left()
{
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD); 
}

void right()
{
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
} 

void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void Disconnected()
{
  Stop();
}

void Mapping()
{
  
}

void Start_cleaning()
{
  
}

void Stop_cleaning()
{
  
}

void Saved_mapping()
{
  Serial.print("MAY NAKA SAVED NA ROOM MAPPING");
   //send data in bt module
} 

void Clear_mapping()
{
  for (int i = 0 ; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}

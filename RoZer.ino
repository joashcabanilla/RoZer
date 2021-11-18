#include <AFMotor.h>
#include <EEPROM.h>
#define bt_serial Serial1
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command = ' ';
char mapping = 'N';
int movement_num;
int address;

void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(9600);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  EEPROM.read(0) != 0 ? Saved_mapping() : movement();  
  address = 0;
}

void loop()
{  
  if(bt_serial.available())
  {
    command = bt_serial.read();
    Serial.println(command);
    if(command != 'D')
    {
      command == 'W' ? mapping = 'Y' : command == 'w' ? mapping = 'N': command == 'X' ? mapping = 'C': command == 'V' ? mapping = 'S' : ' ';
      mapping == 'C' ? Clear_mapping() : mapping == 'Y' ? Mapping() : mapping == 'S' ? Start_cleaning() : movement();
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
  Stop();
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
    }
}
void forward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD); 
  movement_num = 1;
}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  movement_num = 2;
}

void left()
{
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD); 
  movement_num = 3;
}

void right()
{
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  movement_num = 4;
} 

void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  movement_num = 5;
}

void Disconnected()
{
  Stop();
}

void Mapping()
{
  movement();
  if(movement_num != 5)
  {
    EEPROM.write(address,movement_num);
    address++;
  }
}

void Start_cleaning()
{
   for (int i = 0 ; i < EEPROM.length() ; i++) {
    switch(EEPROM.read(i))
    {
      case 1: 
        forward();
        break;
        
      case 2: 
        backward();
        break;
        
      case 3: 
        left();
        break;
        
      case 4: 
        right();
        break;
    }
  }
}

void Saved_mapping()
{
  Serial.print("MAY NAKA SAVED NA ROOM MAPPING");
   //send data in bt module
} 
void Clear_mapping()
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

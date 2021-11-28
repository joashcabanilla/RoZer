#include <AFMotor.h>
#include <EEPROM.h>
#define bt_serial Serial1
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command = ' ';
char mapping = ' ';

int seq = 0;
byte seq_movement = 0;
float delay_time = 0.0000f;
float current_time;
float stop_time;

void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(9600);

  //TESTING CLEAR MAPPING-----------------------------------------------------------------------
  for (int i = 0 ; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  //----------------------------------------------------------------------------------------------
  
  EEPROM.read(0) != 0 ? Saved_mapping() : movement();  
}

void loop()
{  
  if(bt_serial.available())
  {
    command = bt_serial.read();
    //Serial.println(command);
    if(command != 'D')
    {
      command == 'M' ? mapping = 'Y' : command == 'E' ? mapping = 'N': command == 'A' ? mapping = 'C': command == 'C' ? mapping = 'S' : command == 'T' ? mapping = 'Q' : ' ';
      mapping == 'C' ? Clear_mapping() : mapping == 'Y' ? Mapping() : mapping == 'S' ? Start_cleaning() : mapping == 'Q' ? Stop_cleaning() : mapping == 'N' ? save_mapping(): movement();
    }
    else
    {
      Disconnected();
    }
  }
}

void movement()
{
  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);
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
  Serial.println(command);
  switch(command){
    case 'F':  
      forward();
      seq_movement = 1;
      current_time = millis();
      Serial.println(current_time, 4);
      break;
    case 'B':  
       backward();
       seq_movement = 2;
      break;
    case 'L':  
      left();
      seq_movement = 3;
      break;
    case 'R':
      right();
      seq_movement = 4;
      break;
    case 'S':
      Stop();
      stop_time = millis();
      Serial.println(stop_time, 4);
      delay_time = (stop_time - current_time) / 1000;
      Serial.println(delay_time, 4);
      EEPROM.write(0,delay_time);
      Serial.println(EEPROM.read(0),4);
      break;
  }
}

void Start_cleaning()
{
}

void Stop_cleaning()
{
  mapping = ' ';
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

void save_mapping()
{ 
  
}

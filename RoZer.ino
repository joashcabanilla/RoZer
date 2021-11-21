#include <AFMotor.h>
#include <EEPROM.h>
#define bt_serial Serial1
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command = ' ';
char mapping = 'N';

int seq = 0;
byte seq_movement = 0;
unsigned long int current_time = 0;
unsigned long int current_time1 = 0;
unsigned long int current_time2 = 0;
unsigned long int current_time3 = 0;
unsigned long int current_time4 = 0;
unsigned long int current_time5 = 0;
byte delay_time1 = 0;
byte delay_time2 = 0;
byte delay_time3 = 0;
byte delay_time4 = 0;
byte delay_time5 = 0;
void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(9600);
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
      command == 'M' ? mapping = 'Y' : command == 'E' ? mapping = 'N': command == 'A' ? mapping = 'C': command == 'C' ? mapping = 'S' : command == 'T' ? mapping = 'Q' : ' ';
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
  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);
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
  seq_movement = 1;
}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  seq_movement = 2;
}

void left()
{
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
  seq_movement = 3;
}

void right()
{
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  seq_movement = 4;
} 

void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  seq_movement = 5;
}

void Disconnected()
{
  Stop();
}

void Mapping()
{
  movement();
  current_time = millis();
  Serial.println(current_time);
  switch(seq_movement){
      case 1: //forward
        current_time1 = millis();
        Serial.println(current_time1);
        delay_time1 = (current_time - current_time1) / 1000;
        Serial.println(delay_time1);
        break;
      case 2: //backward
        current_time2 = millis();
        Serial.println(current_time2);
        delay_time2 = (current_time - current_time2) / 1000;
        Serial.println(delay_time2);
        break;
      case 3: //left 
        current_time3 = millis();
        Serial.println(current_time3);
        delay_time3 = (current_time - current_time3) / 1000;
        Serial.println(delay_time3);
        break;
      case 4: //right
        current_time4 = millis();
        Serial.println(current_time4);
        delay_time4 = (current_time - current_time4) / 1000;
        Serial.println(delay_time4);
        break;
      case 5: //stop
        current_time5 = millis();
        Serial.println(current_time5);
        delay_time5 = (current_time - current_time5) / 1000;
        Serial.println(delay_time5);
        break;
    }
  
}

void Start_cleaning()
{
  
}

void Stop_cleaning()
{
   mapping = 'N';
}

void Saved_mapping()
{
  Serial.print("MAY NAKA SAVED NA ROOM MAPPING");
   //send data in bt module
} 

void Clear_mapping()
{
  mapping = 'N';
  for (int i = 0 ; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}

#include <AFMotor.h>
#include <EEPROM.h>
#define bt_serial Serial1
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command = ' ';
char sanitize = 'N';

void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(9600);
  EEPROM.write(0,0);
  EEPROM.write(1,0);
  EEPROM.write(2,0);
}

void loop()
{  
  if(bt_serial.available())
  {
    command = bt_serial.read();
    if(command != 'D')
    {
      command == 'C' ? sanitize = 'Y' : command == 'T' ? sanitize = 'N' : ' ';
      sanitize == 'Y' ? start_sanitize() : stop_sanitize();
    }
    else
    {
      Disconnected();
    }
  }
}

void movement()
{
  motor_speed(200);
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

void start_sanitize()
{
  motor_speed(150);
  if(EEPROM.read(0) == 0 && EEPROM.read(1) == 0 && EEPROM.read(2) == 0)
  {
    Stop();
    delay(100);
    forward();
    delay(3000);
    Stop();
    delay(1000);
    byte random_turn = random(1,3);
    for(int i = 0; i < 2; i++)
    {
      random_turn == 1 ? left() : right();
      EEPROM.write(i,random_turn);
      random_turn == 1 ? EEPROM.write(2,2) : EEPROM.write(2,1);
      delay(300);
      Stop();
      delay(100);
      forward();
      delay(1000);
    }
  } 
  else
  {
    Stop();
    delay(100);
    EEPROM.read(2) == 2 ? right() : left();
    delay(300);
    clear_turn();
  }
}

void stop_sanitize()
{
  clear_turn();
  movement();
}

void motor_speed(int set_speed)
{
    motor1.setSpeed(set_speed);
    motor2.setSpeed(set_speed);
    motor3.setSpeed(set_speed);
    motor4.setSpeed(set_speed);
}

void clear_turn()
{
  EEPROM.write(0,0);
  EEPROM.write(1,0);
  EEPROM.write(2,0);
}

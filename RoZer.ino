#include <AFMotor.h>
#define bt_serial Serial1
AF_DCMotor motor1(3);
AF_DCMotor motor2(4);

char command = ' ';
char mapping = 'N';
char mapp[5000];  
int mapp_len;

void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(9600);
  motor1.setSpeed(120);
  motor2.setSpeed(120);
  Stop();
  memset(mapp, 0, sizeof mapp);
  mapp_len = 0;
}

void loop()
{  
  if(bt_serial.available())
  {
    command = bt_serial.read();
    command == 'W' ? mapping = 'Y' : command == 'w' ? mapping = 'N': ' ';
    command != 'D' ? mapping == 'Y' ? Mapping(): movement():Disconnected();
  }
}

void movement()
{
  if(command == 'F')
  {
    forward();
  }
  
  if(command == 'B')
  {
    backward();
  }
  
  if(command == 'L')
  {
    left();
  }
  
  if(command == 'R')
  {
    right();
  }
  
  if(command == 'S')
  {
    Stop();
  }
}
void forward()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);

}

void backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void left()
{
  motor1.run(BACKWARD);
  motor2.run(FORWARD); 
}

void right()
{
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
} 

void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void Disconnected()
{
  Stop();
  memset(mapp, 0, sizeof mapp);
}

void Mapping()
{
  movement();
  mapp_len = sizeof(mapp) / sizeof(char);
  mapp[mapp_len+1] = command;
}

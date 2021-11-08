#include <AFMotor.h>
#define bt_serial Serial1
AF_DCMotor motor1(3);
AF_DCMotor motor2(4);

char command = ' '; 
const unsigned long baudRateBT = 9600;

void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(baudRateBT);
  motor1.setSpeed(120);
  motor2.setSpeed(120);
  Stop();
}

void loop()
{  
  if(bt_serial.available())
  {
    command = bt_serial.read();
    Serial.write(command);
    movement();
  }
    
  if(Serial.available())
  {
    command = Serial.read();
    bt_serial.write(command);
    movement();
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

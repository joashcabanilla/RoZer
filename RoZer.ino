#include <AFMotor.h>
#include <EEPROM.h>
#define bt_serial Serial1
#define humi 20
#define uv 21
#define echo1 22
#define trig1 23
#define echo2 24
#define trig2 25

AF_DCMotor motor1(1,MOTOR12_8KHZ); 
AF_DCMotor motor2(2,MOTOR12_8KHZ);
AF_DCMotor motor3(3,MOTOR12_8KHZ);
AF_DCMotor motor4(4,MOTOR12_8KHZ);

char command = ' ';
char sanitize = 'N'; 
char move_left_right = ' ';
long duration1,duration2;
int distance1,distance2;
int randnumber;

void setup() 
{       
  Serial.begin(9600);
  bt_serial.begin(9600);
  pinMode(humi,OUTPUT); 
  pinMode(uv,OUTPUT);
  pinMode(trig1,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
  EEPROM.write(0,0);
  EEPROM.write(1,0);
  digitalWrite(humi,LOW);
  digitalWrite(uv,LOW);
  motor_speed(255);
}

void loop()
{  
  sanitize == 'Y' ? start_sanitize() : stop_sanitize();
  if(bt_serial.available())
  {
    command = bt_serial.read();
    if(command != 'D')
    {
      command == 'C' ? sanitize = 'Y' : command == 'T' ? sanitize = 'N' : ' ';
      command == 'H' ? humi_on_off(1) : command == 'I' ? humi_on_off(2) : command == 'U' ? uv_on_off(1) : command == 'V' ?  uv_on_off(2): movement();
    }
    else
    {
      Disconnected();
    }
  }
}

void movement()
{
 if(sanitize == 'N')
 {
   if(command == 'L' || command == 'R')
    {
      move_left_right = command;
    }
    else if(command == 'F' || command == 'B')
    {
      move_left_right = ' ';
    }
    
  if(move_left_right == 'L')
  {
      left();
  }
  else if(move_left_right == 'R')
  {
    right(); 
  }
  else
  {
    Stop();
    switch(command){
      case 'F':
        forward();
        break;
      case 'B':
         backward();
        break;
    }
   }
 }
}

void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void backward()
{  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void forward()
{  
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void right()
{
  motor1.run(BACKWARD);
  motor2.run(RELEASE);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void left()
{
  motor1.run(RELEASE);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}  



void Disconnected()
{
  Stop();
  clear_turn();
  humi_on_off(1);
  uv_on_off(1);
  sanitize = 'N';
}

void start_sanitize()
{
  Stop();
  delay(500);
  get_distance(23,1);
  get_distance(25,2);
  if(distance1 != 0 && distance2 != 0)
  {
    if(distance1 <= 30 || distance2 <= 30)
    {
      distanceLess30cm(); 
    }
    else
    {
      pattern_without_obs();
    }
  }
  else
  {
    pattern_without_obs();
  }
}

void pattern_without_obs()
{
  forward();
      for(int i=0;i<=3000;i++)
    {
        get_distance(23,1);
        get_distance(25,2);
        if(distance1 != 0 && distance2 != 0)
        {
          if(distance1 <= 30 || distance2 <= 30)
          {
            distanceLess30cm(); 
          }
        }
    }
  Stop();
  delay(500);
  if(EEPROM.read(0) == 0)
  {
   randnumber = random(1,3);
   randnumber == 1 ? left() : right();
   EEPROM.write(0,randnumber);
   delay(1000); 
  }
  else
  {
    if(EEPROM.read(1) == 0)
    {
      randnumber = random(1,3);
      randnumber == 1 ? left() : right();
      EEPROM.write(1,randnumber);
      delay(1000); 
    }
    else
    {
      if(EEPROM.read(0) == 1 && EEPROM.read(1) == 1)
      {
        right();
        delay(1000);
        clear_turn();
      }
      else if(EEPROM.read(0) == 2 && EEPROM.read(1) == 2)
      {
        left();
        delay(1000);
        clear_turn();
      }
      else
      {
        EEPROM.update(0,EEPROM.read(1));
        randnumber = random(1,3);
        randnumber == 1 ? left() : right();
        EEPROM.update(1,randnumber);
        delay(1000); 
      }
    }
  }
}

void stop_sanitize()
{
  clear_turn();
//  movement();
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
}

void humi_on_off(int humi_on)
{
  if(humi_on == 1)
  {
    digitalWrite(humi,LOW);
    
  }
  else
  {
    digitalWrite(humi,HIGH);
  }
}

void uv_on_off(int uv_on)
{
    if(uv_on == 1)
  {
    digitalWrite(uv,LOW);
  }
  else
  {
    digitalWrite(uv,HIGH);
  }
}

void get_distance(int trig, int ultrasonic)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  switch(ultrasonic){
      case 1:
          duration1 = pulseIn(echo1, HIGH);
          distance1 = duration1 * 0.034 / 2;
          break;
      case 2:
          duration2 = pulseIn(echo2, HIGH);
          distance2 = duration2 * 0.034 / 2;
          break;
    }
}

void distanceLess30cm()
{
  Stop();
  delay(500);
  left();
  delay(1000);
}

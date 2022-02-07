#define analogWaterLevel1 A1
#define analogWaterLevel2 A2

int waterLevel1 = 0;
int waterLevel2 = 0;

void setup() {
Serial.begin(9600);
}

void loop() {
waterLevel1 = analogRead(analogWaterLevel1);
waterLevel2 = analogRead(analogWaterLevel2);
if(waterLevel1 <= 185)
{
  Serial.print("Water Level ");
  Serial.print(waterLevel1);
  Serial.println(" : LOW");
}
else if(waterLevel1 > 185 && waterLevel2 < 185)
{
  Serial.print("Water Level ");
  Serial.print(waterLevel1);
  Serial.print(waterLevel2);
  Serial.println(" : MEDIUM");
}
else if(waterLevel2 > 185)
{
  Serial.print("Water Level ");
  Serial.print(waterLevel2);
  Serial.println(" : HIGH");
}
delay(2000);
}

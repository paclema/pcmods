
int value, value2 ;
int ledpin = 9;                           // light connected to digital pin 10
int ledpin2 = 11;                           // light connected to digital pin 11
long time=0;

int periode = 2000;
int displace = 500;

void setup()
{
 // nothing for setup
}

void loop()
{
 time = millis();
 value = 128+127*cos(2*PI/periode*time);
 value2 = 128+127*cos(2*PI/periode*(displace-time));
 analogWrite(ledpin, value);           // sets the value (range from 0 to 255)
 analogWrite(ledpin2, value2);           // sets the value (range from 0 to 255)
}
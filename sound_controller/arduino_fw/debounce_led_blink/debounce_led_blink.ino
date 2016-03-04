/*
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 This example based on the Arduino Example Fade sketch
 but modified to use timing instead of the delay() function

 */
int brightness = 0;    // how bright the LED is
int fadeAmount = 8;    // how many points to fade the LED by
unsigned long currentTime;
unsigned long loopTime;
int ledState = LOW;

const int buttonPin = 2;    // the number of the pushbutton pin

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay =500;    // the debounce time; increase if the output flickers



void setup()  {
  // declare pin 9 to be an output:
  pinMode(9, OUTPUT);
  currentTime = millis();
  loopTime = currentTime;

  pinMode(buttonPin, INPUT);
}

void loop()  {

  // read the state of the switch into a local variable:
   int reading = digitalRead(buttonPin);
   if (reading != lastButtonState) {
     lastDebounceTime = millis();
   }

   if ((millis() - lastDebounceTime) > debounceDelay) {
     if (reading != buttonState) {
       buttonState = reading;
       if (buttonState == HIGH) {
         ledState = !ledState;
       }
     }
   }

   // set the LED:
   if (ledState) {
     currentTime = millis();
     if(currentTime >= (loopTime + 20)){
       analogWrite(9, brightness);
       brightness = brightness + fadeAmount;
       if (brightness == 0 || brightness == 255) {
         fadeAmount = -fadeAmount ;
       }
       loopTime = currentTime;
     }
   }
   else{
     digitalWrite(9,HIGH);

   }

   lastButtonState = reading;
}

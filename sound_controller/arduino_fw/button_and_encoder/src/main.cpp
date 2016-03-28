#include <Arduino.h>

#define numLEDs 4
#define numButtons 5
int leds[numLEDs]  = {6, 5, 9 ,10};
int buttons[numButtons] = {8, 7, 12, 11, 4};	//pin 4 es encoder_button

long buttonTimer[numButtons];
long longPressTime = 400;

boolean buttonActive[numButtons];
boolean longPressActive[numButtons];

boolean longClic[numButtons] = {false,false,false,false,false};
boolean shortClic[numButtons] = {false,false,false,false,false};
boolean LEDligh[numLEDs] = {false,false,false,false};

long time = 0;
unsigned long _millis;
int periode = 1000;

#define encoder0PinA 2
#define encoder0PinB 3

volatile unsigned int encoder0Pos = 0;
static boolean rotating=false;

void read_button(int index);
void control_led_state(int index);
void encoder_controll();
void reset_variables(int index);
void rotEncoder();

void setup() {

	Serial.begin(115200);

	for(int i=0; i<5;i++){
		pinMode(leds[i], OUTPUT);
		pinMode(buttons[i], INPUT);
		digitalWrite(leds[i],HIGH);
		digitalWrite(buttons[i],HIGH);
	}

	pinMode(encoder0PinA, INPUT);
	digitalWrite(encoder0PinA, HIGH);
	pinMode(encoder0PinB, INPUT);
	digitalWrite(encoder0PinB, HIGH);

	attachInterrupt(0, rotEncoder, CHANGE);


}

void rotEncoder(){
  rotating=true;
  // If a signal change (noise or otherwise) is detected
  // in the rotary encoder, the flag is set to true

}

void loop() {
	while(rotating) {
		delay(2);
		if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))	encoder0Pos++;
		else	encoder0Pos--;
		rotating=false; // Reset the flag back to false
		//Serial.println(encoder0Pos);
	}

	_millis = millis();

	for(int i=0; i<5; i++){
		read_button(i);
		control_led_state(i);
	}

	encoder_controll();

	for(int i=0; i<5; i++){
		reset_variables(i);
	}
}

void read_button(int index){
	if (digitalRead(buttons[index]) == LOW) {
		if (buttonActive[index] == false) {
			buttonActive[index] = true;
			buttonTimer[index] = _millis;
		}

		if ((_millis - buttonTimer[index] > longPressTime) && (longPressActive[index] == false)) {
			longPressActive[index] = true;
			Serial.print("Long clic: ");
			Serial.println(index);
			longClic[index] = true;
		}

	}
	else {
		if (buttonActive[index] == true) {
			if (longPressActive[index] == true) {
				longPressActive[index] = false;
			} else {
				Serial.print("Short clic: ");
				Serial.println(index);
				shortClic[index] = true;

			}
			buttonActive[index] = false;
		}
	}
}

void control_led_state(int index){

	if(shortClic[index]){
		Serial.print("Entro en shortClic. Ledlight: ");
		Serial.println(LEDligh[index]);
		if(LEDligh[index]){
			digitalWrite(leds[index],HIGH);
			//Controller_send("unmute", index);
		}
		else{
			digitalWrite(leds[index],LOW);
			//Controller_send("mute", index);
		}
		LEDligh[index] = !LEDligh[index];
	}


	if(longClic[index]){
		Serial.print("Entro en longClic de: ");
		Serial.println(index);
		time = _millis;
		analogWrite(leds[index], 128+127*cos(2*PI/periode*time));           // sets the value (range from 0 to 255)

	}

}

void encoder_controll(){

		if(shortClic[4] == true){
			Serial.println("------encoder_controll------");

			for(int i=0; i<4; i++){
				//shortClic[i] = false;
				if(longClic[i]){
					longClic[i] = false;
					if(LEDligh[i])	digitalWrite(leds[i],HIGH);
					else	digitalWrite(leds[i],LOW);
				}

			}
		}
}

void reset_variables(int index){

	if(shortClic[index]){
		shortClic[index] = false;
		longClic[index] = false;
		LEDligh[index] = !LEDligh[index];
	}

}

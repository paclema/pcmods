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
void control_status_mode(int index);
void encoder_controll();
void reset_variables(int index);
void rotEncoder();
void print_shortClic(int index);
void print_longClic(int index);

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
Serial.println("______________Sound Mixer v0.1___________________________________");
void rotEncoder(){
  rotating=true;/*
  // If a signal change (noise or otherwise) is detected
  // in the rotary encoder, the flag is set to true
	if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))	encoder0Pos++;
	else	encoder0Pos--;
	rotating=false; // Reset the flag back to false
	Serial.println(encoder0Pos);*/
}

void loop() {

	while(rotating) {
		delay(2);
		if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))	encoder0Pos++;
		else	encoder0Pos--;
		rotating=false; // Reset the flag back to false
		Serial.println(encoder0Pos);
	}

	_millis = millis();

	for(int i=0; i<4; i++){
		read_button(i);
		//control_led_state(i);
	}

	for(int i=0; i<4; i++){
		control_status_mode(i);
	}

/*
	Serial.print("Short clic: ");
	for(int i=0; i<4; i++){
		print_shortClic(i);
		Serial.print(",");
	}
	Serial.println(".");

	Serial.print("Long clic: ");
	for(int i=0; i<4; i++){
		print_longClic(i);
		Serial.print(",");
	}
	Serial.println(".");
	Serial.println("");
	Serial.println("____________________________________________________________________________________");
*/

	//encoder_controll();

	for(int i=0; i<4; i++){
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
			//Serial.print("Long clic: ");
			//Serial.println(index);
			longClic[index] = true;
		}

	}
	else {
		if (buttonActive[index] == true) {
			if (longPressActive[index] == true) {
				longPressActive[index] = false;
			} else {
				//Serial.print("Short clic: ");
				//Serial.println(index);
				shortClic[index] = true;

			}
			buttonActive[index] = false;
		}
	}
}


	if(shortClic[index]){
	}

}

void control_status_mode(int index){


					digitalWrite(leds[index],HIGH);
					//Controller_send("unmute", index);
					Serial.println("unmute");
					LEDligh[index] = true;
				}
				digitalWrite(leds[index],LOW);
				//Controller_send("mute", index);
				Serial.println("mute");
				LEDligh[index] = false;
			}

		}
			//Serial.print("Entro en longClic de: ");
			//Serial.println(index);
			time = _millis;
			analogWrite(leds[index], 128+127*cos(2*PI/periode*time));           // sets the value (range from 0 to 255)
			Serial.println("selVol");
		}
}

void encoder_controll(){
		if(shortClic[4] == true){
			//Serial.println("------encoder_controll------");

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
//	if(shortClic[index]){
		shortClic[index] = false;
		longClic[index] = false;
	//	LEDligh[index] = !LEDligh[index];
//	}

}

void print_shortClic(int index){
	Serial.print(shortClic[index]);
}

void print_longClic(int index){
	Serial.print(longClic[index]);
}

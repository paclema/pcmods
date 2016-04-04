#include <Arduino.h>

#define numLEDs 4
#define numButtons 5
#define numChannels ( numButtons - 1)
#define encVolMultiply 4

int leds[numLEDs]  = {6, 5, 9 ,10};
int buttons[numButtons] = {8, 7, 12, 11, 4};	//pin 4 es encoder_button

long buttonTimer[numButtons];
long longPressTime = 400;

boolean buttonActive[numButtons];
boolean longPressActive[numButtons];

boolean longClic[numButtons] = {false,false,false,false,false};
boolean shortClic[numButtons] = {false,false,false,false,false};

int volume_channel[numChannels+1] = {50,50,50,50,50};				//volume_channel[0-3]:Channels 1-4 volume_channel[4]:Global Volume
int mode_channel_state[numChannels] = {0,0,0,0};				//States: 0:unmute 1:mute 2:selecVolume
int mode_channel_state_last[numChannels] = {1,1,1,1};

boolean LEDligh[numLEDs] = {false,false,false,false};

long time = 0;
unsigned long _millis;
int periode = 1000;

#define encoder0PinA 2
#define encoder0PinB 3

volatile int encoder0Pos = 0, SelVol = 50;
static boolean rotating = false, SelVol_state = false;


void read_button(int index);
void update_channel_mode(int index);
void control_status_mode(int index);
void encoder_controll();
void reset_variables(int index);
void rotEncoder();
void print_shortClic(int index);
void print_longClic(int index);

void setup() {

	Serial.begin(115200);

	for(int i=0; i<numButtons;i++){
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

Serial.println("______________Sound Mixer v0.1___________________________________");
}

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

		if(SelVol_state){
			if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))	SelVol = SelVol + encVolMultiply;
			else	SelVol = SelVol - encVolMultiply;

			if(SelVol>=100) SelVol = 100;
			else if(SelVol<=0) SelVol = 0;

			Serial.print("SelVol: ");
			Serial.println(SelVol);
		}
		else{
			if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))	encoder0Pos++;
			else	encoder0Pos--;

			Serial.print("encoder0Pos: ");
			Serial.println(encoder0Pos);
		}

		rotating=false; // Reset the flag back to false
	}

	_millis = millis();

	for(int i=0; i<numButtons; i++){
		read_button(i);
	}

	for(int i=0; i<numChannels; i++){
		update_channel_mode(i);
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

	encoder_controll();

	for(int i=0; i<numChannels; i++){
		control_status_mode(i);
	}
	for(int i=0; i<numButtons; i++){
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

void update_channel_mode(int index){

	if(shortClic[index]){
		if(LEDligh[index])	mode_channel_state[index]=1;
		else	mode_channel_state[index]=0;
	}
	if(longClic[index]){
		SelVol_state = true;
		mode_channel_state[index]=2;
	}

}

void encoder_controll(){
		if(shortClic[4] == true){
			Serial.println("------encoder_control   START------");
			Serial.print("SelVolume channel: ");

			for(int i=0; i<numChannels; i++){
				if(mode_channel_state[i]==2){
					mode_channel_state_last[i]= 2;
					mode_channel_state[i] = 0;
					Serial.print(i);
					Serial.print(",");
				}
			}
			Serial.println("");
			Serial.println("------encoder_control    END------");

			SelVol_state = false;
			// COPIAR AQUI selVol en el array de volumenes de cada canal
		}
}

void control_status_mode(int index){

	if(mode_channel_state[index]!=mode_channel_state_last[index]){

			if(mode_channel_state[index]==0){
					digitalWrite(leds[index],HIGH);
					//Controller_send("unmute", index);
					Serial.print("unmute ch: ");
					Serial.println(index);
					LEDligh[index] = true;
				}
			else if (mode_channel_state[index]==1) {
				digitalWrite(leds[index],LOW);
				//Controller_send("mute", index);
				Serial.print("mute ch: ");
				Serial.println(index);
				LEDligh[index] = false;
			}

		}
		if(mode_channel_state[index]==2){
			//Serial.print("Entro en longClic de: ");
			//Serial.println(index);
			time = _millis;
			analogWrite(leds[index], 128+127*cos(2*PI/periode*time));           // sets the value (range from 0 to 255)
			//Serial.println("SelVol_state");
		}
		mode_channel_state_last[index] = mode_channel_state[index];
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

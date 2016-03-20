int leds[4]  = {6, 5, 9 ,10};
int buttons[5] = {8, 7, 12, 11, 4};	//pin 4 es encoder_button

long buttonTimer[4] = {0};
long longPressTime = 400;

boolean buttonActive[4] = {};
boolean longPressActive[4] = {};

boolean longClic[4] = {false};
boolean shortClic[4] = {false};
boolean LEDligh[4] = {false};

long time = 0;
int periode = 1000;

#define encoder0PinA 2
#define encoder0PinB 3

volatile unsigned int encoder0Pos = 0;
static boolean rotating=false;

int volumeCh1 = 50, lastVolumeCh1 = 50;

void setup() {

	for(int i=0; i<4;i++){
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

	Serial.begin(115200);
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

	for(int i=0; i<4; i++){
		read_button(i);
		control_led_state(i);
	}




}

void read_button(int index){

	if (digitalRead(buttons[index]) == LOW) {
		if (buttonActive[index] == false) {
			buttonActive[index] = true;
			buttonTimer[index] = millis();
		}

		if ((millis() - buttonTimer[index] > longPressTime) && (longPressActive[index] == false)) {
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
		shortClic[index] = false;
		longClic[index] = false;
	}


	if(longClic[index]){
		Serial.println("Entro en longClic");
		time = millis();
		analogWrite(leds[index], 128+127*cos(2*PI/periode*time));           // sets the value (range from 0 to 255)

	}

}

void encoder_controll(){

/*
		volumeCh1 = encoder0Pos;
		if(volumeCh1 >= 100) volumeCh1 = 100;
		if(volumeCh1 <= 0) volumeCh1 = 0;
		lastVolumeCh1 = volumeCh1;

		Serial.println(volumeCh1);
		*/
}
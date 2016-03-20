int LED1 = 9;
int button = 3;

boolean LED1State = false;
boolean LED2State = false;

long buttonTimer = 0;
long longPressTime = 400;

boolean buttonActive = false;
boolean longPressActive = false;

boolean longClic = false;
boolean shortClic = false;
boolean LEDligh = false;

long time = 0;
int periode = 2000;

void setup() {

	pinMode(LED1, OUTPUT);
	pinMode(button, INPUT);
	Serial.begin(115200);
}

void loop() {

	if (digitalRead(button) == HIGH) {
		if (buttonActive == false) {
			buttonActive = true;
			buttonTimer = millis();
		}

		if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
			longPressActive = true;
			LED1State = !LED1State;
			//digitalWrite(LED1, LED1State);
			Serial.println("Long clic");
			//LedState(0);
			longClic = true;
		}

	} 
	else {
		if (buttonActive == true) {
			if (longPressActive == true) {
				longPressActive = false;
			} else {
				LED2State = !LED2State;
				Serial.println("Short clic");
				//digitalWrite(LED2, LED2State);
				//LedState(1);
				shortClic = true;

			}
			buttonActive = false;
		}
	}




	if(shortClic){
		Serial.print("Entro en shortClic. Ledlight: ");
		Serial.println(LEDligh);
		if(LEDligh)	digitalWrite(LED1,HIGH);
		else digitalWrite(LED1,LOW);
		LEDligh = !LEDligh;
		shortClic = false;
		longClic = false;
	}


	if(longClic){
		time = millis();
		analogWrite(LED1, 128+127*cos(2*PI/periode*time));           // sets the value (range from 0 to 255)
	}
}
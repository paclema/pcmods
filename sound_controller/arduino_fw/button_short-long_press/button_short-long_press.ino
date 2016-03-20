int LED1 = 9;
int button = 3;

boolean LED1State = false;
boolean LED2State = false;

long buttonTimer = 0;
long longPressTime = 400;

boolean buttonActive = false;
boolean longPressActive = false;

unsigned long currentTime;
unsigned long loopTime;
int brightness = 0;    // how bright the LED is
int fadeAmount = 6;    // how many points to fade the LED by

boolean longClic = false;
boolean shortClic = false;
boolean LEDligh = false;
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
		 currentTime = millis();
	     if(currentTime >= (loopTime + 20)){
	     	analogWrite(LED1, brightness);
	     	brightness = brightness + fadeAmount;
	     	if (brightness == 0 || brightness == 255) {
	     		fadeAmount = -fadeAmount ;
	     	}
	     	loopTime = currentTime;
	     }
	}
}
int LED1 = 12;
int button = 3;

boolean LED1State = false;
boolean LED2State = false;

long buttonTimer = 0;
long longPressTime = 250;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {

	pinMode(LED1, OUTPUT);
	pinMode(button, INPUT);

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
		}

	} 
	else {
		if (buttonActive == true) {
			if (longPressActive == true) {
				longPressActive = false;
			} else {
				LED2State = !LED2State;
				//digitalWrite(LED2, LED2State);
			}
			buttonActive = false;
		}
	}


	// set the LED:
	if(LED2State){
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
			if (LED1State)	digitalWrite(9,HIGH);
				else	digitalWrite(9,HIGH);
			}


}

const int  buttonPin = 2;
const int ledPin = 13;

int buttonState = 0;
int lastButtonState = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      Serial.println("A");
    } else {
      Serial.println("B");
    }
  }
  lastButtonState = buttonState;
}

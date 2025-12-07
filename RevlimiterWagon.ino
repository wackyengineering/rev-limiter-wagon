unsigned long pulseCount = 0;
unsigned long startTime;
const unsigned long duration = 100; // timeframe in ms
const int potPin = A0;
const int relayPin = 3;
long scaledPulseCount;
long scaledPotValue;

// Pulse duration for relay ON
const unsigned long onPulseDuration = 20; // milliseconds
unsigned long onPulseStart = 0;
bool onPulseActive = false;

void setup() {
    pinMode(2, INPUT);
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); // start OFF
    attachInterrupt(digitalPinToInterrupt(2), countPulse, RISING);
    startTime = millis();
    Serial.begin(9600);
}

void loop() {
    int potValue = analogRead(potPin);
    scaledPotValue = map(potValue, 0, 1023, 400, 6000);
    scaledPulseCount = map(pulseCount, 0, 100, 0, 10000);

    Serial.println("Scaled pulse count: " + String(scaledPulseCount));
    Serial.println("Scaled pot value: " + String(scaledPotValue));
    Serial.println("Relay ON pulse active: " + String(onPulseActive));

    // Trigger ON pulse if above setpoint and pulse is not active
    if (!onPulseActive && scaledPulseCount > scaledPotValue) {
        digitalWrite(relayPin, HIGH);
        onPulseStart = millis();
        onPulseActive = true;
    }

    // Check if ON pulse is done
    if (onPulseActive && (millis() - onPulseStart >= onPulseDuration)) {
        digitalWrite(relayPin, LOW);
        onPulseActive = false;
    }

    // Reset pulse count every 'duration' ms
    if ((millis() - startTime) >= duration) {
        startTime = millis();
        pulseCount = 0;
    }
}

void countPulse() {
    pulseCount++;
}

const int pwmPin = 9;
const int adcPin = A0;
const int startButton = 2;
const int abortButton = 3;

const unsigned long duration = 60000; // 60 seconds
const int samplingInterval = 1;       // in ms
const int totalSamples = duration / samplingInterval;

unsigned long startTime;
bool isRunning = false;

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(abortButton, INPUT_PULLUP);
  Serial.begin(115200);
  while (!Serial); // Wait for serial monitor
  Serial.println("Ready. Press START.");
}

void loop() {
  if (digitalRead(startButton) == LOW && !isRunning) {
    delay(200); // debounce
    isRunning = true;
    startTime = millis();
    Serial.println("START");
    runSweep();
    isRunning = false;
    Serial.println("DONE");
  }

  if (digitalRead(abortButton) == LOW && isRunning) {
    isRunning = false;
    Serial.println("ABORTED");
  }
}

void runSweep() {
  for (int i = 0; i < totalSamples && isRunning; i++) {
    float t = (float)i * samplingInterval / 1000.0; // seconds
    float freq = 1.0 + 9.0 * t / 60.0; // linear chirp from 1 Hz to 10 Hz
    float chirp = sin(2 * PI * freq * t);

    // Convert -1 to 1 range to 0 to 255 for PWM
    int pwmVal = (int)((chirp + 1.0) * 127.5);
    analogWrite(pwmPin, pwmVal);

    int adcVal = analogRead(adcPin); // 0-1023 range

    // Send both values to PC (raw values)
    Serial.print(i); Serial.print(",");
    Serial.print(pwmVal); Serial.print(",");
    Serial.println(adcVal);

    delay(samplingInterval);
  }
}

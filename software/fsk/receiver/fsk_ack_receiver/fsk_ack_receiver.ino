// NODE B: FSK PACKET DETECTOR + ACK SENDER

const int DATA_PD_PIN     = A0;   // photodiode seeing Node A's LED
const int DATA_THRESHOLD  = 680;  // from your measurement
const int ACK_LED_PIN     = 3;    // ACK LED pointing to Node A
const int BIT_WINDOW_MS   = 400;  // time window to watch for transitions
const int SAMPLE_STEP_MS  = 15;   // sampling interval

void setup() {
  Serial.begin(9600);
  pinMode(ACK_LED_PIN, OUTPUT);
  digitalWrite(ACK_LED_PIN, LOW);
  Serial.println("NODE B (FSK ACK) ready. Waiting for FSK packet...");
}

// Count light/dark transitions in a time window
int countTransitionsFSK(unsigned long windowMs) {
  unsigned long start = millis();
  int transitions = 0;

  // initial state (light or dark)
  bool lastState = (analogRead(DATA_PD_PIN) < DATA_THRESHOLD);

  while (millis() - start < windowMs) {
    bool current = (analogRead(DATA_PD_PIN) < DATA_THRESHOLD);
    if (current != lastState) {
      transitions++;
      lastState = current;
    }
    delay(SAMPLE_STEP_MS);
  }

  return transitions;
}

void sendAck() {
  Serial.println("Sending ACK (OOK flashes)...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(ACK_LED_PIN, HIGH);
    delay(80);
    digitalWrite(ACK_LED_PIN, LOW);
    delay(80);
  }
}

void loop() {
  Serial.println("Listening for FSK packet...");
  int transitions = countTransitionsFSK(BIT_WINDOW_MS);

  Serial.print("Transitions counted = ");
  Serial.println(transitions);

  // If high number of transitions -> we treat as an FSK packet
  if (transitions >= 4) {   // tune if needed (4–6 is typical)
    Serial.println("FSK PACKET DETECTED from NODE A!");
    sendAck();
    delay(1000); // dead time so we don't detect same packet multiple times
  }

  delay(200);
}
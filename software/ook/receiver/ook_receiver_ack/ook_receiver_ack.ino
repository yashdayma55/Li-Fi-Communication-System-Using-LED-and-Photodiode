// NODE B: Receive OOK "packet" from Node A, then send ACK back via LED

const int DATA_PD_PIN     = A0;   // photodiode that sees Node A's data LED
const int DATA_THRESHOLD  = 680;  // from your measurements
const int ACK_LED_PIN     = 3;    // LED that points to Node A's ACK photodiode

void setup() {
  Serial.begin(9600);
  pinMode(ACK_LED_PIN, OUTPUT);
  digitalWrite(ACK_LED_PIN, LOW);
  Serial.println("NODE B ready (waiting for packet)...");
}

void loop() {
  int v = analogRead(DATA_PD_PIN);

  // If light from Node A's LED is detected:
  if (v < DATA_THRESHOLD) {
    Serial.println("PACKET RECEIVED from NODE A!");

    // Send ACK: 3 short flashes
    Serial.println("Sending ACK...");
    for (int i = 0; i < 3; i++) {
      digitalWrite(ACK_LED_PIN, HIGH);
      delay(80);
      digitalWrite(ACK_LED_PIN, LOW);
      delay(80);
    }

    // Small pause to avoid detecting the same packet many times
    delay(1000);
  }
}
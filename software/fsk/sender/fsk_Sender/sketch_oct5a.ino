// NODE A — FSK LARGE  SENDER


const int DATA_LED_PIN = 3;
const int ACK_PD_PIN   = A0;
const int ACK_THRESHOLD = 650;

// FSK pulse lengths
const int SHORT_ON  = 80;   
const int LONG_ON   = 160;   
const int GAP_OFF   = 80;

// PREAMBLE (sync byte)
const byte PREAMBLE = 0xAA;


String MESSAGE = "HELLO YASH THIS IS A MUCH LONGER MESSAGE USING FSK VLC SYSTEM AND IT SHOULD DECODE CORRECTLY.";


void setup() {
  Serial.begin(9600);
  pinMode(DATA_LED_PIN, OUTPUT);
  digitalWrite(DATA_LED_PIN, LOW);

  Serial.println("NODE A — LARGE FSK Sender READY");
  Serial.println("Press ENTER to send.");
}

// send ONE bit 
void sendBit(int bit) {
  int onTime = (bit == 1) ? LONG_ON : SHORT_ON;

  digitalWrite(DATA_LED_PIN, HIGH);
  delay(onTime);
  digitalWrite(DATA_LED_PIN, LOW);
  delay(GAP_OFF);
}

// send ONE byte 
void sendByte(byte b) {
  for (int i = 7; i >= 0; i--) {
    sendBit((b >> i) & 1);
  }
}

// send full frame
void sendFrame() {
  Serial.println("\nSending message:");
  Serial.println(MESSAGE);

  // 1) Send preamble
  sendByte(PREAMBLE);

  // 2) Send LENGTH of message
  sendByte(MESSAGE.length());

  // 3) Send MESSAGE bytes
  for (int i = 0; i < MESSAGE.length(); i++) {
    sendByte((byte)MESSAGE[i]);
  }

  Serial.println("Done sending.");
}

//  wait for ACK 
bool waitForAck(unsigned long timeoutMs) {
  unsigned long start = millis();

  while (millis() - start < timeoutMs) {
    int v = analogRead(ACK_PD_PIN);
    if (v < ACK_THRESHOLD) {
      int hits = 0;
      for (int i = 0; i < 5; i++) {
        if (analogRead(ACK_PD_PIN) < ACK_THRESHOLD) hits++;
        delay(20);
      }
      return hits >= 3;
    }
  }
  return false;
}

void loop() {
  Serial.println("\nPress ENTER to SEND...");
  while (!Serial.available());
  while (Serial.available()) Serial.read();

  sendFrame();

  Serial.println("Waiting for ACK...");
  bool ack = waitForAck(4000);

  if (ack) Serial.println("ACK RECEIVED ");
  else     Serial.println("ACK TIMEOUT ");

  delay(2000);
}

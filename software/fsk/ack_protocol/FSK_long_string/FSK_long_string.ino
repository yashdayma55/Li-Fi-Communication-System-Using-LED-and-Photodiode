
// NODE B — LARGE FSK MESSAGE RECEIVER + ACK


const int DATA_PD_PIN    = A0;
const int DATA_THRESHOLD = 680;

const int ACK_LED_PIN = 3;

const int SHORT_MAX = 120;
const int LONG_MIN  = 120;

const byte PREAMBLE = 0xAA;

// Max we can store
#define MAX_MESSAGE 200
char messageBuf[MAX_MESSAGE+1];

void setup() {
  Serial.begin(9600);
  pinMode(ACK_LED_PIN, OUTPUT);
  digitalWrite(ACK_LED_PIN, LOW);

  Serial.println("NODE B — LARGE FSK Receiver READY");
}

// Send ACK
void sendAck() {
  Serial.println("Sending ACK...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(ACK_LED_PIN, HIGH);
    delay(80);
    digitalWrite(ACK_LED_PIN, LOW);
    delay(80);
  }
}

// Measure ON-time
unsigned long readPulse() {
  while (analogRead(DATA_PD_PIN) > DATA_THRESHOLD);

  unsigned long start = millis();
  while (analogRead(DATA_PD_PIN) < DATA_THRESHOLD);

  return millis() - start;
}

// Decode bit
int readBit() {
  unsigned long t = readPulse();

  Serial.print("ON=");
  Serial.print(t);
  Serial.print("ms -> ");

  if (t < SHORT_MAX) {
    Serial.println("0");
    return 0;
  } else {
    Serial.println("1");
    return 1;
  }
}

// Decode byte
byte readByte() {
  byte b = 0;
  for (int i = 0; i < 8; i++) {
    b = (b << 1) | readBit();
  }
  return b;
}

void readFrame() {
  Serial.println("\nWaiting for PREAMBLE (0xAA)...");

  // find preamble
  while (true) {
    byte b = readByte();
    if (b == PREAMBLE) break;
  }

  Serial.println("Preamble detected!");

  // Next byte = length
  int length = readByte();
  Serial.print("Length = ");
  Serial.println(length);

  if (length > MAX_MESSAGE) {
    Serial.println("ERROR: message too big!");
    return;
  }

  // Read message
  for (int i = 0; i < length; i++) {
    messageBuf[i] = readByte();
  }
  messageBuf[length] = '\0';

  Serial.println("\n========== RECEIVED MESSAGE ==========");
  Serial.println(messageBuf);

  sendAck();
}

void loop() {
  readFrame();
  delay(500);
}

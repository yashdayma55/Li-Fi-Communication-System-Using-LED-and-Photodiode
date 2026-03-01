// =======================================
// NODE B — PWM MESSAGE RECEIVER + ACK SENDER
// =======================================

const int DATA_PD_PIN    = A0;     
const int DATA_THRESHOLD = 900;   

const int ACK_LED_PIN    = 3;     

// PWM brightness decoding threshold
const int PWM_VOLTAGE_THRESHOLD = 550;

const int BIT_DURATION_MS = 100; 
const int SAMPLE_DELAY_MS = 50;

const byte PREAMBLE = 0xAA;

#define MAX_MESSAGE 200
char messageBuf[MAX_MESSAGE + 1];


void setup() {
  Serial.begin(9600);
  pinMode(ACK_LED_PIN, OUTPUT);
  digitalWrite(ACK_LED_PIN, LOW);

  Serial.println("NODE B — PWM Receiver READY");
}

void sendAck() {
  Serial.println("Sending ACK...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(ACK_LED_PIN, HIGH);
    delay(80);
    digitalWrite(ACK_LED_PIN, LOW);
    delay(80);
  }
}


int readBit() {
  delay(SAMPLE_DELAY_MS);

  int v = analogRead(DATA_PD_PIN);

  delay(BIT_DURATION_MS - SAMPLE_DELAY_MS + 5);

  return (v < PWM_VOLTAGE_THRESHOLD) ? 1 : 0;
}


byte readByte() {
  byte b = 0;
  for (int i = 0; i < 8; i++) {
    b = (b << 1) | readBit();
  }
  return b;
}


void readFrame() {
  Serial.println("\nWaiting for PREAMBLE...");

  // 1. WAIT until LED turns ON (dark → bright)
  while (analogRead(DATA_PD_PIN) > DATA_THRESHOLD) { }

  // 2. Detect 0xAA using shifting register
  byte shiftReg = 0;

  while (true) {
    int bit = readBit();
    shiftReg = (shiftReg << 1) | bit;

    if (shiftReg == PREAMBLE) break;
  }

  Serial.println("Preamble detected!");

  // 3. READ LENGTH BYTE
  byte length = readByte();
  Serial.print("Length decoded = ");
  Serial.println(length);

  if (length > MAX_MESSAGE) {
    Serial.println("ERROR: Message too long or corrupted!");
    return;
  }

  // 4. READ MESSAGE BYTES
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

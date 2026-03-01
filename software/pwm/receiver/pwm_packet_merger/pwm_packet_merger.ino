
// NODE B — PWM Receiver


const int DATA_PD_PIN    = A0;      
const int DATA_THRESHOLD = 900;     
const int ACK_LED_PIN    = 3;       

const int PWM_VOLTAGE_THRESHOLD = 480;

const int BIT_DURATION_MS = 150;
const int SAMPLE_DELAY_MS = 75;
const byte PREAMBLE = 0xAA;

#define MAX_MESSAGE 200
char messageBuf[MAX_MESSAGE + 1];


String fullMessage = "";

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

  // wait for LED to turn ON
  while (analogRead(DATA_PD_PIN) > DATA_THRESHOLD) {}

  // detect preamble
  byte shiftReg = 0;
  while (true) {
    int bit = readBit();
    shiftReg = (shiftReg << 1) | bit;
    if (shiftReg == PREAMBLE) break;
  }

  Serial.println("Preamble detected!");

  
  byte length = readByte();
  Serial.print("Length decoded = ");
  Serial.println(length);

  if (length == 0 || length > MAX_MESSAGE) {
    Serial.println("ERROR: invalid packet length");
    return;
  }

  // read payload
  for (int i = 0; i < length; i++) {
    messageBuf[i] = readByte();
  }
  messageBuf[length] = '\0';

  // Print this packet (unchanged)
  Serial.println("\n=== PACKET RECEIVED ===");
  Serial.println(messageBuf);
  Serial.println("========================");

  
  fullMessage += String(messageBuf);

  Serial.println("\n=== MERGED FULL MESSAGE SO FAR ===");
  Serial.println(fullMessage);
  Serial.println("==================================");

  sendAck();
}

void loop() {
  readFrame();
  delay(500);
}


// NODE B — FSK MESSAGE RECEIVER + ACK 


const int DATA_PD_PIN    = A0;   // Photodiode seeing Node A's LED
const int DATA_THRESHOLD = 680;  

const int ACK_LED_PIN = 3;       // ACK LED facing Node A

// These correspond to Node A's SHORT_ON / LONG_ON
const int SHORT_MAX = 120;   
const int LONG_MIN  = 120;   

const byte PREAMBLE = 0xAA; 
const int  MESSAGE_LEN = 9;  

char messageBuf[MESSAGE_LEN + 1];

void setup() {
  Serial.begin(9600);
  pinMode(ACK_LED_PIN, OUTPUT);
  digitalWrite(ACK_LED_PIN, LOW);

  Serial.println("NODE B — FSK (Pulse-Length) Receiver READY");
}

// Send ACK via OOK
void sendAck() {
  Serial.println("Sending OOK ACK...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(ACK_LED_PIN, HIGH);
    delay(80);
    digitalWrite(ACK_LED_PIN, LOW);
    delay(80);
  }
}

// Measure how long light is ON (in ms)
unsigned long measurePulseOnTime() {
  // Wait for LED to go ON first
  while (analogRead(DATA_PD_PIN) > DATA_THRESHOLD) {
    // still dark
  }

  unsigned long start = millis();

  // Now wait until LED goes OFF again
  while (analogRead(DATA_PD_PIN) < DATA_THRESHOLD) {
    // still light
  }

  unsigned long duration = millis() - start;
  return duration;
}

// Decode one bit using ON duration
int readBit() {
  unsigned long onTime = measurePulseOnTime();

  Serial.print("ON time = ");
  Serial.print(onTime);
  Serial.print(" ms -> ");

  if (onTime < SHORT_MAX) {
    Serial.println("bit 0");
    return 0;
  } else {
    Serial.println("bit 1");
    return 1;
  }
}

// Read one byte (MSB first)
byte readByte() {
  byte b = 0;
  for (int i = 0; i < 8; i++) {
    int bit = readBit();
    b = (b << 1) | (bit & 0x01);
  }
  return b;
}

// Read full frame: PREAMBLE + fixed-length message
void readFrame() {
  Serial.println("Waiting for PREAMBLE (0xAA)...");

  // Find preamble by sliding over bytes
  while (true) {
    byte b = readByte();
    Serial.print("Byte read: 0x");
    Serial.println(b, HEX);

    if (b == PREAMBLE) {
      Serial.println("Preamble detected!");
      break;
    }
  }

  // Now read MESSAGE_LEN bytes as message
  for (int i = 0; i < MESSAGE_LEN; i++) {
    byte c = readByte();
    messageBuf[i] = (char)c;
  }
  messageBuf[MESSAGE_LEN] = '\0';

  Serial.println("\n===== MESSAGE RECEIVED =====");
  Serial.print("Message: **");
  Serial.print(messageBuf);
  

  // Send ACK
  sendAck();
}

void loop() {
  readFrame();
  delay(500);
}

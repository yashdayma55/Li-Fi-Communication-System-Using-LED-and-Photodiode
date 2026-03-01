
// NODE B: OOK

const int DATA_PD_PIN     = A0;   
const int DATA_THRESHOLD  = 680;  
const int ACK_LED_PIN     = 3;    
const int BIT_PERIOD      = 150;  

const uint8_t HEADER      = 0xA5;
const int MAX_LEN         = 128;

void setup() {
  Serial.begin(9600);
  pinMode(ACK_LED_PIN, OUTPUT);
  digitalWrite(ACK_LED_PIN, LOW);
  Serial.println("NODE B ready. Waiting for frames...");
}

// ---- Low-level: read one bit ----
bool readBit() {
  int v = analogRead(DATA_PD_PIN);
  bool bit = (v < DATA_THRESHOLD); // light => 1, dark => 0
  delay(BIT_PERIOD);
  return bit;
}

// ---- Read one byte (8 bits MSB-first) ----
uint8_t readByte() {
  uint8_t b = 0;
  for (int i = 0; i < 8; i++) {
    b <<= 1;
    if (readBit()) b |= 1;
  }
  return b;
}

// ---- Simple checksum ----
uint8_t computeChecksum(const char *data, uint8_t len) {
  uint16_t sum = 0;
  for (uint8_t i = 0; i < len; i++) {
    sum += (uint8_t)data[i];
  }
  return (uint8_t)(sum & 0xFF);
}

// ---- Find HEADER byte (0xA5) in the bit stream ----
bool waitForHeader(uint8_t header, unsigned long timeoutMs) {
  unsigned long start = millis();
  uint8_t reg = 0;

  while (millis() - start < timeoutMs) {
    bool bit = readBit();
    reg = (reg << 1) | (bit ? 1 : 0);
    if (reg == header) {
      return true;  // we found the HEADER pattern
    }
  }
  return false;
}

// ---- Receive a frame: [HEADER][LEN][DATA...][CHK] ----
bool receiveFrame(char *outBuf, uint8_t &outLen) {
  // 1) Wait for header
  Serial.println("Looking for HEADER...");
  if (!waitForHeader(HEADER, 10000)) {  // 10 second timeout
    Serial.println("No HEADER detected (timeout).");
    return false;
  }

  Serial.println("HEADER detected, reading LEN...");
  // 2) Read length byte
  uint8_t len = readByte();
  Serial.print("LEN received: ");
  Serial.println(len);

  if (len == 0 || len > MAX_LEN) {
    Serial.println("Invalid length.");
    return false;
  }

  // 3) Read payload
  for (uint8_t i = 0; i < len; i++) {
    outBuf[i] = (char)readByte();
  }

  // 4) Read checksum
  uint8_t recvChk = readByte();

  // 5) Verify checksum
  uint8_t calcChk = computeChecksum(outBuf, len);
  if (calcChk != recvChk) {
    Serial.print("Checksum mismatch. recv=");
    Serial.print(recvChk);
    Serial.print(" calc=");
    Serial.println(calcChk);
    return false;
  }

  // Null-terminate
  outBuf[len] = '\0';
  outLen = len;
  return true;
}

// ---- Send optical ACK: 3 short flashes ----
void sendAck() {
  Serial.println("Sending optical ACK...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(ACK_LED_PIN, HIGH);
    delay(80);
    digitalWrite(ACK_LED_PIN, LOW);
    delay(80);
  }
}

void loop() {
  static char buffer[MAX_LEN + 1];
  uint8_t length = 0;

  Serial.println("Waiting for incoming frame...");

  if (receiveFrame(buffer, length)) {
    Serial.print("Frame OK. Length=");
    Serial.println(length);
    Serial.print("Message: ");
    Serial.println(buffer);

    // Send ACK back to Node A
    sendAck();

    delay(1500);
  } else {
    Serial.println("Frame receive FAILED.");
    delay(1000);
  }
}
// NODE A — PWM SENDER large message


const int DATA_LED_PIN = 3;
const int ACK_PD_PIN   = A0;
const int ACK_THRESHOLD = 650;  

const int LEVEL_ONE  = 255;  
const int LEVEL_ZERO = 10;   


const int BIT_DURATION_MS = 150; 
const byte PREAMBLE = 0xAA;

String HUGE_MESSAGE = "This is a very long message that is now being sent in small, safe packets. This eliminates timing drift and allows us to send essentially infinite amounts of data without errors! Packetization is how the real internet works.";

const int CHUNK_SIZE = 30; 
int currentPosition = 0;
bool messageComplete = false;

void setup() {
  Serial.begin(9600);
  pinMode(DATA_LED_PIN, OUTPUT);
  analogWrite(DATA_LED_PIN, 0);

  Serial.println("NODE A — Smart Packet Sender READY.");
  Serial.println("Press ENTER to start transmission.");
}

void sendBit(int bit) {
  int level = (bit == 1) ? LEVEL_ONE : LEVEL_ZERO;
  analogWrite(DATA_LED_PIN, level);
  delay(BIT_DURATION_MS);
  analogWrite(DATA_LED_PIN, 0); 
  delay(5);
}

void sendByte(byte b) {
  for (int i = 7; i >= 0; i--) {
    sendBit((b >> i) & 1);
  }
}

void sendFrame(String payload) {
  Serial.print("Sending Packet: \"");
  Serial.print(payload);
  Serial.println("\"");

  sendByte(PREAMBLE);
  sendByte((byte)payload.length());

  for (int i = 0; i < payload.length(); i++) {
    sendByte((byte)payload[i]);
  }

  analogWrite(DATA_LED_PIN, 0);
}

bool waitForAck(unsigned long timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (analogRead(ACK_PD_PIN) < ACK_THRESHOLD) {
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
  if (currentPosition == 0 && !messageComplete) {
     Serial.println("\nPress ENTER to START...");
     while (!Serial.available());
     while (Serial.available()) Serial.read();
  }

  if (currentPosition < HUGE_MESSAGE.length()) {
    int endIndex = currentPosition + CHUNK_SIZE;
    if (endIndex > HUGE_MESSAGE.length()) endIndex = HUGE_MESSAGE.length();
    
    String currentChunk = HUGE_MESSAGE.substring(currentPosition, endIndex);

    sendFrame(currentChunk);

    Serial.println("Waiting for ACK...");
    bool ack = waitForAck(5000); 

    if (ack) {
      Serial.println("ACK RECEIVED . Moving to next packet.");
      currentPosition += CHUNK_SIZE; 
      delay(1000); 
    } else {
      Serial.println("ACK TIMEOUT . Retrying same packet...");
      delay(2000); 
    }

  } else if (!messageComplete) {
    Serial.println("\n--- FULL MESSAGE TRANSFER COMPLETE ---");
    messageComplete = true;
  }
}

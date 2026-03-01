// ==========================================
// NODE B: ook long message
// ==========================================

const int DATA_PD_PIN    = A0;
const int DATA_THRESHOLD = 680; 
const int ACK_LED_PIN    = 3;   
const int BIT_PERIOD     = 40;  

void setup() {
  Serial.begin(9600);
  pinMode(ACK_LED_PIN, OUTPUT);
  digitalWrite(ACK_LED_PIN, LOW);
  Serial.println("\n--- RECEIVER READY ---");
  Serial.println("Waiting for stream...\n");
}

void sendFinalAck() {
  Serial.println("\n[End of Message detected. Sending ACK]");
  // 3 quick flashes to signal success
  for (int i = 0; i < 3; i++) {
    digitalWrite(ACK_LED_PIN, HIGH);
    delay(100); 
    digitalWrite(ACK_LED_PIN, LOW);
    delay(100); 
  }
}

char readByte() {
  // 1. Wait for START BIT (Rising Edge)
  while (analogRead(DATA_PD_PIN) > DATA_THRESHOLD) { 
    // Idle loop (waiting for light)
  }

  // Sync: Skip Start bit, land in middle of Bit 0
  delay(BIT_PERIOD + (BIT_PERIOD / 2));

  // 2. Read 8 Data Bits
  char c = 0;
  for (int i = 0; i < 8; i++) {
    int v = analogRead(DATA_PD_PIN);
    bool bit = (v < DATA_THRESHOLD); 
    
    if (bit) c |= (1 << i);
    
    delay(BIT_PERIOD);
  }

  // 3. Wait for STOP BIT (Line must go dark)
  // This is crucial to prevent reading the last data bit as a new start bit
  delay(BIT_PERIOD / 2); 
  
  return c;
}

void loop() {
  // Block until a character arrives
  char c = readByte();
  
  // Print character immediately to form the sentence
  Serial.print(c);
  
  // If we receive the newline terminator, we know the message is done
  if (c == '\n') {
    sendFinalAck();
    Serial.println("\n-----------------------------------");
  }
}
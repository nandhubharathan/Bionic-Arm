#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
uint8_t armAddress[] = {0x1C, 0x69, 0x20, 0x95, 0x94, 0x4A};

// Pins: Thumb=35, Index=32, Middle=33, Ring=34, Pinky=26
int flexPins[] = {35, 32, 33, 34, 26}; 
int idleValues[5];
int closeThresholds[5];
int openThresholds[5];

typedef struct {
  uint16_t f1, f2, f3, f4, f5;
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 5; i++) pinMode(flexPins[i], INPUT);
  
  Serial.println("--- SENSITIVITY: THUMB=6, OTHERS=9 CLOSE / 1 UP TO OPEN ---");
  Serial.println("KEEP GLOVE PERFECTLY FLAT...");
  delay(2500); 

  for (int i = 0; i < 5; i++) {
    long sum = 0;
    for(int j=0; j<100; j++) {
      sum += map(analogRead(flexPins[i]), 0, 4095, 0, 1023);
      delay(5);
    }
    idleValues[i] = sum / 100;
    
    // CLOSE MARGIN: 6 for thumb (i=0), 9 for others
    if (i == 0) {
      closeThresholds[i] = idleValues[i] - 4; // Thumb
    } else {
      closeThresholds[i] = idleValues[i] - 9; // Other fingers
    }
    
    // OPEN MARGIN: 1 unit ABOVE close threshold
    openThresholds[i] = closeThresholds[i] + 1; 
    
    Serial.printf("Pin %d: Idle=%d | Close=%d | Open=%d\n", 
                  flexPins[i], idleValues[i], closeThresholds[i], openThresholds[i]);
  }

  SerialBT.begin("Glove_Controller", true); 
}

int getSmoothedValue(int pin) {
  long sum = 0;
  for(int i=0; i<30; i++) {
    sum += map(analogRead(pin), 0, 4095, 0, 1023);
    delayMicroseconds(50);
  }
  return sum / 30;
}

void loop() {
  if (!SerialBT.connected()) {
    SerialBT.connect(armAddress);
    delay(1000);
    return;
  }

  int r[5];
  for(int i=0; i<5; i++) r[i] = getSmoothedValue(flexPins[i]);

  // Close: value drops below close threshold
  // Open: value rises 1 unit above close threshold
  if (myData.f1 == 0 && r[0] < closeThresholds[0]) myData.f1 = 1;
  else if (myData.f1 == 1 && r[0] >= openThresholds[0]) myData.f1 = 0;

  if (myData.f2 == 0 && r[1] < closeThresholds[1]) myData.f2 = 1;
  else if (myData.f2 == 1 && r[1] >= openThresholds[1]) myData.f2 = 0;

  if (myData.f3 == 0 && r[2] < closeThresholds[2]) myData.f3 = 1;
  else if (myData.f3 == 1 && r[2] >= openThresholds[2]) myData.f3 = 0;

  if (myData.f4 == 0 && r[3] < closeThresholds[3]) myData.f4 = 1;
  else if (myData.f4 == 1 && r[3] >= openThresholds[3]) myData.f4 = 0;

  if (myData.f5 == 0 && r[4] < closeThresholds[4]) myData.f5 = 1;
  else if (myData.f5 == 1 && r[4] >= openThresholds[4]) myData.f5 = 0;

  SerialBT.write((uint8_t*)&myData, sizeof(myData));
  
  Serial.printf("T:%d I:%d M:%d R:%d P:%d | Sent: %d%d%d%d%d\n", 
                r[0], r[1], r[2], r[3], r[4], myData.f1, myData.f2, myData.f3, myData.f4, myData.f5);
  
  delay(40); 
}
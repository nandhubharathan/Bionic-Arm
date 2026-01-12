#include <BluetoothSerial.h>
#include <ESP32Servo.h>

BluetoothSerial SerialBT;
Servo servos[5];
int servoPins[] = {13, 12, 14, 27, 26}; 

typedef struct {
  uint16_t f1, f2, f3, f4, f5;
} struct_message;

struct_message receivedData;

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < 5; i++) {
    servos[i].setPeriodHertz(50);    // Standard 50Hz for SG90/MG996R
    servos[i].attach(servoPins[i], 500, 2400); // Standard pulse width
    servos[i].write(0); 
  }
  SerialBT.begin("RoboHand_Arm");
}

void loop() {
  if (SerialBT.available() > 0) {
    // If we have too much data, the buffer is unaligned. Flush it.
    if (SerialBT.available() > 30) { 
        while(SerialBT.available()) SerialBT.read();
        return;
    }

    if (SerialBT.available() >= sizeof(receivedData)) {
      SerialBT.readBytes((uint8_t*)&receivedData, sizeof(receivedData));

      // Debug: Checking for the 1 vs 0
      Serial.printf("T:%u I:%u M:%u R:%u P:%u\n", 
                    receivedData.f1, receivedData.f2, receivedData.f3, 
                    receivedData.f4, receivedData.f5);

      // Execute movement
      servos[0].write(receivedData.f1 == 1 ? 180 : 0);
      servos[1].write(receivedData.f2 == 1 ? 180 : 0);
      servos[2].write(receivedData.f3 == 1 ? 180 : 0);
      servos[3].write(receivedData.f4 == 1 ? 180 : 0);
      servos[4].write(receivedData.f5 == 1 ? 180 : 0);
    }
  }
}
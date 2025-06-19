// uses VSPI on esp32, sd card on HSPI

#include <SPI.h>
#include <LoRa.h>

#define LORA_CS   5
#define LORA_RST  27
#define LORA_IRQ  2

const unsigned long switchInterval = 5000;
unsigned long lastSwitchTime = 0;
bool isTransmitting = true;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // LoRa init
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(915.2E6)) {
    Serial.println("RFM95 init failed!");
    while (1);
  }

  Serial.println("RFM95 initialized.");
}

void loop() {
  unsigned long now = millis();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }
    int rssi = LoRa.packetRssi();
    Serial.print("Received: ");
    Serial.print(incoming);
    Serial.print(" | RSSI: ");
    Serial.println(rssi);
  }

  // Switch modes every interval
  if (now - lastSwitchTime >= switchInterval) {
    lastSwitchTime = now;

    if (isTransmitting) {
      String message = "Chirp @ " + String(now);
      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();
      Serial.println("Sent: " + message);
    } else {
      Serial.println("Listening for chirps...");
    }

    isTransmitting = !isTransmitting;
  }
}

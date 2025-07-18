#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LoRa.h>
#include <SPI.h>
#include <SD.h>

// gps
#define GPS_RX 16
#define GPS_TX 17
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

//mpu
Adafruit_MPU6050 mpu;

//lora
#define LORA_CS   5
#define LORA_RST  27
#define LORA_IRQ  2

// sd card
#define SD_CS     4
#define SD_MOSI   13
#define SD_MISO   25
#define SD_SCK    14

SPIClass hspi(HSPI);

const unsigned long INTERVAL = 2000;
unsigned long lastTime = 0;
bool isTransmitting = true; 
int lastRSSI = 0; //if nothing received

void setup() {
  Serial.begin(115200);
  delay(1000);

  //gps init
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("GPS OK");

  //imu init
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }
  Serial.println("IMU OK");

  // sd init
  hspi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, hspi)) {
    Serial.println("SD init failed!");
    while (1);
  }
  Serial.println("SD OK");

  // lora init
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(915.2E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  LoRa.setTxPower(20);
  Serial.println("LoRa OK");
}

void loop() {
  // gps data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  //lora parser -> returns rssi
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }
    lastRSSI = LoRa.packetRssi();
    Serial.print("RX packet: ");
    Serial.print(incoming);
    Serial.print(" | RSSI: ");
    Serial.println(lastRSSI);
  }

  // log every 2 sec
  if (millis() - lastTime >= INTERVAL) {
    lastTime = millis();

    // gps values
    String lat = gps.location.isValid() ? String(gps.location.lat(), 6) : "0.0";
    String lon = gps.location.isValid() ? String(gps.location.lng(), 6) : "0.0";
    String alt = gps.altitude.isValid() ? String(gps.altitude.meters()) : "0.0";

    // imu values
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    String ax = String(a.acceleration.x, 2);
    String ay = String(a.acceleration.y, 2);
    String az = String(a.acceleration.z, 2);

    // lora switcher (transmitter/receiver)
    if (isTransmitting) {
      String message = "Chirp @ " + String(millis());
      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();
      Serial.println("Sent: " + message);
      // No RSSI this cycle
    } else {
      Serial.println("Listening...");
    }
    isTransmitting = !isTransmitting;  // switch mode

    // log all to sd card csv
    File logFile = SD.open("/log.csv", FILE_APPEND);
    if (logFile) {
      String row = lat + "," + lon + "," + alt + ",";
      row += ax + "," + ay + "," + az + ",";
      row += String(lastRSSI);  // lastRSSI gets updated on receive
      logFile.println(row);
      logFile.close();
      Serial.println("Logged: " + row);
    } else {
      Serial.println("SD write failed.");
    }
  }
}
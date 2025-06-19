#include <TinyGPSPlus.h>

#define GPS_RX 16 
#define GPS_TX 17 

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

const unsigned long gpsInterval = 2000;
unsigned long lastGPSTime = 0;

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX); 
  Serial.println("GPS initialized");
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // logging - add sd later
  if (millis() - lastGPSTime >= gpsInterval) {
    lastGPSTime = millis();

    if (gps.location.isValid()) {
      Serial.print("Lat: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print("  Lng: ");
      Serial.print(gps.location.lng(), 6);
      Serial.print("  Alt: ");
      Serial.print(gps.altitude.meters());
      Serial.print(" m  Sats: ");
      Serial.print(gps.satellites.value());
      Serial.print("  HDOP: ");
      Serial.println(gps.hdop.hdop());
    } else {
      Serial.println("Can't find satellites");
    }
  }
}

*/
# Synthetic Aperture Balloon
This is a remote-sensing project intended to be done on a High Altitude Balloon launched in Boston for the Hack Club Hackathon Apex. We are trying to simulate Synthetic Aperture Radar on a low budget, using limited data, but lots of smart post-processing as a proof of concept.

## Our Payload
We are sending an IMU, GPS module, and a LoRa Radio attached to a directional antenna that will send chirps downward. An onboard ESP-32 will log the data collected from each of the sensors. Using the orientation, position, and RSSI collected and logged at set intervals, we will then use data processing Python libraries to simulate an SAR scan and relate it to the terrain that was actually mapped. The full BOM can be found on [this spreadsheet](https://docs.google.com/spreadsheets/d/1EXQkzV46__q9V7-iupEEt-R7V52zfWwjpi_Zgep5wXs/edit?usp=sharing).

### The IMU
We're using a breakout board for the MPU 6050 called the GY-521 and this will sense changes in orientation and the data will be sent to the ESP-32 via I2C and logged onto a microSD card.

### The GPS
We're using a breakout board for the NEO6M V2 which will communicate with the ESP-32 via UART and the altitude, and position data will be logged onto the microSD card.

### The LoRa Radio Transceiver
We're using Adafruit's breakout board for the RFM95W and directly connecting it to a patch antenna. I'm a little worried about the range, but it's hard to find a directional antenna that will allow our payload to stay within the weight constraints while also having a high enough range.

## Acknowledgements
Big thank you to Hack Club and the Apex organizers for giving us this amazing opportunity to run an experiment on a high-altitude balloon!

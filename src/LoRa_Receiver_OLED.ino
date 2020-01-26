#include <SPI.h>
#include <LoRa.h>
#include <U8x8lib.h>

#define ss 18
#define rst 14
#define dio0 26

const byte TXLED  = 25;

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);

String RxString, RxString1, RxString2, RxString3;
String RxRSSI;
int x=0,y=0;

void setup() {
  Serial.begin(115200);
  //while (!Serial);

  Serial.println("APRS LoRa RX");

  // pinMode(TXLED, OUTPUT);
  // digitalWrite(TXLED, LOW);
  u8x8.begin();

  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  u8x8.drawString(0, 0, "APRS LoRa RX");
  u8x8.setFont(u8x8_font_5x7_r);

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433.775E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(12); //
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
}


void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    // digitalWrite(TXLED, HIGH);
    x=-4;
    y=1;
    while (LoRa.available()) {
      RxString = (char)LoRa.read();
      Serial.print(RxString);
      ++x;
      if (x >= 0) {
        u8x8.setCursor(x, y);
        u8x8.print(RxString);
        if (x == 15) {
          x = -1;
          ++y;
        }
      }
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    RxRSSI = LoRa.packetRssi();
    Serial.println(RxRSSI);
    u8x8.setCursor(0, 7);
    u8x8.print("RSSI: ");
    u8x8.print(RxRSSI);
    u8x8.print(" dBm");
    // digitalWrite(TXLED, LOW);
  }


}

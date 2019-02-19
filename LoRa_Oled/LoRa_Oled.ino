/*
 * Author:  wagadai
 * Date:  02.19.2019
 * Version: 1.0.1
 */
//===============================================================================
/*
 * LoRa interfaces with I2C Oled
 * Oled   |   Atmega328 Uno  |   SX1278
 * SDA    -     SDA
 * SCL    -     SCL
 * 3V3    -     3V3      -  3V3
 * GND    -     GND      -  GND
 *        -     PB1      -  RST
 *        -     PD2      -  DIO0
 *        -     MOSI     -  MOSI
 *        -     MISO     -  MISO
 *        -     SCK      -  SCK
 *        -     SS       -  SS
 */
#include <SPI.h>
#include <LoRa.h>

// OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

static int pkg_num = 0;
static float snr_total = 0;
static int rssi_total = 0;

//================================================================================
//  oledDisplay
//================================================================================
void oledDisplay(String string) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("-+-+-+-+-+-+-+-+-+-+-"); 
  display.setCursor(0,10);
  display.print(string);
  display.setCursor(0,20);
  display.print("-+-+-+-+-+-+-+-+-+-+-");  
  display.display();
  delay(2000);
}

//================================================================================
//  oledDisplayPacketInfo
//================================================================================
void oledDisplayPacketInfo( int packet_numb, int rssi, float snr ){
  rssi_total = rssi_total + rssi;
  snr_total = snr_total + snr;
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  // Display Number of Packet Received
  display.setCursor(0,0);
  display.print("Recv Packet: "); 
  display.print(packet_numb);
  // Display RSSI / Average RSSI
  display.setCursor(0,10);
  display.print("Rssi: "); 
  display.print(rssi);
  display.print(" / ");
  display.print((double)rssi_total/packet_numb);
  // Display SNR / Average SNR
  display.setCursor(0,20);
  display.print("Snr: ");
  display.print(snr);
  display.print(" / ");  
  display.print(snr_total/packet_numb);
  // Display command 
  display.display();
}

//================================================================================
//  setup
//================================================================================
void setup() {
  pinMode(3, OUTPUT); // Led Gpio 
  digitalWrite(3, HIGH);
  delay(1000);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oledDisplay("Oled Begin!");
  
  if (!LoRa.begin(433E6)) {
    oledDisplay("LoRa Failed!");
    while (1);
  }
  // CONFIGURATION: 433MHz, SF = 12, BW = 125K, CR = 4/5, PL = 8, Syncword = 0x12, CRC.
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3); 
  LoRa.setCodingRate4(5);
  LoRa.setPreambleLength(8);
  LoRa.setSyncWord(0x12);
  LoRa.enableCrc();
  oledDisplay("Setting done");
  oledDisplay("Begin Rx Range Test");
  digitalWrite(3, LOW );
  /* Register the receive callback */
  LoRa.onReceive(onReceive);
  /* Put the radio into receive mode */
  LoRa.receive();
  oledDisplayPacketInfo(0, 0, 0);
}

//================================================================================
//  loop
//================================================================================
void loop() {
  // do nothing
}

//================================================================================
//  onReceive
//================================================================================
/* Handle received packets */
void onReceive(int packetSize) {
  pkg_num = pkg_num + 1;
  oledDisplayPacketInfo(pkg_num, LoRa.packetRssi(), LoRa.packetSnr());
}
//================================================================================

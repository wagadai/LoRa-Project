#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*============================================================================
 * Definitions
==============================================================================*/
#define OLED_RESET 4

/*============================================================================
 * Definitions
==============================================================================*/
Adafruit_SSD1306 display(OLED_RESET);

/*============================================================================
 * public functions
==============================================================================*/
//=========================================================
//  oled_setup
//=========================================================
void oled_setup() {
  Wire.begin();  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled_display("Oled Begin!");
}
//=========================================================
//  oled_display
//=========================================================
void oled_display(String string) {
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

//=========================================================
//  oled_display_packet_info
//=========================================================
void oled_display_packet_info( int packet_numb, int rssi, float snr ){
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
/*============================================================================
 * EOF
==============================================================================*/

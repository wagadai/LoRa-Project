#include <SPI.h>
#include <LoRa.h>

/*============================================================================
 * struct
==============================================================================*/
struct LoRa_parameter { // default parameters
  long freq_hz = 433E6;
  int sf = 12;
  long bw = 125E3;
  int _cr = 5;
  long pl = 8;
  int sw = 0x12;
  bool crc = true;
};

/*============================================================================
 * Global variables
==============================================================================*/

static int pkg_num = 0;
static float snr_total = 0;
static int rssi_total = 0;

/*============================================================================
 * setup
==============================================================================*/
void setup() {
  Serial.begin(9600);
  // Setup oled
  oled_setup();
  
  // Setup LoRa
  if (LoRa_setup() == false) {
    oled_display("LoRa Failed!.. while(1)");
    while(1);
  }

  // DEBUG
  oled_display("Setting done");
  oled_display("Begin Rx Range Test");

  /* Register the receive callback */
  LoRa.onReceive(onReceive);
  /* Put the radio into receive mode */
  LoRa.receive();
  oled_display_packet_info(0, 0, 0);
}

/*============================================================================
 * loop
==============================================================================*/
void loop() {
  // do nothing
}

/*============================================================================
 * onReceive
==============================================================================*/
/* Handle received packets */
void onReceive(int packetSize) {
  pkg_num = pkg_num + 1;
  oled_display_packet_info(pkg_num, LoRa.packetRssi(), LoRa.packetSnr());
}

/*============================================================================
 * LoRa_setup
==============================================================================*/
bool LoRa_setup() {
  struct LoRa_parameter lora_param;

  Serial.println(lora_param.freq_hz);
  /* Check if EEPROM is empty */
  if (is_eeprom_empty() == true) 
    /* Write new parameters to EEPROM if ERPROM is empty */
    {
      eeprom_put(lora_param);
    }
    
  /* Get parameters from EEPROM */
  else lora_param = eeprom_get();

  /* Debug LoRa Parameters */
  Serial.print(F("Freq: "));  Serial.println(lora_param.freq_hz);
  Serial.print(F("sf: "));    Serial.println(lora_param.sf);
  Serial.print(F("bw: "));    Serial.println(lora_param.bw);
  Serial.print(F("_cr: "));   Serial.println(lora_param._cr);
  Serial.print(F("pl: "));    Serial.println(lora_param.pl);
  Serial.print(F("sw: "));    Serial.println(lora_param.sw);
  Serial.print(F("crc: "));   Serial.println(lora_param.crc);
  
  /* Setup LoRa Parameters */
  if (!LoRa.begin(lora_param.freq_hz)) {
    Serial.println(F("LoRa Failed!"));
    return false;
  }
  LoRa.setSpreadingFactor(lora_param.sf);
  LoRa.setSignalBandwidth(lora_param.bw); 
  LoRa.setCodingRate4(lora_param._cr);
  LoRa.setPreambleLength(lora_param.pl);
  LoRa.setSyncWord(lora_param.sw);
  if (lora_param.crc == true) LoRa.enableCrc();
  else LoRa.disableCrc();
  
  return true;
}
//====================EOF============================================================

#include <EEPROM.h>

void eeprom_put(struct LoRa_parameter para){
  Serial.println(F("EEPROM: Putting LoRa parameters to EEPROM...."));
  EEPROM.write(0, 0xAA);  // 0xAA: epprom is not empty
  EEPROM.put(1, para);
  Serial.println(F("EEPROM: Write LoRa parameters to EEPROM Succeeded!"));
}

struct LoRa_parameter eeprom_get(){
  Serial.println(F("EEPROM: Getting LoRa parameters from EEPROM...."));
  struct LoRa_parameter _para;
  EEPROM.get(1, _para);
  Serial.println(F("EEPROM: Get LoRa parameters from EEPROM Succeeded!"));
  return _para;
}

bool is_eeprom_empty() {
  if (EEPROM.read(0) == 0xFF) {
    Serial.println(F("EEPROM: EEPROM is empty"));
    return true;
   }
   return false;
}

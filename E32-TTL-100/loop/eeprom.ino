#include <EEPROM.h>

void eeprom_write(uint8_t node_addr){
  Serial.println(F("Writing Node Address to EEPROM...."));
  EEPROM.write(0, node_addr);
  Serial.print(F("Node Address: 0x")); Serial.println(node_addr, HEX);
}

uint8_t eeprom_read(){
  Serial.println(F("Reading Node Address from EEPROM...."));
  uint8_t node_addr = EEPROM.read(0);
  Serial.print(F("Node Address: 0x"));  Serial.println(node_addr, HEX);
  return node_addr;
}

uint8_t get_address_from_eeprom(){
  #ifdef EEPROM_WRITE
  eeprom_write(NODE_ADDRESS_WRITE_TO_EEPROM);
  #endif
  return eeprom_read();
}

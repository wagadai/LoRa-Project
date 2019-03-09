uint8_t node_address;

void setup(){
  Serial.begin(9600);
  #ifdef TEST_BENCH
  test_bench();
  #endif
  
  node_address = get_address_from_eeprom();
  
  aodv_protocol();
}

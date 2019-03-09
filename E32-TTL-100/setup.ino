void setup(){
  Serial.begin(9600);
  Serial.println("setup");
  eeprom();
  aodv_protocol();
}

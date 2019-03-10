#include <SoftwareSerial.h>

/************************* ADDRESS *****************************/
#define HEADER          0xC0
#define NODE_AddressH   0xBB
#define NODE_AddressL   0x11
#define NODE_Channel    0x17
#define SPEED           0x1D  // 19.2 KBPS
#define OPTION          0xC4  //20dBm

/************************* PIN DEFINITION *****************************/
#define M0pin   A0
#define M1pin   A1
#define AUXpin  A2
#define SOFT_RX 7
#define SOFT_TX 6
#define TIMEOUT_RET   2
/************************* GLOBAL VARIABLE *****************************/
SoftwareSerial LoraSerial(SOFT_RX, SOFT_TX);

/************************* PUBLIC FUNCTIONS *****************************/
/*******************************************************************
 * E32_LoRa_setup()
********************************************************************/
bool E32_LoRa_setup() {
  Serial.println("E32-TTL-100: Configuring.......!");
  pinMode(M0pin, OUTPUT);
  pinMode(M1pin, OUTPUT);
  pinMode(AUXpin, INPUT);
  LoraSerial.begin(9600);

  // configure
  Serial.println("E32-TTL-100: Mode 3: Configuring Mode");
  digitalWrite(M0pin, HIGH);
  digitalWrite(M1pin, HIGH);
  delay(10);
  if(_wait_AUX_high() == TIMEOUT_RET) {exit(0);}
  uint8_t CMD[6] = {HEADER, NODE_AddressH, NODE_AddressL, SPEED, NODE_Channel, OPTION};
  LoraSerial.write(CMD, 6);
  _wait_AUX_high();
  delay(1200);
  Serial.println("E32-TTL-100: Setting Configure has been sent!");

  //Read Set configure
  while(LoraSerial.available()){LoraSerial.read();} //Clean Uart Buffer
  uint8_t READCMD[3] = {0xC1, 0xC1, 0xC1};
  LoraSerial.write(READCMD, 3);
  _wait_AUX_high();
  delay(50);
  Serial.println("E32-TTL-100: Reading Configure has been sent!");
  uint8_t readbuf[6];
  uint8_t Readcount, idx, buff;
  Readcount = LoraSerial.available();
  if (Readcount){
      Serial.println("E32-TTL-100: Setting Configure is:  ");
      for(idx = 0; idx < Readcount; idx++) {
          readbuf[idx] = LoraSerial.read();
          delay(10);
      }
      Serial.print("Shutdown Mode: ");Serial.println(0xFF & readbuf[0],HEX);
      Serial.print("Address High: ");Serial.println(0xFF & readbuf[1],HEX);
      Serial.print("Address Low: ");Serial.println(0xFF & readbuf[2],HEX);
      Serial.print("Speed: ");Serial.println(0xFF & readbuf[3],HEX);
      Serial.print("Channel: ");Serial.println(0xFF & readbuf[4],HEX);
      Serial.print("Option: ");Serial.println(0xFF & readbuf[5],HEX);

      if ((readbuf[0] == HEADER) && (readbuf[1] == NODE_AddressH ) && (readbuf[2] == NODE_AddressL )  
                    && (readbuf[3] == SPEED) && (readbuf[4] == NODE_Channel ) && (readbuf[5] == OPTION) ) {
          digitalWrite(M0pin, LOW);
          digitalWrite(M1pin, LOW);
          Serial.println("E32-TTL-100: Mode 0: Normal: Ready...!");
          return true;
      }
  }
  Serial.println("E32-TTL-100: FAILED to configure!");
  return false;
}
/*******************************************************************
 * E32_LoRa_send()
********************************************************************/
void E32_LoRa_send(uint8_t *payload, uint8_t nb_byte) {
    // uint8_t FrameSend[8] = {GW_AddressH, GW_AddressL, GW_Channel, NODE_AddressH, NODE_AddressL, 0x01, 0x02, 0x03};
//    LoraSerial.write(payload, nb_byte);
//    #ifdef DEBUG_SERIAL
//    Serial.println("Data hasbeen sent!");
//    #endif // DEBUG_SERIAL
//    ledToggle(1, true, false, 500);
//    delay(10);
}

/*******************************************************************
 * E32_LoRa_listen()
********************************************************************/
int E32_LoRa_listen() {
//    delay(100);
//    int data_len;
//    data_len = LoraSerial.available();
//    if (data_len > 0) {
//        #ifdef DEBUG_SERIAL
//        Serial.print("ReceiveMsg: ");  Serial.print(data_len);  Serial.println(" bytes.");
//        #endif // DEBUG_SERIAL
//        ledToggle(1, false, true, 500);
//    }
//    return data_len;
}

/*******************************************************************
 * E32_LoRa_fetch()
********************************************************************/
void E32_LoRa_fetch(int data_len, uint8_t *pdata) {
//  if (data_len > 0) {
//      for(int i=0;i<data_len;i++) {
//          pdata[i] = LoraSerial.read();
//          #ifdef DEBUG_SERIAL
//          Serial.print(pdata[i]);
//          Serial.print(" ");
//          #endif // DEBUG_SERIAL
//      }
//      #ifdef DEBUG_SERIAL
//      Serial.println("");
//      #endif // DEBUG_SERIAL
//  }
}
/*********************** PRIVATE FUNCTIONS *****************************/
/*******************************************************************
 * _read_AUX()
********************************************************************/
bool _read_AUX() {
  bool AUX_value;
  int val = analogRead(AUXpin);
  if(val<50){
      AUX_value = LOW;
  }else {
      AUX_value = HIGH;
  }
  return AUX_value;
}
/*******************************************************************
 * _wait_AUX_high()
********************************************************************/
int _wait_AUX_high() {
  int ret_var = 0;
  uint8_t cnt = 0;
  while ((_read_AUX() == LOW) && (cnt++ < 100)){
      Serial.print(".");
      delay(100);
  }
  if (cnt >= 100 ) {
      ret_var = TIMEOUT_RET;
      Serial.println("E32-TTL-100: Timeout!");
  }
  return ret_var;
}

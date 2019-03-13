#include <SoftwareSerial.h>

/************************* ADDRESS *****************************/
#define HEADER          0xC0
#define NODE_AddressH   0xBB
#define NODE_AddressL   0x11
#define CHANNEL    0x17
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

//==================================================================//
//====================== PRIVATE FUNCTIONS =========================*/
//==================================================================//
/*******************************************************************
 * _E32_LoRa_setup()
********************************************************************/
bool _E32_LoRa_setup() {
  Serial.println(F("E32-TTL-100: Configuring.......!"));
  pinMode(M0pin, OUTPUT);
  pinMode(M1pin, OUTPUT);
  pinMode(AUXpin, INPUT);
  LoraSerial.begin(9600);

  // configure
  Serial.println(F("E32-TTL-100: Mode 3: Configuring Mode"));
  digitalWrite(M0pin, HIGH);
  digitalWrite(M1pin, HIGH);
  delay(10);
  if(__wait_AUX_high() == TIMEOUT_RET) {exit(0);}
  uint8_t CMD[6] = {HEADER, NODE_AddressH, NODE_AddressL, SPEED, CHANNEL, OPTION};
  LoraSerial.write(CMD, 6);
  __wait_AUX_high();
  delay(1200);
  Serial.println(F("E32-TTL-100: Setting Configure has been sent!"));

  //Read Set configure
  while(LoraSerial.available()){LoraSerial.read();} //Clean Uart Buffer
  uint8_t READCMD[3] = {0xC1, 0xC1, 0xC1};
  LoraSerial.write(READCMD, 3);
  __wait_AUX_high();
  delay(50);
  Serial.println(F("E32-TTL-100: Reading Configure has been sent!"));
  uint8_t readbuf[6];
  uint8_t Readcount, idx, buff;
  Readcount = LoraSerial.available();
  if (Readcount){
      Serial.println(F("E32-TTL-100: Setting Configure is:  "));
      for(idx = 0; idx < Readcount; idx++) {
          readbuf[idx] = LoraSerial.read();
          delay(10);
      }
      Serial.print(F("Shutdown Mode: "));Serial.println(0xFF & readbuf[0]);
      Serial.print(F("Address High: "));Serial.println(0xFF & readbuf[1]);
      Serial.print(F("Address Low: "));Serial.println(0xFF & readbuf[2]);
      Serial.print(F("Speed: "));Serial.println(0xFF & readbuf[3]);
      Serial.print(F("Channel: "));Serial.println(0xFF & readbuf[4]);
      Serial.print(F("Option: "));Serial.println(0xFF & readbuf[5]);

      if ((readbuf[0] == HEADER) && (readbuf[1] == NODE_AddressH ) && (readbuf[2] == NODE_AddressL )  
                    && (readbuf[3] == SPEED) && (readbuf[4] == CHANNEL) && (readbuf[5] == OPTION) ) {
          digitalWrite(M0pin, LOW);
          digitalWrite(M1pin, LOW);
          Serial.println(F("E32-TTL-100: Mode 0: Normal: Ready...!"));
          return true;
      }
  }
  Serial.println(F("E32-TTL-100: FAILED to configure!"));
  return false;
}
/*******************************************************************
 * _E32_LoRa_listen()
********************************************************************/
bool _E32_LoRa_listen(uint8_t *message_received) {
  uint8_t message_length = 0;
  
  message_length = LoraSerial.available();
  if (message_length > 0) {
    // Fetch received packet
    Serial.print(F("E32_TTL_100: Receive message: "));  Serial.print(message_length);  Serial.println(F(" bytes."));
    Serial.print(F("E32_TTL_100: Message is: {"));
    for (int i=0; i < message_length; i++ ) {
      *(message_received + i) = LoraSerial.read();
      Serial.print(*(message_received + i)); Serial.print(F(", "));
    }
    Serial.println(F("}"));
    return true;
  }
  return false;
}
/*******************************************************************
 * __read_AUX()
********************************************************************/
bool __read_AUX() {
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
 * __wait_AUX_high()
********************************************************************/
int __wait_AUX_high() {
  int ret_var = 0;
  uint8_t cnt = 0;
  while ((__read_AUX() == LOW) && (cnt++ < 100)){
      Serial.print(F("."));
      delay(100);
  }
  if (cnt >= 100 ) {
      ret_var = TIMEOUT_RET;
      Serial.println(F("E32-TTL-100: Timeout!"));
  }
  return ret_var;
}

//==================================================================//
//====================== PUBLIC FUNCTIONS =========================*/
//==================================================================//
/*******************************************************************
 * LoRa_broadcast()
********************************************************************/
void LoRa_broadcast(uint8_t *message) {
  Serial.println(F("E32_TTL_100: Sending broadcast message....."));
  uint8_t broadcast_addr[3] = {0xFF, 0xFF, CHANNEL};
  uint8_t frame_to_send[_sizeof(message) + sizeof(broadcast_addr)];  //TODO: test
  
  memcpy(frame_to_send, broadcast_addr, sizeof(broadcast_addr));
  memcpy(frame_to_send + sizeof(broadcast_addr), message, _sizeof(message));
  LoraSerial.write(frame_to_send, sizeof(frame_to_send));
  Serial.println(F("E32_TTL_100: broadcast message has been sent!"));
}
/*******************************************************************
 * LoRa_unicast()
********************************************************************/
void LoRa_unicast(struct send_address* send_to, uint8_t* message) {
  Serial.println(F("E32_TTL_100: Sending unicast message....."));
  uint8_t channel = CHANNEL;
  uint8_t frame_to_send[sizeof(send_address) + _sizeof(message)];

  memcpy(frame_to_send, send_to, sizeof(send_address));
  memcpy(frame_to_send + sizeof(send_address), channel, sizeof(channel));
  memcpy(frame_to_send + sizeof(send_address) + sizeof(channel), message, _sizeof(message));
  LoraSerial.write(frame_to_send, sizeof(frame_to_send));
  Serial.println(F("E32_TTL_100: unicast message has been sent!"));
}
/*******************************************************************
 * LoRa_listen()
********************************************************************/
uint8_t LoRa_listen(uint8_t *message) {
  if (_E32_LoRa_listen(message) == true) {
  // Check type of message: RREQ, RREP, RERR, data_message?
    switch (*message) {
      case RREQ_MESSAGE_TYPE:
        Serial.println(F("E32_TTL_100: Receive RREQ_message"));
        return RREQ_MESSAGE_TYPE;
      case RREP_MESSAGE_TYPE:
        Serial.println(F("E32_TTL_100: Receive RREP_message"));
        return RREP_MESSAGE_TYPE;
      case RERR_MESSAGE_TYPE:
        Serial.println(F("E32_TTL_100: Receive RERR_message"));
        return RERR_MESSAGE_TYPE;
      case DATA_MESSAGE_TYPE:
        Serial.println(F("E32_TTL_100: Receive data_message"));
        return DATA_MESSAGE_TYPE;
//      case ACK_MESSAGE_TYPE:
//        Seria.println("E32_TTL_100: Receive ack_message"));
//        return ACK_MESSAGE_TYPE;
      default:
        Serial.println(F("E32_TTL_100: ERROR"));
        return 0x00;
    }
  }
  return 0x00; // NOT RECEIVE ANY PACKET
}

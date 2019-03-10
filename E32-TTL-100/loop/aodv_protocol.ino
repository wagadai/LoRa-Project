void aodv_protocol(){
  Serial.println("aodv_protocol");
}

void broadcast_RREQ(struct RREQ_message* RREQ) {
  LoRa_broadcast(RREQ);
}

void listen_message(){
  uint8_t* message;
  uint8_t message_t = LoRa_listen(message);
  switch (message_t) {
    case RREQ_MESSAGE_TYPE:
      // receive RREQ_message
      struct RREQ_message* _RREQ;
      memcpy(_RREQ, message, sizeof(message));
      receive_RREQ_message(_RREQ);
      break;
    case RREP_MESSAGE_TYPE:
      // receive RREP_message
//        receive_RREP_message(message);
      break;
    case RERR_MESSAGE_TYPE:
      // receive RREP_message
//      receive_RERR_message(message);
      break;
    case DATA_MESSAGE_TYPE:
      // receive data_message
//      receive_data_message(message);
    default:
      // not receive any packet
      break;
  }
}

void receive_RREQ_message(struct RREQ_message* _RREQ) {
  
}

void receive_RREP_message() {
  
}

void receive_RERR_message() {
  
}

void receive_data_message() {
  
}

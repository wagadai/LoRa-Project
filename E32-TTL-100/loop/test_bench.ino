/*******************************************************************
 * OVERALL
********************************************************************/
void test_bench() {
  #ifdef MESSAGE_TEST
  RREQ_message_test();
//  RREP_message_test();
//  RERR_message_test();
//  routing_table_test();
  while(1);
  #endif

  #ifdef E32_TTL_100_TEST
  _E32_LoRa_setup_test();
  _E32_LoRa_send_test();
  _E32_LoRa_listen_test();
  _E32_LoRa_fetch_test();
  #endif
}
/*******************************************************************
 * MESSAGE
********************************************************************/
void RREQ_message_test(){
  Serial.println("TEST: Testing RREQ_message_test....");
  Serial.println(sizeof(struct RREQ_message));
  if (sizeof(struct RREQ_message) != 12) {
    Serial.println("TEST: ERROR: size of RREQ_message is not compatible");
    return;
  }
  struct RREQ_message* RREQ;
  memset(RREQ, 0, sizeof(struct RREQ_message));
  RREQ->type = 0x73;                  Serial.print(RREQ->type, HEX);
  RREQ->dest.address_high = 0x22;     Serial.print(RREQ->dest.address_high, HEX);
  RREQ->dest.address_low = 0x33;      Serial.print(RREQ->dest.address_low, HEX);
  RREQ->dest.sequence_number = 0x44;  Serial.print(RREQ->dest.sequence_number, HEX);
  RREQ->src.address_high = 0x55;      Serial.print(RREQ->src.address_high, HEX);
  RREQ->src.address_low = 0x66;       Serial.print(RREQ->src.address_low, HEX);
  RREQ->src.sequence_number = 0x77;   Serial.print(RREQ->src.sequence_number, HEX);
  RREQ->hop_count = 0x66;             Serial.print(RREQ->hop_count, HEX);
  RREQ->broadcast_id = 0x88;          Serial.print(RREQ->broadcast_id, HEX);
  RREQ->reverse_path = 0xAA;          Serial.print(RREQ->reverse_path, HEX);
  RREQ->send_from.address_high = 0xBB;Serial.print(RREQ->send_from.address_high, HEX);
  RREQ->send_from.address_low = 0xCC; Serial.println(RREQ->send_from.address_low, HEX);

  // Copy another_array & RREQ to frame_to_send:
  uint8_t frame_to_send[sizeof(struct RREQ_message) + 3];
  uint8_t another_array[] = {0xAA, 0xBB, 0xCC};
  memcpy(frame_to_send, another_array, sizeof(another_array));
  memcpy(frame_to_send + sizeof(another_array), RREQ, sizeof(struct RREQ_message));
  Serial.print("TEST: frame_to_send:");
  Serial.println(frame_to_send[4], HEX);

  // Test copy 2 same-type struct
  Serial.println("TEST: copy 2 same-type struct");
  struct destination* dest;
  dest->address_high = 0xAA;
  dest->address_low = 0xBB;
  dest->sequence_number = 0xCC;
  
  RREQ->dest = *dest;
  Serial.print(RREQ->dest.address_high, HEX);
  Serial.print(RREQ->dest.address_low, HEX);
  Serial.println(RREQ->dest.sequence_number, HEX);

  // Test find pointer size
  uint8_t* message;
  memset(message, 0, sizeof(struct RREQ_message));
  memcpy(message, RREQ, sizeof(struct RREQ_message));
  Serial.print("TEST: Size of pointer: ");
  Serial.println(_sizeof(message));

  Serial.println("TEST: Test RREQ_message_test done!");
}

int _sizeof(uint8_t* pointer_array) {
  int _size = 0;
  for (uint8_t i = 0; i < 100; i++ ) {
    if (*(pointer_array + i) == 0 && *(pointer_array + i + 1) == 0) {
      return _size;
    }
    _size++;
  }
}


//void RREP_message_test(){
//  Serial.println("TEST: Testing RREP_message_test....");
//  if (sizeof(struct RREP_message) != 6) {
//    Serial.println("TEST: ERROR: size of RREP_message is not compatible");
//    return;
//  }
//  struct RREP_message* RREP;
//  RREP->dest_addr = 0x11;     Serial.print(RREP->dest_addr, HEX);
//  RREP->dest_seq_no = 0x22;   Serial.print(RREP->dest_seq_no, HEX);
//  RREP->source_addr = 0x33;   Serial.print(RREP->source_addr, HEX);
//  RREP->source_seq_no = 0x44; Serial.print(RREP->source_seq_no, HEX);
//  RREP->hop_count = 0x55;     Serial.println(RREP->hop_count, HEX);
//  Serial.println("TEST: Test RREP_message done!");
//}
//
//void RERR_message_test(){
//  Serial.println("Testing RERR_message_test....");
//  if (sizeof(struct RERR_message) != 3) {
//    Serial.println("TEST: ERROR: size of RERR_message is not compatible");
//    return;
//  }
//  struct RERR_message* RERR;
//  RERR->dest_addr_unreachable = 0x11;   Serial.print(RERR->dest_addr_unreachable, HEX);
//  RERR->dest_seq_no_unreachable = 0x22; Serial.println(RERR->dest_seq_no_unreachable, HEX);
//  Serial.println("TEST: Test RERR_message done!");  
//}
//
//void routing_table_test(){
//  Serial.println("TEST: Testing routing_table....");
//  if (sizeof(struct routing_table) != 4) {
//    Serial.println("TEST: ERROR: size of routing_table is not compatible");
//    return;
//  }
//  struct routing_table* rt_tbl;
//  rt_tbl->node_addr = 0x11; Serial.print(rt_tbl->node_addr, HEX);
//  rt_tbl->next_hop = 0x22;  Serial.print(rt_tbl->next_hop, HEX);
//  rt_tbl->seq_no = 0x33;    Serial.print(rt_tbl->seq_no, HEX);
//  rt_tbl->hop_count = 0x44; Serial.println(rt_tbl->hop_count, HEX);
//  Serial.println("TEST: Test routing_table done!");  
//}
/*******************************************************************
 * E32_TTL_100 LORA
********************************************************************/
void _E32_LoRa_setup_test() {
  Serial.println("TEST: Testing _E32_LoRa_setup_test....");
  if (_E32_LoRa_setup()) {
    Serial.println("TEST: _E32_LoRa_setup_test OK!");
    return;
  }
  Serial.println("TEST: _E32_LoRa_setup_test ERROR!");
}

void _E32_LoRa_send_test() {
  Serial.println("TEST: Testing _E32_LoRa_send_test....");
  
}

void _E32_LoRa_listen_test() {
  Serial.println("TEST: Testing E32_LoRa_listen_test....");
  
}

void E32_LoRa_fetch_test() {
  Serial.println("TEST: Testing _E32_LoRa_fetch_test....");
  
}
/*******************************************************************
 * EOF
********************************************************************/

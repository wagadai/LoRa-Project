/*******************************************************************
 * OVERALL
********************************************************************/
void test_bench() {
  #ifdef MESSAGE_TEST
  RREQ_message_test();
  RREP_message_test();
  RERR_message_test();
  routing_table_test();
  while(1);
  #endif

  #ifdef E32_TTL_100_TEST
  E32_LoRa_setup_test();
  E32_LoRa_send_test();
  E32_LoRa_listen_test();
  E32_LoRa_fetch_test();
  #endif
}
/*******************************************************************
 * MESSAGE
********************************************************************/
void RREQ_message_test(){
  Serial.println("TEST: Testing RREQ_message_test....");
  if (sizeof(struct RREQ_message) != 6) {
    Serial.println("TEST: ERROR: size of RREQ_message is not compatible");
    return;
  }
  struct RREQ_message* RREQ;
  RREQ->broadcast_id = 0x11;  Serial.print(RREQ->broadcast_id, HEX);
  RREQ->dest_addr = 0x22;     Serial.print(RREQ->dest_addr, HEX);
  RREQ->dest_seq_no = 0x33;   Serial.print( RREQ->dest_seq_no, HEX);
  RREQ->source_addr = 0x44;   Serial.print(RREQ->source_addr, HEX);
  RREQ->source_seq_no = 0x55; Serial.print(RREQ->source_seq_no, HEX);
  RREQ->hop_count = 0x66;     Serial.println(RREQ->hop_count, HEX);
  Serial.println("TEST: Test RREQ_message_test done!");
}

void RREP_message_test(){
  Serial.println("TEST: Testing RREP_message_test....");
  if (sizeof(struct RREP_message) != 5) {
    Serial.println("TEST: ERROR: size of RREP_message is not compatible");
    return;
  }
  struct RREP_message* RREP;
  RREP->dest_addr = 0x11;     Serial.print(RREP->dest_addr, HEX);
  RREP->dest_seq_no = 0x22;   Serial.print(RREP->dest_seq_no, HEX);
  RREP->source_addr = 0x33;   Serial.print(RREP->source_addr, HEX);
  RREP->source_seq_no = 0x44; Serial.print(RREP->source_seq_no, HEX);
  RREP->hop_count = 0x55;     Serial.println(RREP->hop_count, HEX);
  Serial.println("TEST: Test RREP_message done!");
}

void RERR_message_test(){
  Serial.println("Testing RERR_message_test....");
  if (sizeof(struct RERR_message) != 2) {
    Serial.println("TEST: ERROR: size of RERR_message is not compatible");
    return;
  }
  struct RERR_message* RERR;
  RERR->dest_addr_unreachable = 0x11;   Serial.print(RERR->dest_addr_unreachable, HEX);
  RERR->dest_seq_no_unreachable = 0x22; Serial.println(RERR->dest_seq_no_unreachable, HEX);
  Serial.println("TEST: Test RERR_message done!");  
}

void routing_table_test(){
  Serial.println("TEST: Testing routing_table....");
  if (sizeof(struct routing_table) != 4) {
    Serial.println("TEST: ERROR: size of routing_table is not compatible");
    return;
  }
  struct routing_table* rt_tbl;
  rt_tbl->node_addr = 0x11; Serial.print(rt_tbl->node_addr, HEX);
  rt_tbl->next_hop = 0x22;  Serial.print(rt_tbl->next_hop, HEX);
  rt_tbl->seq_no = 0x33;    Serial.print(rt_tbl->seq_no, HEX);
  rt_tbl->hop_count = 0x44; Serial.println(rt_tbl->hop_count, HEX);
  Serial.println("TEST: Test routing_table done!");  
}
/*******************************************************************
 * E32_TTL_100 LORA
********************************************************************/
void E32_LoRa_setup_test() {
  Serial.println("TEST: Testing E32_LoRa_setup_test....");
  if (E32_LoRa_setup()) {
    Serial.println("TEST: E32_LoRa_setup_test OK!");
    return;
  }
  Serial.println("TEST: E32_LoRa_setup_test ERROR!");
}

void E32_LoRa_send_test() {
  Serial.println("TEST: Testing E32_LoRa_send_test....");
  
}

void E32_LoRa_listen_test() {
  Serial.println("TEST: Testing E32_LoRa_listen_test....");
  
}

void E32_LoRa_fetch_test() {
  Serial.println("TEST: Testing E32_LoRa_fetch_test....");
  
}
/*******************************************************************
 * EOF
********************************************************************/

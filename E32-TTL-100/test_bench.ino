/*******************************************************************
 * OVERALL
********************************************************************/
void test_bench() {
  RREQ_message_test();
  RREP_message_test();
  RERR_message_test();
  routing_table();
  while(1);
}
/*******************************************************************
 * MESSAGE
********************************************************************/
void RREQ_message_test(){
  Serial.println("Testing RREQ_message_test....");
  if (sizeof(struct RREQ_message) != 6) {
    Serial.println("ERROR: size of RREQ_message is not compatible");
    return;
  }
  struct RREQ_message* RREQ;
  RREQ->broadcast_id = 0x11;  Serial.print(RREQ->broadcast_id, HEX);
  RREQ->dest_addr = 0x22;     Serial.print(RREQ->dest_addr, HEX);
  RREQ->dest_seq_no = 0x33;   Serial.print( RREQ->dest_seq_no, HEX);
  RREQ->source_addr = 0x44;   Serial.print(RREQ->source_addr, HEX);
  RREQ->source_seq_no = 0x55; Serial.print(RREQ->source_seq_no, HEX);
  RREQ->hop_count = 0x66;     Serial.println(RREQ->hop_count, HEX);
  Serial.println("Test RREQ_message_test done!");
}

void RREP_message_test(){
  Serial.println("Testing RREP_message_test....");
  if (sizeof(struct RREP_message) != 5) {
    Serial.println("ERROR: size of RREP_message is not compatible");
    return;
  }
  struct RREP_message* RREP;
  RREP->dest_addr = 0x11;     Serial.print(RREP->dest_addr, HEX);
  RREP->dest_seq_no = 0x22;   Serial.print(RREP->dest_seq_no, HEX);
  RREP->source_addr = 0x33;   Serial.print(RREP->source_addr, HEX);
  RREP->source_seq_no = 0x44; Serial.print(RREP->source_seq_no, HEX);
  RREP->hop_count = 0x55;     Serial.println(RREP->hop_count, HEX);
  Serial.println("Test RREP_message done!");
}

void RERR_message_test(){
  Serial.println("Testing RERR_message_test....");
  if (sizeof(struct RERR_message) != 2) {
    Serial.println("ERROR: size of RERR_message is not compatible");
    return;
  }
  struct RERR_message* RERR;
  RERR->dest_addr_unreachable = 0x11;   Serial.print(RERR->dest_addr_unreachable, HEX);
  RERR->dest_seq_no_unreachable = 0x22; Serial.println(RERR->dest_seq_no_unreachable, HEX);
  Serial.println("Test RERR_message done!");  
}

void routing_table(){
  Serial.println("Testing routing_table....");
  if (sizeof(struct routing_table) != 4) {
    Serial.println("ERROR: size of routing_table is not compatible");
    return;
  }
  struct routing_table* rt_tbl;
  rt_tbl->node_addr = 0x11; Serial.print(rt_tbl->node_addr, HEX);
  rt_tbl->next_hop = 0x22;  Serial.print(rt_tbl->next_hop, HEX);
  rt_tbl->seq_no = 0x33;    Serial.print(rt_tbl->seq_no, HEX);
  rt_tbl->hop_count = 0x44; Serial.println(rt_tbl->hop_count, HEX);
  Serial.println("Test routing_table done!");  
}
/*******************************************************************
 * 
********************************************************************/

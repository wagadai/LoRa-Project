/*
 * sequence_number: increment when sending RREP
 * broadcast_id: increment when sending RREQ
 */

/*******************************************************************
 * GLOBAL VARIABLES
********************************************************************/
uint8_t broadcast_id = 0;

struct source* src;

struct routing_table rt_tbl[10];
uint8_t number_of_node = 0;

struct received_broadcast_message received_broadcast_table[10];
uint8_t number_of_node_in_broadcast_table = 0;

/*******************************************************************
 * aodv_protocol()
********************************************************************/
void aodv_protocol(){
  Serial.println("aodv_protocol");

  // ROUTE DISCOVERY:
  // Broadcast RREQ_message to node 1
  struct destination *node_1;
  node_1->address_high = 0x00;
  node_1->address_low = 0x01;
  broadcast_RREQ(node_1);
  // if receive RREQ_message: rebroadcast, update routing table; unicast RREP
}
/*******************************************************************
 * broadcast_RREQ()
********************************************************************/
void broadcast_RREQ(struct destination* dest) {
  Serial.println("AODV_PROTOCOL: Sending RREQ_broadcast message.....");
  // unknown sequence_number of destination: NaN = 255;
  dest->sequence_number = 255;

  // pack RREQ_message:
  struct RREQ_message *RREQ;
  RREQ->dest = *dest;
  RREQ->src = *src;
  RREQ->broadcast_id = broadcast_id++;
  RREQ->hop_count = 0;

  // convert struct to array
  uint8_t _message[sizeof(struct RREQ_message)];
  memcpy(_message, RREQ, sizeof(struct RREQ_message));
  
  // broadcast RREQ_message
  LoRa_broadcast(_message);
  Serial.println("AODV_PROTOCOL: broadcast RREQ_broadcast message has been sent!");
  
  // wait for RREP/ ACK
  listen_message();
}
/*******************************************************************
 * unicast_RREQ_ACK(): reversed_path
********************************************************************/
void unicast_RREQ_ACK(struct send_address* send_to, struct RREQ_message* _RREQ) {
  Serial.print("AODV_PROTOCOL: Unicasting RREQ_ACK message to: ");
  Serial.print(send_to->address_high);
  Serial.println(send_to->address_low);
  _RREQ->reverse_path = REVERSE_PATH;

  // convert struct to array
  uint8_t _message[sizeof(struct RREQ_message)];
  memcpy(_message, _RREQ, sizeof(struct RREQ_message));
  LoRa_unicast(send_to, _message);
  Serial.println("AODV_PROTOCOL: Unicast RREQ_ACK message DONE! ");
}
/*******************************************************************
 * unicast_RREP()
********************************************************************/
void unicast_RREP(struct send_address* send_to, struct RREP_message* _RREP, uint8_t is_ACK = 0x00) {
  if (is_ACK){
    Serial.print("AODV_PROTOCOL: Unicasting RREP_ACK message to: ");
    Serial.print(send_to->address_high);
    Serial.println(send_to->address_low);
    _RREP->reverse_path = REVERSE_PATH;
  }
  else {
    Serial.print("AODV_PROTOCOL: Unicasting RREP message to: ");
    Serial.print(send_to->address_high);
    Serial.println(send_to->address_low);
  }

  // convert struct to array
  uint8_t _message[sizeof(struct RREP_message)];
  memcpy(_message, _RREP, sizeof(struct RREP_message));
  LoRa_unicast(send_to, _message);
  Serial.println("AODV_PROTOCOL: Unicast RREP message DONE! ");

  if (is_ACK){}
  // wait for RREP/ ACK
  else listen_message();
}
/*******************************************************************
 * listen_message()
********************************************************************/
void listen_message(){
  uint8_t* message;
  uint8_t message_t = LoRa_listen(message);
  switch (message_t) {
    case RREQ_MESSAGE_TYPE: // receive RREQ_message
      
      struct RREQ_message* _RREQ;
      // convert array to struct
      memcpy(_RREQ, message, sizeof(message));
      receive_RREQ_message(_RREQ);
      break;
    case RREP_MESSAGE_TYPE: // receive RREP_message
      // convert array to struct
      struct RREP_message* _RREP;
      memcpy(_RREP, message, sizeof(message));
      receive_RREP_message(_RREP);
      break;
    case RERR_MESSAGE_TYPE:
      // receive RREP_message
//      receive_RERR_message(message);
      break;
    case DATA_MESSAGE_TYPE:
      // receive data_message
//      receive_data_message(message);
    case ACK_MESSAGE_TYPE:
//      received_ack_message(message);
    default:
      // not receive any packet
      break;
  }
}
/*******************************************************************
 * receive_RREQ_message()
********************************************************************/
void receive_RREQ_message(struct RREQ_message* _RREQ) {
  // check if reversed path (RREQ reverse = ACK)
  if (_RREQ->reverse_path == REVERSE_PATH ) {  // Yes
    // TURN OFF TIMER, no need to wait for ACK anymore.
    Timer1.stop();
  }
  else {
    // unicast ACK to send_from
    struct send_address* _send_to;
    _send_to->address_high = _RREQ->send_from.address_high;
    _send_to->address_low = _RREQ->send_from.address_low;
    unicast_RREQ_ACK(_send_to, _RREQ);
  }
  
  // check if duplicate RREQ? Based on Source address & broadcast_id
  bool destination_exist_in_broadcast_table = false;
  for (int i=0; i<number_of_node_in_broadcast_table; i++) {
    if (received_broadcast_table[i].src.address_high == _RREQ->src.address_high &&
          received_broadcast_table[i].src.address_low == _RREQ->src.address_low ) {
            destination_exist_in_broadcast_table = true;
            if (_RREQ->broadcast_id == received_broadcast_table[i].broadcast_id) {
              // duplicate
              return;
            }
            else if (_RREQ->broadcast_id == 0 || received_broadcast_table[i].broadcast_id < _RREQ->broadcast_id) {
              // update received_broadcast_table
              received_broadcast_table[i].broadcast_id = _RREQ->broadcast_id;
              break;
            }
          }
  }
  if (destination_exist_in_broadcast_table == false) {  // add new
    received_broadcast_table[number_of_node_in_broadcast_table].src.address_high = _RREQ->src.address_high;
    received_broadcast_table[number_of_node_in_broadcast_table].src.address_low = _RREQ->src.address_low;
    received_broadcast_table[number_of_node_in_broadcast_table].broadcast_id = _RREQ->broadcast_id;
  }
  
  // hop_count++ -> update routing table -> check if destination?
  // hop_count++
  _RREQ->hop_count = _RREQ->hop_count + 1;
  // Update Routing Table Process
  bool source_in_routing_table = false;    
  bool destination_in_routing_table = false;
  for (int i=0; i<number_of_node; i++) {  // check table if the source exists in the table. Yes: update; No: Add new
    if (rt_tbl[i].dest.address_high == _RREQ->src.address_high 
          && rt_tbl[i].dest.address_low == _RREQ->src.address_low) {
      source_in_routing_table = true;
      // update sequence number
      if (rt_tbl[i].dest.sequence_number <= _RREQ->src.sequence_number || _RREQ->src.sequence_number == 0) {  // check sequence number
        rt_tbl[i].dest.sequence_number = _RREQ->src.sequence_number;
        rt_tbl[i].hop_count = _RREQ->hop_count;
        rt_tbl[i].nxt_h.address_high = _RREQ->send_from.address_high;
        rt_tbl[i].nxt_h.address_low = _RREQ->send_from.address_low;
      }
    }
    // after receiving RREQ, if destination exists in routing_table -> forward to source
    if (rt_tbl[i].dest.address_high == _RREQ->dest.address_high
          && rt_tbl[i].dest.address_low == _RREQ->dest.address_low) {
      destination_in_routing_table = true;
      // prepare RREP_message, send to source
      struct RREP_message* _RREP;
      _RREP->type = RREP_MESSAGE_TYPE;
      _RREP->dest.address_high = _RREQ->src.address_high;
      _RREP->dest.address_low = _RREQ->src.address_low;
      _RREP->dest.sequence_number = _RREQ->src.sequence_number;
      _RREP->src.address_high = _RREQ->dest.address_high;
      _RREP->src.address_low = _RREQ->dest.address_low;
      _RREP->src.sequence_number = rt_tbl[i].dest.sequence_number;
      _RREP->hop_count = rt_tbl[i].hop_count;
      _RREP->reverse_path = 0x00;
      
      _RREP->send_from.address_high = src->address_high;
      _RREP->send_from.address_low = src->address_low;

      struct send_address* _send_to;
      _send_to->address_high = _RREQ->src.address_high;
      _send_to->address_low = _RREQ->src.address_low;
      
      unicast_RREP(_send_to, _RREP);
    }
    
  }
  if (source_in_routing_table == false) {
    rt_tbl[number_of_node].dest.address_high = _RREQ->dest.address_high;
    rt_tbl[number_of_node].dest.address_low = _RREQ->dest.address_low;
    rt_tbl[number_of_node].dest.sequence_number = _RREQ->dest.sequence_number;
    rt_tbl[number_of_node].nxt_h.address_high = _RREQ->send_from.address_high;
    rt_tbl[number_of_node].nxt_h.address_low = _RREQ->send_from.address_low;
    rt_tbl[number_of_node].hop_count = _RREQ->hop_count;

    number_of_node++;
  }

  // check if destination?
  if (_RREQ->dest.address_high == src->address_high && _RREQ->dest.address_low == src->address_low) {
    // Yes, it is the destination
    // UNICASTLY SEND RREP TO SOURCE
    // prepare RREP_message
    struct RREP_message* _RREP;
    _RREP->type = RREP_MESSAGE_TYPE;
    _RREP->dest.address_high = _RREQ->dest.address_high;
    _RREP->dest.address_low = _RREQ->dest.address_low;
    _RREP->dest.sequence_number = _RREQ->dest.sequence_number;
    _RREP->src.address_high = src->address_high;
    _RREP->src.address_low = src->address_low;
    src->sequence_number = src->sequence_number + 1;
    _RREP->src.sequence_number = src->sequence_number;
    _RREP->hop_count = 0;
    _RREP->reverse_path = 0x00;
    _RREP->send_from.address_high = src->address_high;
    _RREP->send_from.address_low = src->address_low;

    struct send_address* _send_to;
    _send_to->address_high = _RREQ->send_from.address_high;
    _send_to->address_low = _RREQ->send_from.address_low;
    
    // unicast RREP_message
    unicast_RREP(_send_to ,_RREP);
    
    return;
  }
  
  // No, it is not the destination, Rebroadcast Process
  _RREQ->send_from.address_high = src->address_high;
  _RREQ->send_from.address_low = src->address_low;
  uint8_t _message[sizeof(struct RREQ_message)];
  memcpy(_message, _RREQ, sizeof(struct RREQ_message));
  LoRa_broadcast(_message);
  
  return;
  
}
/*******************************************************************
 * receive_RREP_message()
********************************************************************/
void receive_RREP_message(struct RREP_message* _RREP) {
  // check if reversed_path (ACK)
  if (_RREP->reverse_path == REVERSE_PATH) {
    // TODO: Yes-> TURN OFF TIMER, no need to wait for ACK anymore. 
    Timer1.stop();
  }

  else {
    // unicast reversed_path (ACK) to send_from
    _RREP->reverse_path = REVERSE_PATH;
    struct send_address* _send_to;
    _send_to->address_high = _RREP->send_from.address_high;
    _send_to->address_low = _RREP->send_from.address_low;
    unicast_RREP(_send_to, _RREP, REVERSE_PATH);
  }

  // hop_count ++;
  _RREP->hop_count = _RREP->hop_count + 1;
  // update routing_table
  bool source_in_routing_table = false;
  int index_next_hop = 0;
  for (int i=0; i<number_of_node; i++) {  // check table if the destination exists in the table. Yes: update; No: Add new
    if (rt_tbl[i].dest.address_high == _RREP->src.address_high 
          && rt_tbl[i].dest.address_low == _RREP->src.address_low){
            source_in_routing_table = true;
            // update sequence number
            if (rt_tbl[i].dest.sequence_number <= _RREP->src.sequence_number || _RREP->src.sequence_number == 0) {  // check sequence number
              rt_tbl[i].dest.sequence_number = _RREP->src.sequence_number;
              rt_tbl[i].hop_count = _RREP->hop_count;
              rt_tbl[i].nxt_h.address_high = _RREP->send_from.address_high;
              rt_tbl[i].nxt_h.address_low = _RREP->send_from.address_low;
            }
          }
    if (rt_tbl[i].dest.address_high == _RREP->dest.address_high 
          && rt_tbl[i].dest.address_low == _RREP->dest.address_low) {
            index_next_hop = i;
          }
  }
  if (source_in_routing_table == false) {
    rt_tbl[number_of_node].dest.address_high = _RREP->dest.address_high;
    rt_tbl[number_of_node].dest.address_low = _RREP->dest.address_low;
    rt_tbl[number_of_node].dest.sequence_number = _RREP->dest.sequence_number;
    rt_tbl[number_of_node].nxt_h.address_high = _RREP->send_from.address_high;
    rt_tbl[number_of_node].nxt_h.address_low = _RREP->send_from.address_low;
    rt_tbl[number_of_node].hop_count = _RREP->hop_count;

    number_of_node++;
  }

  // check if destination?
  if (_RREP->dest.address_high == src->address_high && _RREP->dest.address_low == src->address_low) { // yes
    // do nothing
    return;
  }
  else {
    // No, it's not the destination
    // UNICASTLY SEND RREP TO SOURCE through next_hop
    // prepare RREP_message
    _RREP->send_from.address_high = src->address_high;
    _RREP->send_from.address_low = src->address_low;
    
    // search in routing_table for next_hop to destination: index_next_hop
    struct send_address* _send_to;
    _send_to->address_high = rt_tbl[index_next_hop].nxt_h.address_high;
    _send_to->address_low = rt_tbl[index_next_hop].nxt_h.address_low;
    
    // unicast RREP_message
    unicast_RREP(_send_to ,_RREP);
  }

  return;
}
/*******************************************************************
 * wait_for_ACK_timeout()
********************************************************************/
void wait_for_ACK_timeout() {
  // TODO: IF NOT RECEIVE ack, SHOULD DO SOMETHING!
  Serial.println("AODV_PROTOCOL: ACK Timeout!");
}
/*******************************************************************
 * receive_RERR_message()
********************************************************************/
void receive_RERR_message() {
  
}
/*******************************************************************
 * receive_data_message()
********************************************************************/
void receive_data_message() {
  
}

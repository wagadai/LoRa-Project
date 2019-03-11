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
  // unknown sequence_number of destination: NaN = 255;
  dest->sequence_number = 255;

  // pack RREQ_message:
  struct RREQ_message *RREQ;
  RREQ->dest = *dest;
  RREQ->src = *src;
  RREQ->broadcast_id = broadcast_id++;
  RREQ->hop_count = 0;
  
  // broadcast RREQ_message
  LoRa_broadcast(RREQ);

  // wait for RREP/ ACK
  listen_message();
}

void listen_message(){
  uint8_t* message;
  uint8_t message_t = LoRa_listen(message);
  switch (message_t) {
    case RREQ_MESSAGE_TYPE:
      // receive RREQ_message
      struct RREQ_message* _RREQ;
      // convert array to struct
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
    case ACK_MESSAGE_TYPE:
//      received_ack_message(message);
    default:
      // not receive any packet
      break;
  }
}

void receive_RREQ_message(struct RREQ_message* _RREQ) {
  // check if reversed path (RREQ reverse = ACK)
  if (_RREQ->reverse_path == REVERSE_PATH ) {  // Yes
    // TODO: TURN OFF TIMER, no need to wait for ACK anymore.
    
  }
  // check if duplicate RREQ? Based on Source address & broadcast_id
  bool destination_exist_in_broadcast_table = false;
  for (int i=0; i<number_of_node; i++) {
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
    received_broadcast_table[number_of_node].src.address_high = _RREQ->src.address_high;
    received_broadcast_table[number_of_node].src.address_low = _RREQ->src.address_low;
    received_broadcast_table[number_of_node].broadcast_id = _RREQ->broadcast_id;
  }
  
  // check if destination?
  if (_RREQ->dest.address_high == src->address_high && _RREQ->dest.address_low == src->address_low) {
    // Yes, it is the destination
    // TODO: UNICASTLY SEND RREP TO SOURCE (BASED ON HOP, LAST SENDER)
    
    return;
  }
  
  // No, it's not the destination; unicastly send ack to "sender" -> hop_count++ -> update routing table -> rebroadcast
  // TODO: UNICAST ACK TO SENDER
  
  // hop_count++
  _RREQ->hop_count = _RREQ->hop_count + 1;
  // Update Routing Table Process
  bool destination_exist_in_routing_table = false;    
  for (int i=0; i<number_of_node; i++) {  // check table if the destination exists in the table. Yes: update; No: Add new
    if (rt_tbl[i].dest.address_high == _RREQ->dest.address_high 
          && rt_tbl[i].dest.address_low == _RREQ->dest.address_low){
            destination_exist_in_routing_table = true;
            // update sequence number
            if (rt_tbl[i].dest.sequence_number <= _RREQ->src.sequence_number) {  // check sequence number
              rt_tbl[i].dest.sequence_number = _RREQ->src.sequence_number;
              rt_tbl[i].hop_count = _RREQ->hop_count;
              rt_tbl[i].nxt_h.address_high = _RREQ->sender.address_high;
              rt_tbl[i].nxt_h.address_low = _RREQ->sender.address_low;
            }
            break;
          }
  }
  if (destination_exist_in_routing_table == false) {
    rt_tbl[number_of_node].dest.address_high = _RREQ->dest.address_high;
    rt_tbl[number_of_node].dest.address_low = _RREQ->dest.address_low;
    rt_tbl[number_of_node].dest.sequence_number = _RREQ->dest.sequence_number;
    rt_tbl[number_of_node].nxt_h.address_high = _RREQ->sender.address_high;
    rt_tbl[number_of_node].nxt_h.address_low = _RREQ->sender.address_low;
    rt_tbl[number_of_node].hop_count = _RREQ->hop_count;

    number_of_node++;
  }

  // Rebroadcast Process
  _RREQ->sender.address_high = src->address_high;
  _RREQ->sender.address_low = src->address_low;
  LoRa_broadcast(_RREQ);
  
  return;
  
}

void receive_RREP_message() {
  
}

void receive_RERR_message() {
  
}

void receive_data_message() {
  
}

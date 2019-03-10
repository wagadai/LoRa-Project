/*******************************************************************
 * TEST_BENCH
********************************************************************/
//#define TEST_BENCH

//#define MESSAGE_TEST
//#define E32_TTL_100_TEST
/*******************************************************************
 * EEPROM
********************************************************************/
//#define EEPROM_WRITE
//#define NODE_ADDRESS_WRITE_TO_EEPROM  0x01

/*******************************************************************
 * LORA E32-TTL-100
********************************************************************/


/*******************************************************************
 * ADOV_PROTOCOL
********************************************************************/
struct RREQ_message {
  uint8_t broadcast_id;   // broadcast id
  uint8_t dest_addr;      //  destination address
  uint8_t dest_seq_no;    //  destination sequence number
  uint8_t source_addr;    //  source address;
  uint8_t source_seq_no;  // source sequence number
  uint8_t hop_count;      // hop count
};

struct RREP_message {
  uint8_t dest_addr;
  uint8_t dest_seq_no;
  uint8_t source_addr; 
  uint8_t source_seq_no; 
  uint8_t hop_count; 
};

struct RERR_message {
  uint8_t dest_addr_unreachable;
  uint8_t dest_seq_no_unreachable;
};

struct routing_table {
  uint8_t node_addr;
  uint8_t next_hop;
  uint8_t seq_no;
  uint8_t hop_count;
};

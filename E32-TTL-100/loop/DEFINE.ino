/*******************************************************************
 * TEST_BENCH
********************************************************************/
#define TEST_BENCH

#define MESSAGE_TEST
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
#define   RREQ_MESSAGE_TYPE 0x01
#define   RREP_MESSAGE_TYPE 0x02
#define   RERR_MESSAGE_TYPE 0x03
#define   DATA_MESSAGE_TYPE 0x04
#define   ACK_MESSAGE_TYPE  0x05

#define   REVERSE_PATH  0xFF

struct source {
  uint8_t address_high;
  uint8_t address_low;
  uint8_t sequence_number;
};

struct destination {
  uint8_t address_high;
  uint8_t address_low;
  uint8_t sequence_number;
};

struct sender_address {
  uint8_t address_high;
  uint8_t address_low;
};

struct next_hop {
  uint8_t address_high;
  uint8_t address_low;
};

struct RREQ_message {
  uint8_t type;
  struct source src;         //  source address & sequence number
  struct destination dest;    //  destination address & sequence number
  struct sender_address sender;
  uint8_t hop_count;      //  hop count
  uint8_t broadcast_id;
  uint8_t reverse_path;
  
  
};

struct RREP_message {
  uint8_t type;
  struct source src; 
  struct destination dest;
  uint8_t hop_count; 
};

struct RERR_message {
  uint8_t type;
  struct destination;
};

struct routing_table {
  struct destination dest;
  struct next_hop nxt_h;
  uint8_t hop_count;
};

struct received_broadcast_message {
  struct source src;
  uint8_t broadcast_id;
};

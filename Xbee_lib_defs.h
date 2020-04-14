/*
  setup.h - Library for setting up xbee network and 328 base pcb
  interrupt pin, baudrate, software serial pins
*/
#ifndef Xbee_lib_defs_h
#define Xbee_lib_defs_h
#include <Arduino.h>

// all addresses  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF // broadcast
// xbee1 address  0x00, 0x13, 0xA2, 0x00, 0x41, 0x25, 0xA4, 0x79 // sleep coordinator
// xbee2 address  0x00, 0x13, 0xA2, 0x00, 0x41, 0x25, 0xA4, 0x81
// xbee3 address  0x00, 0x13, 0xA2, 0x00, 0x41, 0x4E, 0x65, 0x93
// xbee4 address  0x00, 0x13, 0xA2, 0x00, 0x41, 0x4E, 0x65, 0x8D
// xbee5 address  0x00, 0x13, 0xA2, 0x00, 0x41, 0x4E, 0x65, 0x8E
// xbee6 address  0x00, 0x13, 0xA2, 0x00, 0x41, 0x25, 0xA4, 0x95

// address of destination
#define ADDR_B1  0x00
#define ADDR_B2  0x13
#define ADDR_B3  0xA2
#define ADDR_B4  0x00
#define ADDR_B5  0x41
#define ADDR_B6  0x25
#define ADDR_B7  0xA4
#define ADDR_B8  0x79

// TX frame structure
namespace TX
{
enum TX
{
  SOM           = 0,  // byte 0
  MSB_LEN       = 1,  // length MSB
  LSB_LEN       = 2,  // length LSB
  FRAME_TYPE    = 3,  // 0x10 is TX request
  FRAME_ID      = 4,  // 0x00 is n ack, 0x8B is received ack
  DEST_0        = 5,  // address MSB
  DEST_1        = 6,
  DEST_2        = 7,
  DEST_3        = 8,
  DEST_4        = 9,
  DEST_5        = 10,
  DEST_6        = 11,
  DEST_7        = 12, // address LSB
  RESV_1        = 13,
  RESV_2        = 14,
  BRDCST_RAD    = 15, // number of hops for broadcast transmission 0x00 is max
  TX_OPT        = 16, // transmit options (bit packed) (0xC1)
                      //  0=Disable Ack, 1=Disable RD, 2=Enable NACK, 3=Trace Route
                      //  4=Reserved,    5=Reserved,   6=Delivery 1,  7=Delivery 2
                      //  Bit 6&7: b'00=invalid, b'01=point/mulit, b'10=repeater, b'11=digimesh
  PAYLOAD_CNT   = 17, // first payload byte
  PAYLOAD_ID    = 18, // first payload byte
  PAYLOAD_0     = 19, // first payload byte                      // last byte is checksum
};
}

// RX frame structure
namespace RX
{
enum RX
{
  SOM           = 0,  // byte 0
  MSB_LEN       = 1,  // length MSB
  LSB_LEN       = 2,  // length LSB
  FRAME_TYPE    = 3,  // 0x90 RX packet
  SRC_0         = 4,  // address MSB
  SRC_1         = 5,
  SRC_2         = 6,
  SRC_3         = 7,
  SRC_4         = 8,
  SRC_5         = 9,
  SRC_6         = 10,
  SRC_7         = 11, // address LSB
  RESV_1        = 12,
  RESV_2        = 13,
  TX_OPT        = 14, // transmit options (bit packed) (0xC1)
                      // 0=Disable Ack, 1=Disable RD, 2=Enable NACK, 3=Trace Route
                      // 4=Reserved,    5=Reserved,   6=Delivery 1,  7=Delivery 2
                      // Bit 6&7: b'00=invalid, b'01=point/mulit, b'10=repeater, b'11=digimesh
  PAYLOAD_CNT   = 15, // payload tx count
  PAYLOAD_ID    = 16, // payload id type
  PAYLOAD_0     = 17, // first byte of data
  PAYLOAD_1     = 18, // second byte of data
  PAYLOAD_2     = 19  // third byte of data
};
}

namespace PARSE
{
enum PARSE
{
  SOM = 0,
  LENGTH = 1,
  FRAME_TYPE = 2,
  SOURCE = 3,
  UNUSED = 4,
  PAYLOAD_CNT = 5,
  PAYLOAD_TYPE = 6,
  PAYLOAD = 7,
  CHECKSUM = 8
};
}

enum ID
{
  XBEE_X = 0,
  XBEE_1 = 1,
  XBEE_2 = 2,
  XBEE_3 = 3,
  XBEE_4 = 4,
  XBEE_5 = 5,
  XBEE_6 = 6
};

enum RX_STATUS
{
  SUCCESS = 0,
  FAILED = 1
};

struct Msg_data
{
  bool valid = false;
  uint8_t frame_type = 0;
  uint8_t address = 0;
  uint8_t length = 0;
  uint8_t payload_cnt = 0;
  uint8_t payload_id = 0;
  uint8_t payload[3] = {};
};

#endif

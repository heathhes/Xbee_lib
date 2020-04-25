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
#define ADDR_B0  0x00
#define ADDR_B1  0x13
#define ADDR_B2  0xA2
#define ADDR_B3  0x00
#define ADDR_B4  0x41
#define ADDR_B5  0x25
#define ADDR_B6  0xA4
#define ADDR_B7  0x79

// TX frame structure
namespace TX
{
enum TX
{
  SOM           = 0,  // byte 0
  MSB_LEN       = 1,  // length MSB
  LSB_LEN       = 2,  // length LSB
  FRAME_TYPE    = 3,  // 0x10 is TX request
  FRAME_ID      = 4,  // frame id for ack
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
  FRAME_TYPE    = 3,  // 0x90 RX packet, 0x8B is received ack
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
  RX_OPT        = 14, // Receive options (bit packed) (0xC1)
                      // 0=Packet acknowledged, 1=Packet was broadcast packet
                      // 2,3,4,5=Unused   6=Delivery 1,  7=Delivery 2
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

namespace CMD_ID
{
enum CMD_ID
{
  NO_ACK  = 'n',    // no acknowledgment expected
  ACK     = 'a',    // basic response
  IO_OUT  = 'o',    // digital, pmw, analog outputs
  IO_IN   = 'i',    // HEX: 69 digital, analog , sensor inputs
  FNCTN_0 = 'f',    // custom function to call
  FNCTN_1 = 'g'     // custom function to call
};
}

namespace STAT_ID
{
enum STAT_ID
{
  NO_ACK  = 'n',    // no acknowledgment expected
  ACK     = 'a',    // basic response
  IO_OUT  = 'o',    // digital, pmw, analog outputs
  IO_IN   = 'i',    // digital, analog , sensor inputs
  FNCTN_0 = 'f',    // custom function to call
  FNCTN_1 = 'g'     // custom function to call
};
}

struct Msg_data
{
  bool valid = false;
  uint8_t frame_type = 0;
  uint8_t address = 0;
  uint8_t payload_id = CMD_ID::ACK;
  uint8_t payload_cnt = 0;
  uint8_t payload_len = 0;
  uint8_t payload[3] = {};
};





//////////////////////////////////////////////////////////////////////

// Receive frame type: 0x90
/*
7E, 0, 11, 90, 0, 13, A2, 0, 41, 4E, 65, 93, FF, FE, C1, D7, A1, 41, 41, 0, 7B,
uint8_t rx_array[] = {0x7E,             // SOM
                      0x00,             // length MSB
                      0x11,             // length LSB
                      0x90,             // Frame Type (0x10 Tx request)
                      ADDR_B0, ADDR_B1, ADDR_B2,
                      ADDR_B3, ADDR_B4, ADDR_B5,
                      ADDR_B7, ADDR_B7,
                      0xFF,             // Reserved 1
                      0xFE,             // Reserved 2
                      0x00,             // Receive options bit backed, (0xC0, 0xC1)
                      0x01,             // Payload, count
                      0xA1,             // Payload, ID
                      0x32, 0x33, 0x34, // Payload
                      0xD6};            // Checksum
*/


//////////////////////////////////////////////////////////////////////
// Transmit request: 0x10
/*
7E, 0, 13, 10, 0, 0, 13, A2, 0, 41, 25, A4, 81, FF, FE, 0, 0, P0, P1, 32, 33, 34, D1
uint8_t tx_array[] = {0x7E,             // SOM
                      0x00,             // length MSB
                      0x13,             // length LSB
                      0x10,             // Frame Type (0x10 Tx request)
                      0x00,             // Frame ID (used for ACK)
                      ADDR_B0, ADDR_B1, ADDR_B2,
                      ADDR_B3, ADDR_B4, ADDR_B5,
                      ADDR_B7, ADDR_B7,
                      0xFF,             // Reserved 1
                      0xFE,             // Reserved 2
                      0x00,             // Broadcast radius
                      0x00,             // Transmit options bit backed, (0x00, 0xC0, 0xC1)
                      0x01,             // Payload, count
                      0xA1,             // Payload, ID
                      0x32, 0x33, 0x34, // Payload
                      0xD6};            // Checksum
*/
#endif

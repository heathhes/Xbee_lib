#ifndef Xbee_lib_h
#define Xbee_lib_h

#include "Arduino.h"
#include "Xbee_lib_defs.h"
#include <SoftwareSerial.h>

class Xbee_lib
{
public:
  Xbee_lib(SoftwareSerial* ss);
  uint8_t Get_checksum(uint8_t frame[], uint8_t len);
  void Set_dest_addr(uint8_t array[], uint8_t dest);
  uint8_t Get_address(uint8_t address_byte);
  void Clear_array(uint8_t array[], uint8_t len);
  uint8_t Transmit_data(uint8_t array[],
                        uint8_t len,
                        ID dest = ID::XBEE_1);

  void Process_byte(uint8_t rx_byte);
  void Clear_msg(struct Msg_data& msg);
  void Set_callback(void (*msg_callback)(struct Msg_data));
  void Begin(uint32_t baud);
  void Print_array(uint8_t array[], uint8_t len, bool hex);
  void Print_msg(struct Msg_data msg, uint8_t len_payload);

protected:

private:
  SoftwareSerial* _ss;
  void (*_msg_callback)(struct Msg_data);
  struct Msg_data m_msg_data;
  uint8_t m_parser_state = PARSE::SOM;
  uint8_t m_parser_cnt = 0;
  uint8_t m_msg_array[21] = {};

  void reset_parser();
};
#endif

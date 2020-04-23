#ifndef Xbee_lib_h
#define Xbee_lib_h

#include <Arduino.h>
#include <Xbee_lib_defs.h>
#include <Print_lib.h>
#include <SoftwareSerial.h>

class Xbee_lib
{
public:
  Xbee_lib(Print_lib* printer);
  uint8_t Get_checksum(const uint8_t frame[], const uint8_t len);
  void Set_dest_addr(uint8_t array[], const uint8_t dest);
  uint8_t Get_address(const uint8_t address_byte);
  void Clear_array(uint8_t array[], const uint8_t len);
  uint8_t Transmit_data(uint8_t array[],
                        const uint8_t len,
                        const ID dest = ID::XBEE_1);

  bool Build_frame(const struct Msg_data tx_msg, uint8_t* tx_array);

  void Process_byte(const uint8_t rx_byte);
  void Clear_msg(struct Msg_data& msg);
  void Set_callback(void (*msg_callback)(const struct Msg_data));
  void Begin(const uint32_t baud);
  void Print_array(const uint8_t array[], const uint8_t len, bool hex = true);
  void Print_msg(const struct Msg_data msg, bool hex = true);

protected:

private:
  Print_lib* _m_print;
  void (*_msg_callback)(const struct Msg_data);
  struct Msg_data m_msg_data;
  uint8_t m_parser_state = PARSE::SOM;
  uint8_t m_parser_cnt = 0;
  uint8_t m_msg_array[21] = {};

  void reset_parser();
};
#endif

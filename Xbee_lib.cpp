#include "Xbee_lib.h"

Xbee_lib::Xbee_lib(SoftwareSerial *ss)
{
  _ss = ss;
};

////////////////////////////////////////////////////////////

void Xbee_lib::Begin(const uint32_t baud)
{
  // call m_xbee.Begin(19200) from setup() in .ino
  _ss->begin(baud);
}

////////////////////////////////////////////////////////////

uint8_t Xbee_lib::Get_checksum(const uint8_t frame[], const uint8_t length)
{
  long sum = 0;
  for(int i = 3; i < (length - 1); i++){
    sum += frame[i];
  }
  uint8_t check_sum = 0xFF - (sum & 0xFF);
  return check_sum;
};

////////////////////////////////////////////////////////////

void Xbee_lib::Set_dest_addr(uint8_t array[], const uint8_t dest)
{
  switch(dest)
  {
    case 1:
      array[10] = 0x25;
      array[11] = 0xA4;
      array[12] = 0x79;
      break;
    case 2:
      array[10] = 0x25;
      array[11] = 0xA4;
      array[12] = 0x81;
      break;
    case 3:
      array[10] = 0x4E;
      array[11] = 0x65;
      array[12] = 0x93;
      break;
    case 4:
      array[10] = 0x4E;
      array[11] = 0x65;
      array[12] = 0x8D;
      break;
    case 5:
      array[10] = 0x4E;
      array[11] = 0x65;
      array[12] = 0x8E;
      break;
    case 6:
      array[10] = 0x25;
      array[11] = 0xA4;
      array[12] = 0x95;
      break;

    default:
      break;
  }
};

////////////////////////////////////////////////////////////

uint8_t Xbee_lib::Get_address(const uint8_t address_byte)
{
  int xbee = 0;
  switch(address_byte)
  {
    case 0x79:
      xbee = 1;
      break;
    case 0x81:
      xbee = 2;
      break;
    case 0x93:
      xbee = 3;
      break;
    case 0x8D:
      xbee = 4;
      break;
    case 0x8E:
      xbee = 5;
      break;
    case 0x95:
      xbee = 6;
      break;
    default:
      xbee = 0;
      break;
  }
  return xbee;
};

////////////////////////////////////////////////////////////

void Xbee_lib::Clear_array(uint8_t array[], const uint8_t len)
{
  for(int i = 0; i < len; i++)
  {
    array[i] = 0;
  }
};

////////////////////////////////////////////////////////////

uint8_t Xbee_lib::Transmit_data(uint8_t array[],
                                const uint8_t len,
                                const ID dest)
{
  Set_dest_addr(array, dest);
  uint8_t cs = Get_checksum(array, len);
  array[len - 1] = cs;
  delay(10);
  Serial.write(array, len);
  delay(10);
  return 1;
};

//////////////////////////////////////////////////////////////////////

uint8_t Xbee_lib::Transmit_data(const Msg_data tx_msg)
{
  const uint8_t length = sizeof(tx_msg.payload) + 20; // omit SOM, MSB, LSB, CHECKSUM
  uint8_t tx_array[length];
  tx_array[0] = 0x7E;
  tx_array[1] = 0x00;
  tx_array[2] = length - 4;
  tx_array[3] = tx_msg.frame_type;
  tx_array[4] = 0x00;                 // if not zero, responds with ACK
  tx_array[5] = ADDR_B0;
  tx_array[6] = ADDR_B1;
  tx_array[7] = ADDR_B2;
  tx_array[8] = ADDR_B3;
  tx_array[9] = ADDR_B4;
  Set_dest_addr(tx_array, tx_msg.address);
  tx_array[13] = 0xFF;                // resv 1
  tx_array[14] = 0xFE;                // resv 2
  tx_array[15] = 0x00;                // broadcast radius
  tx_array[16] = 0x00;                // C0 = mesh no-ack , C1 = mesh ack
  tx_array[17] = tx_msg.payload_cnt;  // payload byte 1
  tx_array[18] = tx_msg.payload_id;
  for(int i = 0; i < sizeof(tx_msg.payload); i++)
  {
    tx_array[i + 19] = tx_msg.payload[i];
  }
  tx_array[length - 1] = Get_checksum(tx_array, length);

  delay(10);
  Serial.write(tx_array, length);
  delay(10);
  return 1;
}

//////////////////////////////////////////////////////////////////////

void Xbee_lib::Process_byte(const uint8_t rx_byte)
{
  m_parser_cnt++;
  switch(m_parser_state)
  {
    case PARSE::SOM :
      if(rx_byte == 0x7E)
      {
        reset_parser();
        m_msg_array[m_parser_cnt] = rx_byte;
        m_parser_state = PARSE::LENGTH;
      }
      break;

    case PARSE::LENGTH :
      m_msg_array[m_parser_cnt] = rx_byte;
      if(m_parser_cnt == RX::LSB_LEN)
      {
        m_msg_data.length = rx_byte;
        m_parser_state = PARSE::FRAME_TYPE;
      }
      break;

    case PARSE::FRAME_TYPE :
      m_msg_array[m_parser_cnt] = rx_byte;
      m_msg_data.frame_type = rx_byte;
      m_parser_state = PARSE::SOURCE;
      break;

    case PARSE::SOURCE :
      m_msg_array[m_parser_cnt] = rx_byte;
      if(m_parser_cnt == RX::SRC_7)
      {
        m_msg_data.address = Get_address(rx_byte);
        m_parser_state = PARSE::UNUSED;
      }
      break;

    case PARSE::UNUSED :
      m_msg_array[m_parser_cnt] = rx_byte;
      if(m_parser_cnt == RX::RESV_1 + 2)  // RESV 1, RESV2, RX_OPT
      {
        m_parser_state = PARSE::PAYLOAD_CNT;
      }
      break;

    case PARSE::PAYLOAD_CNT :
      m_msg_array[m_parser_cnt] = rx_byte;
      m_msg_data.payload_cnt = rx_byte;
      m_parser_state = PARSE::PAYLOAD_TYPE;
      break;

    case PARSE::PAYLOAD_TYPE :
      m_msg_array[m_parser_cnt] = rx_byte;
      m_msg_data.payload_id = rx_byte;
      m_parser_state = PARSE::PAYLOAD;
      break;

    case PARSE::PAYLOAD :
    {
      static uint8_t j = 0;
      m_msg_array[m_parser_cnt] = rx_byte;
      m_msg_data.payload[j] = rx_byte;
      j++;
      if(m_parser_cnt == RX::PAYLOAD_2)
      {
        m_parser_state = PARSE::CHECKSUM;
        j = 0;
      }
      break;
    }

    case PARSE::CHECKSUM :
    {
      m_msg_array[m_parser_cnt] = rx_byte;

      uint8_t cs = Get_checksum(m_msg_array,
                                sizeof(m_msg_array));
      if(cs == m_msg_array[sizeof(m_msg_array) - 1])
      {
        m_msg_data.valid = true;

        _msg_callback(m_msg_data);
      }
      else
      {
        _ss->println("Invalid checksum");
      }
      reset_parser();
      break;
    }

    default:
    {
      reset_parser();
    }
  }
}

//////////////////////////////////////////////////////////////////////

void Xbee_lib::reset_parser()
{
  Clear_array(m_msg_array, sizeof(m_msg_array));
  m_parser_cnt = 0;
  m_parser_state = PARSE::SOM;
  Clear_msg(m_msg_data);
}

//////////////////////////////////////////////////////////////////////

void Xbee_lib::Clear_msg(struct Msg_data& msg)
{
  msg.valid = false;
  msg.frame_type = 0;
  msg.address = 0;
  msg.length = 0;
  msg.payload_cnt = 0;
  msg.payload_id = CMD_ID::ACK;
  msg.payload_len = 0;
  for(int i = 0; i < sizeof(msg.payload); i++)
  {
    msg.payload[i] = 0;
  }
};

//////////////////////////////////////////////////////////////////////

void Xbee_lib::Set_callback(void (*msg_callback)(const struct Msg_data))
{
  _msg_callback = msg_callback;
}

//////////////////////////////////////////////////////////////////////

void Xbee_lib::Print_array(const uint8_t array[], const uint8_t len, bool hex)
{
  if(hex)
  {
    for(int i = 0; i < len; i++)
    {
      _ss->print(array[i],HEX);
      _ss->print(", ");
    }
    _ss->println();
  }
  else
  {
    for(int i = 0; i < len; i++)
    {
      _ss->print(array[i]);
      _ss->print(", ");
    }
    _ss->println();
  }
}

//////////////////////////////////////////////////////////////////////

void Xbee_lib::Print_msg(const Msg_data msg, bool hex)
{
  if(hex)
  {
    _ss->print("Length: ");
    _ss->println(msg.length, HEX);
    _ss->print("Valid: ");
    _ss->println(msg.valid, HEX);
    _ss->print("Frame type: ");
    _ss->println(msg.frame_type, HEX);
    _ss->print("Address: ");
    _ss->println(msg.address, HEX);
    _ss->print("Count: ");
    _ss->println(msg.payload_cnt, HEX);
    _ss->print("Payload id: ");
    _ss->println(msg.payload_id, HEX);
    _ss->print("Payload length: ");
    _ss->println(msg.payload_len, HEX);
    _ss->print("Payload: ");
    Print_array(msg.payload, sizeof(msg.payload), false);
  }
  else
  {
    _ss->print("Length: ");
    _ss->println(msg.length);
    _ss->print("Valid: ");
    _ss->println(msg.valid);
    _ss->print("Frame type: ");
    _ss->println(msg.frame_type);
    _ss->print("Address: ");
    _ss->println(msg.address);
    _ss->print("Count: ");
    _ss->println(msg.payload_cnt);
    _ss->print("Payload id: ");
    _ss->println(msg.payload_id);
    _ss->print("Payload length: ");
    _ss->println(msg.payload_len);
    _ss->print("Payload: ");
    Print_array(msg.payload, sizeof(msg.payload));
  }
  _ss->println();
}

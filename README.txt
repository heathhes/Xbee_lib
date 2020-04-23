// Receive frame type: 0x90
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


//////////////////////////////////////////////////////////////////////
// Transmit request: 0x10

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


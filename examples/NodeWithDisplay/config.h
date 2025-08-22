#ifndef CONFIG_H
#define CONFIG_H

// #define DEBUG // Uncomment to print debug messages
#define PRINT // Uncomment to print messages to serial monitor

// Interrupt pin on MCP2515
// driven low when the a msg is received or the transmit buffer is ready
#define CAN_INT   2
#define CAN_CS    9

// Object dictionary indices
#define DISPLAYED_CHARGE               0x2520
#define STATUS                         0x2000
#define CURRENT                        0x2005
#define VOLTAGE                        0x2005
#define TEMPERATURE                    0x6010
#define MIN_VOLTAGE                    0x2006

#define DEVICE_TYPE_INDEX              0x1000 // Communication Parameters
#define ERROR_REGISTER_INDEX           0x1001
#define SYNC_COBID_INDEX               0x1005
#define COMM_CYCLE_PERIOD_INDEX        0x1006
#define GUARD_TIME_INDEX               0x100C
#define LIFE_TIME_FACTOR_INDEX         0x100D
#define EMERGENCY_COBID_INDEX          0x1014
#define CONSUMER_HEARTBEAT_INDEX       0x1016
#define PRODUCER_HEARTBEAT_INDEX       0x1017
#define IDENTITY_OBJECT_INDEX          0x1018
#define ERROR_BEHAVIOR_INDEX           0x1029

#define TX_PDO1_MAPPING_INDEX          0x1A00 // PDO Mappings
#define TX_PDO2_MAPPING_INDEX          0x1A01
#define TX_PDO3_MAPPING_INDEX          0x1A02
#define TX_PDO4_MAPPING_INDEX          0x1A03
#define RX_PDO1_MAPPING_INDEX          0x1600
#define RX_PDO2_MAPPING_INDEX          0x1601
#define RX_PDO3_MAPPING_INDEX          0x1602
#define RX_PDO4_MAPPING_INDEX          0x1603

// Standard EMCY Error Register Bits
#define EMCY_GENERIC_ERR_BIT         0x01  // Bit 0
#define EMCY_VOLTAGE_ERR_BIT         0x02  // Bit 1
#define EMCY_CURRENT_ERR_BIT         0x04  // Bit 2
#define EMCY_TEMP_ERR_BIT            0x08  // Bit 3
#define EMCY_COMM_ERR_BIT            0x20  // Bit 5
#define EMCY_DEVICE_ERR_BIT          0x80  // Bit 7
// Manufacturer-specific error field templates (5 bytes)
#define MFG_ERROR_GENERIC            {0x00, 0x00, 0x00, 0x00, 0x01}
#define MFG_ERROR_OVERCURRENT        {0x00, 0x00, 0x00, 0x02, 0x02}
#define MFG_ERROR_VOLTAGE            {0x00, 0x00, 0x00, 0x03, 0x03}
#define MFG_ERROR_TEMPERATURE        {0x00, 0x00, 0x00, 0x04, 0x04}
#define MFG_ERROR_COMM               {0x00, 0x00, 0x00, 0x05, 0x05}
#define MFG_ERROR_DEVICE             {0x00, 0x00, 0x00, 0x06, 0x06}

// Message IDs
#define PDO1_RX_ID   0x201 
#define PDO2_RX_ID   0xFFF 
#define PDO3_RX_ID   0xFFF
#define PDO4_RX_ID   0xFFF 
#define PDO1_TX_ID   0x181 
#define PDO2_TX_ID   0x281
#define PDO3_TX_ID   0xFFF 
#define PDO4_TX_ID   0xFFF
#define NMT_ID       0x701 
#define SDO_RX_ID    0x601
#define SDO_TX_ID    0x501
#define EMCY_ID      0x081
// Read command bytes
#define READ_REQ_ANY_CMD     0x40
#define READ_REQ_1BYTE_CMD   0x4F // or response
#define READ_REQ_2BYTE_CMD   0x4B // or response
#define READ_REQ_4BYTE_CMD   0x43 // or response
// Write command bytes
#define WRITE_REQ_1BYTE_CMD      0x2F
#define WRITE_REQ_2BYTE_CMD      0x2B
#define WRITE_REQ_4BYTE_CMD      0x23
#define WRITE_RESP_SUCCESS_CMD   0x60
#define WRITE_RESP_FAIL_CMD      0x80
// Timer values
#define STARTUP_TIMEOUT        5000 // ms
#define HEARTBEAT_CYCLE_TIME   100  // ms
#define PDO1_TX_CYCLE_TIME     100  // ms
#define PDO2_TX_CYCLE_TIME     100  // ms
#define PDO3_TX_CYCLE_TIME     100  // ms
#define PDO4_TX_CYCLE_TIME     100  // ms
#define PDO1_RX_CYCLE_TIME     140  // ms
#define PDO2_RX_CYCLE_TIME     140  // ms
#define PDO3_RX_CYCLE_TIME     140  // ms
#define PDO4_RX_CYCLE_TIME     140  // ms

#endif 
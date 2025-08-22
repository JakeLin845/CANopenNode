#include "object.hpp"

static Object dictionary[] = {
//  |             INDEX             | SUB |          DATA          |       TYPE       |      PERMISSIONS      |
    {DISPLAYED_CHARGE,               0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ},
    {STATUS,                         0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ},
    {CURRENT,                        0x01, {0x01, 0x1A, 0x00, 0x00}, DataType::INT16,  ObjectPermissions::READ},
    {VOLTAGE,                        0x01, {0xC0, 0x12, 0x00, 0x00}, DataType::UINT16, ObjectPermissions::READ},
    {TEMPERATURE,                    0x01, {0x2C, 0x01, 0x00, 0x00}, DataType::UINT16, ObjectPermissions::READ},
    {MIN_VOLTAGE,                    0x03, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    
    {DEVICE_TYPE_INDEX,              0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ},
    {ERROR_REGISTER_INDEX,           0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ},
    {SYNC_COBID_INDEX,               0x00, {0x80, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {COMM_CYCLE_PERIOD_INDEX,        0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {GUARD_TIME_INDEX,               0x00, {0x64, 0x00, 0x00, 0x00}, DataType::UINT16, ObjectPermissions::READ_WRITE},
    {LIFE_TIME_FACTOR_INDEX,         0x00, {0x03, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {EMERGENCY_COBID_INDEX,          0x00, {0x81, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {CONSUMER_HEARTBEAT_INDEX,       0x00, {0x01, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {PRODUCER_HEARTBEAT_INDEX,       0x00, {0x88, 0x13, 0x00, 0x00}, DataType::UINT16, ObjectPermissions::READ_WRITE},

    {IDENTITY_OBJECT_INDEX,          0x00, {0x04, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ},
    {IDENTITY_OBJECT_INDEX,          0x01, {0x34, 0x12, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ},  // Vendor ID
    {IDENTITY_OBJECT_INDEX,          0x02, {0x78, 0x56, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ},  // Product Code
    {IDENTITY_OBJECT_INDEX,          0x03, {0x01, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ},  // Revision
    {IDENTITY_OBJECT_INDEX,          0x04, {0xAB, 0xCD, 0xEF, 0x00}, DataType::UINT32, ObjectPermissions::READ},  // Serial Number

    {ERROR_BEHAVIOR_INDEX,           0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},

    {TX_PDO1_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {TX_PDO1_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {TX_PDO1_MAPPING_INDEX,          0x02, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {TX_PDO1_MAPPING_INDEX,          0x03, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {TX_PDO1_MAPPING_INDEX,          0x04, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},

    {TX_PDO2_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {TX_PDO2_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {TX_PDO2_MAPPING_INDEX,          0x02, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},

    {TX_PDO3_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {TX_PDO3_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},

    {TX_PDO4_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {TX_PDO4_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},

    {RX_PDO1_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {RX_PDO1_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},
    {RX_PDO1_MAPPING_INDEX,          0x02, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},

    {RX_PDO2_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {RX_PDO2_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},

    {RX_PDO3_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {RX_PDO3_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE},

    {RX_PDO4_MAPPING_INDEX,          0x00, {0x00, 0x00, 0x00, 0x00}, DataType::UINT8,  ObjectPermissions::READ_WRITE},
    {RX_PDO4_MAPPING_INDEX,          0x01, {0x00, 0x00, 0x00, 0x00}, DataType::UINT32, ObjectPermissions::READ_WRITE}
  };
constexpr uint8_t NUM_OBJS = sizeof(dictionary)/sizeof(Object);
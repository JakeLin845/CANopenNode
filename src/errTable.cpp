#include "emcy.hpp"

static constexpr Error errorTable[] = {
//  |   ERROR CODE   |     ERROR REGISTER     |    MANUFACTURER DATA    |
    {0x1000,         EMCY_GENERIC_ERR_BIT,    MFG_ERROR_GENERIC        },
    {0x2310,         EMCY_CURRENT_ERR_BIT,    MFG_ERROR_OVERCURRENT    },
    {0x3210,         EMCY_VOLTAGE_ERR_BIT,    MFG_ERROR_VOLTAGE        },
    {0x4210,         EMCY_TEMP_ERR_BIT,       MFG_ERROR_TEMPERATURE    },
    {0x5110,         EMCY_COMM_ERR_BIT,       MFG_ERROR_COMM           },
    {0x8110,         EMCY_DEVICE_ERR_BIT,     MFG_ERROR_DEVICE         }
};

constexpr uint8_t NUM_EMCYS = sizeof(errorTable) / sizeof(Error);

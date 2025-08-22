#include "sdo.hpp"
#include "receiver.hpp"

// Smartly set the SDO object data fields based on the received message
bool SDO::setResponse(const Receiver &recv) {
    if (recv.id == SDO_RX_ID && recv.dlc == dlc)
    {
        uint16_t recvIndex = recv.data[INDEX_HIGH] << 8 | recv.data[INDEX_LOW];
        uint8_t recvSubIndex = recv.data[SUB_INDEX];

        // set SDO object to dictionary object
        uint32_t index = Object::findIndex(recvIndex, recvSubIndex);
        if (index == -1) // findIndex returns -1 if indices are not found in the dictionary
            return false;
        setObject(index);

        uint8_t recvCmd = recv.data[COMMAND];

        // set SDO object data fields to dictionary object
        data[INDEX_LOW] = obj->index & 0xFF;
        data[INDEX_HIGH] = obj->index >> 8;
        data[SUB_INDEX] = obj->subIndex;
        // set SDO data fields and determine if SDO is WRITE or READ
        switch (recvCmd)
        {
        case WRITE_REQ_1BYTE_CMD:
        case WRITE_REQ_2BYTE_CMD:
        case WRITE_REQ_4BYTE_CMD:
            memset(&data[4], 0, 4); // clear data bytes
            sendType = SendType::WRITE;
            break;
        case READ_REQ_ANY_CMD:
        case READ_REQ_1BYTE_CMD:
        case READ_REQ_2BYTE_CMD:
        case READ_REQ_4BYTE_CMD:
            data[DATA_4] = obj->data[0];
            data[DATA_5] = obj->data[1];
            data[DATA_6] = obj->data[2];
            data[DATA_7] = obj->data[3];
            sendType = SendType::READ;
            break;
        }
        data[COMMAND] = getCmd();
        return true;
    }
    else
    {
        #ifdef DEBUG
        Serial.println("SDO Update Failed: Recv ID or DLC != SDO ID or DLC");
        #endif
        return false;
    }
}

// bool SDO::setSend(const Object &obj)
// {
//   data[INDEX_LOW] = obj.index & 0xFF;
//   data[INDEX_HIGH] = obj.index >> 8;
//   data[SUB_INDEX] = obj.subIndex;

//   if (type == WRITE)
//   {
//     if (obj.access == ObjectPermissions::READ)
//     { // cannot write to read-only object
//       return false;
//     }
//     data[COMMAND] = //cmdbyte;
//     data[DATA_4] = obj.data[0];
//     data[DATA_5] = obj.data[1];
//     data[DATA_6] = obj.data[2];
//     data[DATA_7] = obj.data[3];
//   }
//   if (type == READ)
//   {
//     data[COMMAND] = //cmdbyte;
//     memset(&data[4], 0, 4);
//   }
//   return true;
// }
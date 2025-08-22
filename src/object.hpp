#ifndef OBJDICT_H
#define OBJDICT_H

#include <stdint.h>
#include "config.h"

// Data type and size definitions
enum class DataType : uint8_t {
  UINT8 = 1,
  UINT16 = 2,
  UINT32 = 4,
  INT8 = 1,
  INT16 = 2,
  INT32 = 4
};

enum class ObjectPermissions : uint8_t {
  READ,
  WRITE,
  READ_WRITE
}; // Object permissions are from the perspective of Master to Slave

struct Object {
  const uint16_t index;           // object dictionary index
  const uint8_t subIndex;         // object dictionary sub-index
  uint8_t data[4];                // data buffer
  const DataType type;            // data type
  const ObjectPermissions access; // read/write permissions
 
  uint8_t getTypeSize() const { return static_cast<uint8_t>(type); }; // Return uint/int size of object
  static int32_t findIndex(uint16_t index, uint8_t subIndex); // Return object index position in dictionary array

  void printInfo(); // Print object information
};

// External dictionary (definition in .cpp)
extern Object dictionary[];
extern const uint8_t NUM_OBJS;

#endif
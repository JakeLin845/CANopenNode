#include "pdo.hpp"

// Map object to PDO object list at specified byte position
bool PDO::set(Object &obj, uint8_t position) {
  uint8_t size = obj.getTypeSize(); // object data byte size
  if (position > PDO_LEN) return false; // position is past array bounds
  if (position + size > PDO_LEN) return false; // PDO data size extends past array bounds

  objList[position] = &obj; // set object list index value to object pointer
  for (int i = position; i < (position + size); i++)
  { // set object data to PDO message data field
    data[i] = obj.data[i-position];
    // clear space in the object list for the new object
    if (i != position) objList[i] = nullptr;
  }

  return true;
}

// Map object at index and subindex to PDO object list at specified byte position
bool PDO::set(uint16_t index, uint8_t subIndex, uint8_t position) {
  int32_t objIndex = Object::findIndex(index, subIndex);
  if (objIndex < 0) return false; // object not found in dictionary

  uint8_t size = dictionary[objIndex].getTypeSize(); // object data byte size
  if (position > PDO_LEN) return false; // position is past array bounds
  if (position + size > PDO_LEN) return false; // PDO data size extends past array bounds

  objList[position] = &(dictionary[objIndex]); // set object list index value to object pointer
  for (int i = position; i < (position + size); i++)
  { // set object data to PDO message data field
    data[i] = dictionary[objIndex].data[i-position];
    // clear space in the object list for the new object
    if (i != position) objList[i] = nullptr;
  }

  return true;
}

// Update PDO data buffer to match object dictionary values
void PDO::updateData() {
  for (int position = 0; position < 8; position++)
  {
    if (objList[position] != nullptr) // if object is the start of a PDO
    {
      uint8_t size = objList[position]->getTypeSize(); // get object data byte size
      for (int i = 0; i < size; i++)
      { // update PDO data field with object data
        data[position + i] = objList[position]->data[i];
      }
    }
  }
}
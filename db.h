#include <stddef.h>

#ifndef DB_H
#define DB_H

typedef enum {
  TIMEBOMB,
  CD_CHECK
} PatchType;
extern const char *const patchTypes[];

typedef enum {
  TIMEBOMB_BYTE,
  CD_OLD_CHECK_BYTE,
  CD_NEW_CHECK_BYTE
} ByteType;

typedef struct {
  unsigned char oldByte;
  unsigned char newByte;
} ByteStruct;
extern const ByteStruct byteTypes[];

typedef struct {
  const char *name;
  int buildNumber;
  size_t filesize;
  int checksum;
  PatchType patchTypeIndex;
  size_t offset;
  ByteType byteTypeIndex;
} Database;
extern const Database patches[];
extern const int patchesCount;

#endif

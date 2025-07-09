#ifndef DB_H
#define DB_H

typedef enum {
  TIMEBOMB,
  CD_CHECK
} PatchType;
extern const char *patchTypes[];

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
  int filesize;
  int checksum;
  PatchType patchTypeIndex;
  int offset;
  ByteType byteTypeIndex;
} Database;
extern const Database patches[];
extern const int patchesCount;

#endif

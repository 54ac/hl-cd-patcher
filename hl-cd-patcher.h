#ifndef HL_CD_PATCHER_H
#define HL_CD_PATCHER_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  ERR_FILE_EXISTS,
  ERR_FILE_NOT_FOUND,
  ERR_FILE_READ,
  ERR_FILE_SIZE,
  ERR_UNKNOWN_VER,
  ERR_FILE_RENAME,
  ERR_FILE_DELETE,
  ERR_FILE_WRITE
} ErrorType;

static const char *const errorMessages[] = {
    "Backup file already exists",
    "File not found",
    "File read error",
    "File too large",
    "Unknown version or already patched",
    "Couldn't rename file",
    "Couldn't delete dat file",
    "Patched file write error"};

static void exitPrompt() {
  printf("Press any key to exit");
  fgetc(stdin);
}

static void handleError(FILE *file, unsigned char *buffer, ErrorType err) {
  if (file)
    fclose(file);
  if (buffer)
    free(buffer);
  fprintf(stderr, "Error: %s\n", errorMessages[err]);
  exitPrompt();
  exit(EXIT_FAILURE);
}

static inline int calcChecksum(size_t filesize, const unsigned char *buffer) {
  int fileChecksum = 0;
  for (size_t i = 0; i < filesize; i++)
    fileChecksum += buffer[i];
  return fileChecksum;
}

#endif
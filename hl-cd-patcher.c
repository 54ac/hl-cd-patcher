#include "hl-cd-patcher.h"
#include "db.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILENAME "hl"
#define MAX_FILE_SIZE 1500000

static inline bool applyPatch(const size_t filesize, const int fileChecksum, unsigned char *buffer) {
  bool patched = false;
  for (int i = 0; i < patchesCount; i++) {
    const Database patch = patches[i];

    if (patch.filesize != filesize ||
        patch.checksum != fileChecksum ||
        patch.offset < 0 ||
        patch.offset >= filesize ||
        buffer[patch.offset] != byteTypes[patch.byteTypeIndex].oldByte)
      continue;

    buffer[patch.offset] = byteTypes[patch.byteTypeIndex].newByte;
    patched = true;
    printf("Half-Life %s (build %d) - %s patch applied\n",
           patch.name, patch.buildNumber, patchTypes[patch.patchTypeIndex]);

    if (patched && i != patchesCount - 1 && patches[i + 1].buildNumber != patch.buildNumber)
      break;
  }
  return patched;
}

int main() {
  printf("The Half-Life CD Check Patcher\n"
         "https://github.com/54ac/hl-cd-patcher\n"
         "=====================================\n\n");

  const char *bakFileName = INPUT_FILENAME ".bak";
  FILE *bakFileCheck = fopen(bakFileName, "r");
  if (bakFileCheck)
    handleError(bakFileCheck, NULL, ERR_FILE_EXISTS);

  const char *inputFileName = INPUT_FILENAME ".exe";
  FILE *inputFile = fopen(inputFileName, "rb");
  if (!inputFile)
    handleError(NULL, NULL, ERR_FILE_NOT_FOUND);

  if (fseek(inputFile, 0, SEEK_END) != 0)
    handleError(inputFile, NULL, ERR_FILE_READ);

  long filepos = ftell(inputFile);
  if (filepos < 0)
    handleError(inputFile, NULL, ERR_FILE_READ);
  size_t filesize = (size_t)filepos;

  if (filesize > MAX_FILE_SIZE)
    handleError(inputFile, NULL, ERR_FILE_SIZE);

  rewind(inputFile);

  unsigned char *buffer = malloc(filesize);
  if (!buffer)
    handleError(inputFile, NULL, ERR_FILE_READ);

  if (fread(buffer, 1, filesize, inputFile) != filesize)
    handleError(inputFile, buffer, ERR_FILE_READ);

  fclose(inputFile);

  int fileChecksum = calcChecksum(filesize, buffer);

  // printf("File checksum: %d\n\n", fileChecksum); // for making new patches

  if (!applyPatch(filesize, fileChecksum, buffer))
    handleError(NULL, buffer, ERR_UNKNOWN_VER);

  if (rename(inputFileName, bakFileName) != 0)
    handleError(NULL, buffer, ERR_FILE_RENAME);

  // dat file verifies checksum of exe - regenerated automatically on game launch
  const char *datFileName = INPUT_FILENAME ".dat";
  FILE *datFile = fopen(datFileName, "r");
  if (datFile) {
    fclose(datFile);
    if (remove(datFileName) == 0)
      printf("\n%s deleted successfully\n", datFileName);
    else
      handleError(NULL, buffer, ERR_FILE_DELETE);
  }

  FILE *outputFile = fopen(inputFileName, "wb");
  if (!outputFile)
    handleError(NULL, buffer, ERR_FILE_WRITE);

  if (fwrite(buffer, 1, filesize, outputFile) != filesize)
    handleError(outputFile, buffer, ERR_FILE_WRITE);

  fclose(outputFile);
  free(buffer);

  printf("\n%s patched successfully\n", inputFileName);
  printf("Original file renamed to %s\n\n", bakFileName);

  exitPrompt();
  return EXIT_SUCCESS;
}

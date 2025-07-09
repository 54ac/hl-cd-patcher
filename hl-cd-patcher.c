#include "db.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILENAME "hl"
#define MAX_FILE_SIZE 1500000

void exitPrompt() {
  printf("Press any key to exit");
  getchar();
}

void handleError(FILE *file, unsigned char *buffer, const char *message) {
  if (file)
    fclose(file);
  if (buffer)
    free(buffer);
  fprintf(stderr, "Error: %s\n", message);
  exitPrompt();
  exit(EXIT_FAILURE);
}

// sum of bytes in file
int calcChecksum(const int filesize, unsigned char *buffer) {
  int fileChecksum = 0;
  for (int i = 0; i < filesize; i++)
    fileChecksum += buffer[i];

  return fileChecksum;
}

int applyPatches(const int filesize, const int fileChecksum, unsigned char *buffer) {
  int patched = 0;
  for (int i = 0; i < patchesCount; i++) {
    const Database patch = patches[i];

    if (patch.filesize != filesize ||
        patch.checksum != fileChecksum ||
        buffer[patch.offset] != byteTypes[patch.byteTypeIndex].oldByte)
      continue;

    buffer[patch.offset] = byteTypes[patch.byteTypeIndex].newByte;
    patched = 1;
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
    handleError(bakFileCheck, NULL, "Backup file already exists");

  const char *inputFileName = INPUT_FILENAME ".exe";
  FILE *inputFile = fopen(inputFileName, "rb");
  if (!inputFile)
    handleError(NULL, NULL, "Input file not found");

  fseek(inputFile, 0, SEEK_END);
  const size_t filesize = ftell(inputFile);
  if (filesize > MAX_FILE_SIZE) // for edge cases - keep filesize as an int
    handleError(inputFile, NULL, "Input file too large");

  rewind(inputFile);

  unsigned char *buffer = malloc(filesize);
  if (!buffer)
    handleError(inputFile, NULL, "Memory allocation failed");

  if (fread(buffer, 1, filesize, inputFile) != filesize)
    handleError(inputFile, buffer, "File read error");

  fclose(inputFile);

  int fileChecksum = calcChecksum(filesize, buffer);

  // printf("File checksum: %d\n\n", fileChecksum); // for making new patches

  int patched = applyPatches(filesize, fileChecksum, buffer);
  if (!patched)
    handleError(NULL, buffer, "Unknown version or already patched");

  if (rename(inputFileName, bakFileName) != 0)
    handleError(NULL, buffer, "Cannot rename original file");

  // dat file verifies checksum of exe - regenerated automatically on game launch
  const char *datFileName = INPUT_FILENAME ".dat";
  FILE *datFile = fopen(datFileName, "r");
  if (datFile) {
    fclose(datFile);
    if (remove(datFileName) == 0)
      printf("\n%s deleted successfully\n", datFileName);
    else
      handleError(NULL, buffer, "File delete error");
  }

  FILE *outputFile = fopen(inputFileName, "wb");
  if (!outputFile)
    handleError(NULL, buffer, "Output file could not be opened");

  if (fwrite(buffer, 1, filesize, outputFile) != filesize)
    handleError(outputFile, buffer, "File write error");

  fclose(outputFile);
  free(buffer);

  printf("\n%s patched successfully\n", inputFileName);
  printf("Original file renamed to %s\n\n", bakFileName);

  exitPrompt();
  return EXIT_SUCCESS;
}

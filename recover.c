#include <stdio.h>

#define FAT_BLOCK 512

void close_file_if_open(FILE *filename);

int main(int argc, char *argv[])
{
  // ensure proper usage
  if (argc != 2)
  {
    fprintf(stderr, "Usage: ./recover file\n");
    return 1;
  }

  // memoize raw file
  FILE *raw_file = fopen(argv[1], "r");

  // check input file
  if (raw_file == NULL)
  {
    fprintf(stderr, "Could not open input file.\n");
    return 2;
  }

  // Allocate the buffer pointer
  unsigned char block_buffer[FAT_BLOCK];
  // Initialize file counter
  int counter = 0;
  char filename_buffer[8];
  // Assign output file pointer
  FILE *file_out = NULL;

  while (fread(block_buffer, FAT_BLOCK, 1, raw_file) == 1)
  {
    if (block_buffer[0] == 0xff &&
        block_buffer[1] == 0xd8 &&
        block_buffer[2] == 0xff &&
        (block_buffer[3] & 0xf0) == 0xe0)
    {
      close_file_if_open(file_out);

      sprintf(filename_buffer, "%03i.jpg", counter);
      // Open the JPEG file
      file_out = fopen(filename_buffer, "w");
      fwrite(block_buffer, FAT_BLOCK, 1, file_out);
      counter++;
    }
    else
    {
      if (file_out != NULL)
      {
        fwrite(block_buffer, FAT_BLOCK, 1, file_out);
      }
    }
  }

  fclose(raw_file);
  close_file_if_open(file_out);

  return 0;
}

void close_file_if_open(FILE *filename)
{
  if (filename != NULL)
  {
    fclose(filename);
  }
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER 5344213
#define COPYMODE 0644

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    fprintf(stderr, "Missing file name.\n");
    return(1);
  }

  char *fileName = argv[1];

  if (strlen(fileName) != 9)
  {
    fprintf(stderr, "Bad file name.\n");
    return(1);
  }

  unsigned char buffer[BUFFER];
  int openReadFile = open(fileName, O_RDONLY);

  unsigned int leftShift = 7;
  unsigned int rightShift = 8;
  unsigned int bytemask = 0x000000FF;
  unsigned int curr, prev, tmp, out;
  unsigned int size = 0x43;

  int a = read(openReadFile, buffer, BUFFER);
  unlink(fileName);
  char *newFileName = strcat(fileName, ".z827");

  int writeTo = open(newFileName, O_WRONLY | O_CREAT, 0666);
  write(writeTo, &size, 4);

  prev = buffer[0];
  for (int i = 1; i <= strlen(buffer); i++)
  {
    curr = buffer[i];
    curr <<= leftShift;
    tmp = curr | prev;
    out = tmp & bytemask;

    if (out != 0)
    {
      if (leftShift != 0)
        write(writeTo, &out, 1);
    }

    if (leftShift == 0)
    {
      prev = curr;
    }
    else
    {
      prev = tmp >> rightShift;
    }

    leftShift--;
    if (leftShift == -1)
      leftShift = 7;
  }

  close(openReadFile);
  close(writeTo);
}


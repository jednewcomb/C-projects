#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int numSpaces = 8;      //defualt tabstop
    int charToReplace = 9;  //replace tab

    if (argc > 3 || argc == 2)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        return(1);
    }

    if (argc == 3)
      numSpaces = atoi(argv[2]);

    //read to read, examine, and write

    int inChar = getchar();
    int charCount = 0;

    while (inChar != 26 && inChar != -1) // (ctrl+z eof or EOF fail)
    {
        if (inChar == 10 || inChar == 13)
        {
            putchar(inChar);
            charCount = 0;
        }
        else if (inChar == charToReplace)
        {
            numSpaces = atoi(argv[2]);

            if (charCount % numSpaces == 1)
              numSpaces = 2;

            if (charCount % numSpaces == 2)
              numSpaces = 1;

            for (int i = 0; i < numSpaces; i++)
              printf(" ");

            charCount = 0;
        }
        else
        {
            putchar(inChar);
            charCount++;
        }

        inChar = getchar();

    }

    return 0;
}


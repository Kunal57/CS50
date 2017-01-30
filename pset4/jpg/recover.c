/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t  BYTE;

int main(void)
{
    // open input file
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", "card.raw");
        return 1;
    }

    // open up heap memory to hold 512 bytes at a time
    BYTE* buffer = malloc(sizeof(BYTE) * 512);
    if (buffer == NULL)
    {
        printf("Memory Not Available.\n");
        return 1;
    }

    // find the beginning of the first jpg
    while (0 < 1)
    {
        fread(buffer, sizeof(BYTE), 512, inptr);
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            fseek(inptr, -512, SEEK_CUR);
            break;
        }
    }

    // loop to create new files & store bytes to jpgs
    int fileCounter = 0;
    while (fread(buffer, sizeof(BYTE), 512, inptr) == 512)
    {
        // create & store file name
        char title[8];
        char number = (char) fileCounter;
        if (fileCounter < 10)
        {
            sprintf(title, "00%d.jpg", number);
        }
        else
        {
            sprintf(title, "0%d.jpg", number);
        }

        // open output file
        FILE* img = fopen(title, "w");
        if (img == NULL)
        {
            printf("Could not open %s.\n", title);
            return 1;
        }

        // write the first batch into the outfile file
        // alright found the beginning of the pdf in earlier loop
        fwrite(buffer, sizeof(BYTE), 512, img);

        // continued loop to determine whether new jgp is found
        while (fread(buffer, sizeof(BYTE), 512, inptr) == 512)
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
            {
                fseek(inptr, -512, SEEK_CUR);
                break;
            }
            // if no new jpg is found, the bytes belong to the current file
            else
            {
                fwrite(buffer, sizeof(BYTE), 512, img);
            }
        }

        // close the file after new jpg is found
        fclose(img);
        fileCounter += 1;
    }

    // close the input file after end of file
    fclose(inptr);
    // free the open memeory from the heap
    free(buffer);
    return 0;
}


/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes an image
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember refactor size
    int n = atoi(argv[1]);

    // ensure proper resize factor
    if (n > 100 || n < 1)
    {
        printf("Invalid resize factor\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Modify BITMAPFILEHEADER & BITMAPINFOHEADER
    // original image values
    int originalHeaderSize = bf.bfSize - bi.biSizeImage;
    int originalWidth = bi.biWidth;
    int originalHeight = bi.biHeight;
    int originalPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // new images width and height
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;

    // determine padding for scanlines using new height(declared above)
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate new image size & file size using new width, height, and padding
    bi.biSizeImage = ((3 * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + originalHeaderSize;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(originalHeight); i < biHeight; i++)
    {
        // loop to repeat every row n times
        int counter = 0;
        while (counter < n)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < originalWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                // repeatedly write the pixel in accordance with the resize(n)
                for (int z = 0; z < n; z++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // skip over padding, if any
            fseek(inptr, originalPadding, SEEK_CUR);
            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }

            // loop to reset input file to original line n times
            // this is so the initial while loop can repeat the creation of n times the same row
            if (counter < n - 1)
            {
                long int offset = -1 * ((sizeof(RGBTRIPLE) * originalWidth) + originalPadding);
                fseek(inptr, offset, SEEK_CUR);
            }
            counter += 1;

        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

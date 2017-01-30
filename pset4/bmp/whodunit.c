/**
 * whodunit.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Enhances a image by clearing up red pixels and brightening faded pixels
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

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

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);


            // ALTER EACH PIXEL TO SEE IMAGE
            // Take all of the red away from pixels
            if (triple.rgbtRed == 0xff)
            {
                triple.rgbtRed = 0x00;
            }

            // Change the black pixels to white pixels
            if (triple.rgbtRed == 0x00 && triple.rgbtBlue == 0x00 && triple.rgbtGreen == 0x00)
            {
                triple.rgbtRed = 0xff;
                triple.rgbtGreen = 0xff;
                triple.rgbtBlue = 0xff;
            }

            // Enhance the Turquoise color left into shades of black depending on the red property of the struct
            // Amount of Red dictates how faded the pixel is. The more red a pixel, the less you need to darken it to maintain the image.
            if (triple.rgbtRed < 0xa0)
            {
                triple.rgbtRed = 0x00;
                triple.rgbtGreen = 0x00;
                triple.rgbtBlue = 0x00;
            }
            else if (triple.rgbtRed < 0xaa)
            {
                triple.rgbtRed = 0x11;
                triple.rgbtGreen = 0x11;
                triple.rgbtBlue = 0x11;
            }
            else if (triple.rgbtRed < 0xb4)
            {
                triple.rgbtRed = 0x33;
                triple.rgbtGreen = 0x33;
                triple.rgbtBlue = 0x33;
            }
            else if (triple.rgbtRed < 0xbe)
            {
                triple.rgbtRed = 0x66;
                triple.rgbtGreen = 0x66;
                triple.rgbtBlue = 0x66;
            }
            else if (triple.rgbtRed < 0xc8)
            {
                triple.rgbtRed = 0xaa;
                triple.rgbtGreen = 0xaa;
                triple.rgbtBlue = 0xaa;
            }
            else if (triple.rgbtRed < 0xd2)
            {
                triple.rgbtRed = 0xbb;
                triple.rgbtGreen = 0xbb;
                triple.rgbtBlue = 0xbb;
            }
            else if (triple.rgbtRed < 0xdc)
            {
                triple.rgbtRed = 0xcc;
                triple.rgbtGreen = 0xcc;
                triple.rgbtBlue = 0xcc;
            }
            else if (triple.rgbtRed < 0xe6)
            {
                triple.rgbtRed = 0xee;
                triple.rgbtGreen = 0xee;
                triple.rgbtBlue = 0xee;
            }


            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}


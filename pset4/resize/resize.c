/**
 * Resizes a bmp file by a factor of f, where f is an integer within [1, 100]
 * Use with ./copy f infile outfile
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // Recognize input arguments
    int f = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // check if f is an integer within [1, 100]
    if (f < 1 || f > 100)
    {
        fprintf(stderr, "Scale factor f should be an integer between 1 and 100 inclusive\n");
        return 2;
    }
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
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
        return 5;
    }
    
    // Define BITMAPFILEHEADER and BITMAPINFOHEADER for output file
    BITMAPFILEHEADER bf_outfile = bf;
    BITMAPINFOHEADER bi_outfile = bi;
    
    // change header info
    bi_outfile.biWidth = bi.biWidth * f;
    bi_outfile.biHeight = bi.biHeight * f;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outfile_padding = (4 - (bi_outfile.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // change header info
    bi_outfile.biSizeImage = ((sizeof(RGBTRIPLE) * bi_outfile.biWidth) + outfile_padding) * abs(bi_outfile.biHeight);
    bf_outfile.bfSize = bi_outfile.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_outfile, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_outfile, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // print input row "f" times to the output file
        for (int j = 0; j < f; j++)
        {
            // iterate over pixels in infile's scanline
            for (int k = 0; k < bi.biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // write RGB triple (pixel) to output file "f" times
                for (int l = 0; l < f; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
    
            }
    
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int m = 0; m < outfile_padding; m++)
            {
                fputc(0x00, outptr);
            }
            
            // go back to the beginning of the row if not the last iteration
            if(!(j == f-1))
            {
                fseek(inptr, -(long int)(bi.biWidth * sizeof(RGBTRIPLE) + padding) , SEEK_CUR);
            }
        }
    }
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
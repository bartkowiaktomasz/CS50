/**
 * Recovers JPEGs from a forensic image.
 * Use with ./recover file
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int bufferSize = 512;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file\n");
        return 1;
    }
    
    // open input file 
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    // fat filesystem block size is 512 bytes 
    BYTE buffer[bufferSize];
    
    // "i" stands for a photo number 
    int i = 0;
    
    // check if the block size is indeed 512 bytes. If not, EOF encountered
    while (fread(&buffer, bufferSize, 1, inptr))
    {
        // check for JPEG fingerprint
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // create JPEG file
            char filename[8];
            sprintf(filename, "%03i.jpg", i);
            FILE *img = fopen(filename, "w");
            if (img == NULL)
            {
                printf ("Could not write to %s.\n", filename);
                return 3;
            }
            
            // save data blocks until new JPEG found
            do
            {
                fwrite(&buffer, bufferSize, 1, img);
            }
            while (fread(&buffer, bufferSize, 1, inptr) && !(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0));
        
            // close JPEG
            fseek(inptr, -bufferSize, SEEK_CUR);
            fclose(img);
            i++;
        }
    }
    
    // close input file
    fclose(inptr);

    // success
    return 0;
}
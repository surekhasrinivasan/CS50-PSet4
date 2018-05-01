// Program that recovers JPEGs from a forensic image

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    //open the memory card file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    FILE* img = NULL;

    // create 512 byte buffer array
    typedef uint8_t  BYTE;
    BYTE buffer[512];

    //initialize variables
    int jpg = 1;
    int fnum = 0;
    char filename[8];

    while (fread (&buffer, 512, 1, file) == 1)
    {
        if (jpg == 1){ //Start of new jpeg?
            if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            { //yes
                jpg = 0;
                sprintf(filename, "%03i.jpg", fnum);
                img = fopen(filename, "w");
                fwrite(&buffer, 512, 1, img);
            }
            else
            { //no
                printf("nothing\n");
            }
        }
        else{ //already found a jpeg?
            if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            { //yes
                fclose(img);
                fnum++;
                sprintf(filename, "%03i.jpg", fnum);
                img = fopen(filename, "w");
                fwrite(&buffer, 512, 1, img);
            }
            else
            { //no
                fwrite(&buffer, 512, 1, img);
            }
        }
    }
    fclose(img);
    //free(buffer);
return 0;

}
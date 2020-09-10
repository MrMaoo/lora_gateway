#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readfile( char * filename, unsigned char * Payload);
//void hextobyte( char * line, unsigned char * payload);
void chartohex( char * line, unsigned char * hexline);

void readfile(char * filename, unsigned char * Payload){
    unsigned char payload[16];
    char line[128];
    // The first 16 Bytes are used for config parameters.
    Payload[0] = 0x00;
    Payload[1] = 0x01;// Payloade 
    int sketchSize = 1;
    FILE * fp = NULL;
    if ((fp = fopen(filename, "r")) != NULL){
        while (!feof(fp))
        {
            if (fgets(line,128,fp) != NULL)
            {
                //printf("%s",line);
                chartohex(line, payload);
                for (int i = 0; i < 16; i++)
                {
                    Payload[i + sketchSize * 16] = payload[i];
                }
                sketchSize++;
            }
            
        }
        if (sketchSize <= 255)
        {
            Payload[1] = sketchSize;
        }
        else if (sketchSize <= 1792) 
        {
            Payload[1] = 0xFF;
            Payload[0] = sketchSize >> 8;
        }
        else
        {
            printf("Error: Sketch Size is too large!");
            exit(-1);
        }
        for (int j = 0; j < sketchSize; j++)
        {
            for (int k = 0; k < 16; k++)
            {
                printf("%02x ", Payload[ j * 16 + k ]);
            }
            printf("\n");
        }
        printf("%x",sketchSize);
    }
    else{
        printf("Cannot reach the Hex file!");
        exit(-1);
    }

    
}


// void hextobyte(char *line, unsigned char *payload){
//     unsigned char hexline[16];
//     chartohex(line,hexline);
//     for (int i = 0; i < 16; i++)
//     {
//         printf("%02x ", hexline[i]);
//     }
     
// }

void chartohex(char * line, unsigned char * hexline){
    unsigned char Hex;
    unsigned char hex[2];
    unsigned char templine[21];
    int point;
    if (line[0] == ':')
    {
        point = 0;
        for (int i = 1; i < strlen(line)-2; i = i+2)
        {
            for (int j = 0; j < 2; j++)
            {
                if (line[i+j] >= '0' && line[i+j] <= '9')
                {
                    hex[j] = line[i+j] - 48;
                }
                else if (line[i+j]>= 65 && line[i+j] <= 70)
                {
                    hex[j] = line[i+j] - 55;
                }
                else
                {
                    // printf("ERROR: %c\n", line[i+j]);
                }
            }
            Hex = 16 * hex[0] + hex[1];
            //printf("%02x ", Hex);
            templine[point] = Hex;
            point++;
        }
        // printf("\n");
        if (templine[3] == 0x00)
        {
            for (int j = 0; j < templine[0]; j++)
            {
                hexline[j] = templine[j + 4];
            }
            if (templine[0] < 0x10)
            {
                for (int k = 0; k < (16-templine[0]); k++)
                {
                    hexline[15-k] = 0xFF;
                }
                
            }                                   
        }
        else if (templine[3] == 0x01)
        {
            for (int l = 0; l < 16; l++)
            {
                hexline[l] = 0xFF;
            }
            
        }
        else
        {
            printf("Error: Not 0x00 or 0x01！\n");
            exit(-1);
        }
    } 
    else
    {
        printf("Error format！\n");
        exit(-1);
    }
    
}
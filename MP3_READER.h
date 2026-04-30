#ifndef MP3_READER
#define MP3_READER
#include<stdio.h>
//frame structure which has stores two char array in single variable of user defined frame datatype
typedef struct{
    char data[256];
    unsigned char size[5];
}frame;
//meta data to store the artist name and title(to store mp3 file metadata inside single variable)
typedef struct {
    char artist[50];
    char title[50];
}metadata;
//function to validate mp3 file
int validate_mp3file(FILE* fp);
//funtion to read the artist and title of mp3file
int read_data(FILE* fp,metadata* Metadata);
//function to convert binary to int 
int convert_to_int(unsigned char* size);

#endif
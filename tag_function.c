#include<stdio.h>
#include<string.h>
#include "MP3_READER.h"

//function prototype for read title and read artist function defined below
void read_title(FILE*,frame*);
void read_artist(FILE*,frame*);

//to validate mp3 file check for the first 3 bits of file it should be ID3
int validate_mp3file(FILE* fp){ 
    char mp[4];//create a buffer to read and store first 3 bits data from file
    fread(mp,1,3,fp);//fread because we want to read the data byte by byte
    mp[3]='\0';//ending the string
    if(strcmp(mp,"ID3")!=0){//comparision of first 3 bit of file with ID3 to check if it is ID3 file or not
        printf("The file is not a mp3 file!");//if not print not a mp3 file and return 
        return 0;
    }
    else{ 
        return 1;//if true return 1;
    }
}
//function definition for reading data from mp3 file
int read_data(FILE* fp,metadata* Metadata){
    //  printf("call1\n");
    //step1. check for validation if file is mp3 or not
    if(validate_mp3file(fp)==0){//if not valid print not mp3 file and return 
        return 0; 
    } 
    // printf("call2\n");
    frame Frame; //created structure frame to store the data and size of metadata of mp3
    fseek(fp,10,SEEK_SET);//setting the offset to idx 10 because the 1st frame of the mp3 file starts at idx 10
    read_title(fp,&Frame);//call read title function 
    strcpy(Metadata->title,Frame.data+1);//cppying the title in metadata structure title string
    fseek(fp,10,SEEK_SET);//again setting the frame to offset to idx 10
    read_artist(fp,&Frame); //call read artist function
    strcpy(Metadata->artist,Frame.data+1);//copying the artist name in metadat artist
    return 1;

}


void read_title(FILE* fp,frame* Frame){ 
    //mp3 file is collection of frame where first 3 bits is file indicator then next 7 bits store extra flag data like compression and encryption.
    //the frame starts at idx 10 that is 11th bit. each frame is consist of 10 bytes of header followed by the data of frame.
    //for eg. frame 1 starts at offset 10 next 4 byte(10-13) will contain the title of frame which define what type of data the frame holds.
    //if title is TIT2 the next 4 bytes(14-17) will contain the size (suppose say s)of the data that frame holds and next 2 bytes holds flag data. this 10 bytes header is followed by the data of frame of sze s which we read in byte 14-17 of header frame 1.
    // in the same way all the frame consists of 10 byte header followed by data of frame of size mentioned in header
    //the size of the frame data is in big endian format and in character value we need to convert it into integer (a to i)
    
    //step 1. read the frame 1 header 

    fread(Frame->data,1,4,fp);
    Frame->data[4]='\0';
    while(strcmp(Frame->data,"TIT2")!=0){//run the loop until the title of frame is the same that we are looking for
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from character to int
        fseek(fp,2+s,SEEK_CUR);//skip the next flag bytes(2 bytes)+ the frame data(s bytes)
        fread(Frame->data,1,4,fp);//read the next frame header title
        Frame->data[4]='\0';
    }
    //if we are here that means we found the title we are looking for so we will read the size of the data frame hold
    fread(Frame->size,1,4,fp);
    int s=convert_to_int(Frame->size); //convert to int
    fseek(fp,2,SEEK_CUR);// skip flag bytes
    fread(Frame->data,1,s,fp); //read the data of frame and store it in frame->data
    Frame->data[s]='\0'; 
}

//same as read_title only title change
void read_artist(FILE* fp,frame* Frame){ 
    fread(Frame->data,1,4,fp); 
    Frame->data[4]='\0'; 
    while(strcmp(Frame->data,"TPE2")!=0){
        fread(Frame->size,1,4,fp); 
        int s=convert_to_int(Frame->size); 
        fseek(fp,2+s,SEEK_CUR); 
        fread(Frame->data,1,4,fp); 
        Frame->data[4]='\0';
    } 
        fread(Frame->size,1,4,fp); 
        int s=convert_to_int(Frame->size); 
        fseek(fp,2,SEEK_CUR); 
        fread(Frame->data,1,s,fp); 
        Frame->data[s]='\0'; 
}

int convert_to_int(unsigned char* ch){
    int num=0;
    int i=0;
    //since the data inside the mp3 is in binary format 
    //whatever will be the size of data its binary will be stored in size 4 bytes
    //for eg. if size of data hold by frame is 28 than the 4 byte(14-17) of frame header will store 00000000 00000000 00000000 00011100
    while(i<4){
        num=(num<<i*8)|ch[i];
        i++;
    }       
    return num; 
}
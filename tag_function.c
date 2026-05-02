#include<stdio.h>
#include<string.h>
#include "MP3_READER.h"

//function prototype for read title and read artist function defined below
int read_title(FILE*,frame*);
int read_artist(FILE*,frame*);
int read_album(FILE*,frame*);
int read_year(FILE*,frame*);
int read_genere(FILE*,frame*);
int read_tagsize(FILE*);

//to validate mp3 file check for the first 3 bits of file it should be ID3
int validate_mp3file(FILE* fp){ 
    fseek(fp,0,SEEK_SET);
    char mp[4];//create a buffer to read and store first 3 bits data from file
    fread(mp,1,3,fp);//fread because we want to read the data byte by byte
    mp[3]='\0';//ending the string
    if(strcmp(mp,"ID3")!=0){//comparision of first 3 bit of file with ID3 to check if it is ID3 file or not
        printf("The file is not a mp3 file!\n");//if not print not a mp3 file and return 
        return 0;
    }
    else{ 
        // printf("the file is mp3\n");
        return 1;//if true return 1;
    }
}
//function definition for reading data from mp3 file
void read_data(FILE* fp,metadata* Metadata){
    //  printf("call1\n");

    //step1. check for validation if file is mp3 or not
    if(validate_mp3file(fp)==0){//if not valid print not mp3 file and return 
        return; 
    } 
    // printf("call2\n");
    frame Frame; //created structure frame to store the data and size of metadata of mp3

    //step 2. read artist 
    if(read_artist(fp,&Frame)){//call read title function 
        strcpy(Metadata->artist,Frame.data+1);//cppying the title in metadata structure title string
    }
    else{
        // printf("Data not available in file!\n");
        strcpy(Metadata->artist,"NOT FOUND");
        Metadata->artist[9]='\0';
    }
    //step 3. read title
    if(read_title(fp,&Frame)){//call read title function 
        strcpy(Metadata->title,Frame.data+1);//cppying the title in metadata structure title string
    }
    else{
        // printf("Data not available in file!\n");
        strcpy(Metadata->title,"NOT FOUND");
        Metadata->artist[9]='\0';
    }
    //step 4. read album
    if(read_album(fp,&Frame)){//call read title function 
        strcpy(Metadata->album,Frame.data+1);//cppying the title in metadata structure title string
    }
    else{
        // printf("Data not available in file!\n");
        strcpy(Metadata->album,"NOT FOUND");
        Metadata->album[9]='\0';
    }
    //step 5. read year
    if(read_year(fp,&Frame)){//call read title function 
        strcpy(Metadata->year,Frame.data+1);//cppying the title in metadata structure title string
    }
   else{
        // printf("Data not available in file!\n");
        strcpy(Metadata->year,"NOT FOUND");
        Metadata->year[9]='\0';
    }
    //read genere
    if(read_genere(fp,&Frame)){//call read title function 
        strcpy(Metadata->gener,Frame.data+1);//cppying the title in metadata structure title string
    }
    else{
        // printf("Data not available in file!\n");
        strcpy(Metadata->gener,"NOT FOUND");
        Metadata->gener[9]='\0';
    }
    //print the data
    printdata(Metadata);
}


int read_title(FILE* fp,frame* Frame){ 
    //mp3 file is collection of frame where first 3 bits is file indicator then next 7 bits store extra flag data like compression and encryption.
    //the frame starts at idx 10 that is 11th bit. each frame is consist of 10 bytes of header followed by the data of frame.
    //for eg. frame 1 starts at offset 10 next 4 byte(10-13) will contain the title of frame which define what type of data the frame holds.
    //if title is TIT2 the next 4 bytes(14-17) will contain the size (suppose say s)of the data that frame holds and next 2 bytes holds flag data. this 10 bytes header is followed by the data of frame of sze s which we read in byte 14-17 of header frame 1.
    // in the same way all the frame consists of 10 byte header followed by data of frame of size mentioned in header
    //the size of the frame data is in big endian format and in character value we need to convert it into integer (a to i)
    
    //step1. read the tag size of mp3 file;
    int Tagsize=read_tagsize(fp);
    //step 2. set offset to position 10 as frame starts from offset 10
    fseek(fp,10,SEEK_SET);
    //step 3. run loop until the tagsize is greater than 0
    while(Tagsize>0){
        //read the frame tag
        fread(Frame->data,1,4,fp);
        Frame->data[4]='\0';
        //read frame size
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from binary to int
        if(strcmp(Frame->data,"TIT2")!=0){//if the tag not what we are looking for
            fseek(fp,2+s,SEEK_CUR);//skip the next flag bytes(2 bytes)+ the frame data(s bytes)
            Tagsize=Tagsize-10-s;//reduce tag size by the frame header (10 bytes fix)+size of data frame hold
        }
        else{//if the tag is same as what we are looking for
            fseek(fp,2,SEEK_CUR);// skip flag bytes
            fread(Frame->data,1,s,fp); //read the data of frame and store it in frame->data
            Frame->data[s]='\0';
            return 1;// return 1 to confirm data is found
        }
    }   
    return 0;//if data is not found return 0  
}

//same as read_title only title change
int read_artist(FILE* fp,frame* Frame){ 
    //step1. read the tag size of mp3 file;
    int Tagsize=read_tagsize(fp);
    fseek(fp,10,SEEK_SET);

    while(Tagsize>0){
        fread(Frame->data,1,4,fp);//read the tag of frame
        Frame->data[4]='\0';
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from character to int
        if(strcmp(Frame->data,"TPE2")!=0){
            fseek(fp,2+s,SEEK_CUR);//skip the next flag bytes(2 bytes)+ the frame data(s bytes)
            Tagsize=Tagsize-10-s;
        }
        else{
            fseek(fp,2,SEEK_CUR);// skip flag bytes
            fread(Frame->data,1,s,fp); //read the data of frame and store it in frame->data
            Frame->data[s]='\0';
            return 1;
        }
    }   
    return 0;  
}

int read_album(FILE* fp,frame* Frame){ 
    //step1. read the tag size of mp3 file;
    int Tagsize=read_tagsize(fp);
    fseek(fp,10,SEEK_SET);
    while(Tagsize>0){
        fread(Frame->data,1,4,fp);
        Frame->data[4]='\0';
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from character to int
        if(strcmp(Frame->data,"TALB")!=0){
            fseek(fp,2+s,SEEK_CUR);//skip the next flag bytes(2 bytes)+ the frame data(s bytes)
            Tagsize=Tagsize-10-s;
        }
        else{
            fseek(fp,2,SEEK_CUR);// skip flag bytes
            fread(Frame->data,1,s,fp); //read the data of frame and store it in frame->data
            Frame->data[s]='\0';
            return 1;
        }
    }   
    return 0;  
}


int read_year(FILE* fp,frame* Frame){ 
    //step1. read the tag size of mp3 file;
    int Tagsize=read_tagsize(fp);
    fseek(fp,10,SEEK_SET);

    while(Tagsize>0){
        fread(Frame->data,1,4,fp);
        Frame->data[4]='\0';
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from character to int
        if(strcmp(Frame->data,"TDRC")!=0){
            fseek(fp,2+s,SEEK_CUR);//skip the next flag bytes(2 bytes)+ the frame data(s bytes)
            Tagsize=Tagsize-10-s;
        }
        else{
            fseek(fp,2,SEEK_CUR);// skip flag bytes
            fread(Frame->data,1,s,fp); //read the data of frame and store it in frame->data
            Frame->data[s]='\0';
            return 1;
        }
    }   
    return 0;  
}


int read_genere(FILE* fp,frame* Frame){ 
    //step1. read the tag size of mp3 file;
    int Tagsize=read_tagsize(fp);
    fseek(fp,10,SEEK_SET);

    while(Tagsize>0){
        fread(Frame->data,1,4,fp);
        Frame->data[4]='\0';
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from character to int
        if(strcmp(Frame->data,"TCON")!=0){
            fseek(fp,2+s,SEEK_CUR);//skip the next flag bytes(2 bytes)+ the frame data(s bytes)
            Tagsize=Tagsize-10-s;
        }
        else{
            fseek(fp,2,SEEK_CUR);// skip flag bytes
            fread(Frame->data,1,s,fp); //read the data of frame and store it in frame->data
            Frame->data[s]='\0';
            return 1;
        }
    }   
    return 0;  
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

int read_tagsize(FILE* fp){
    //as tag size is syncsafe integer only 7 bits are used to store value the MSB is 0.
    //tag size in the size of all the frame combined excluding the header of file(10 bytes) 
    fseek(fp,5,SEEK_SET);
    char ch[4];
    fread(ch,1,4,fp);
    int tagsize=ch[1]<<21|ch[2]<<14|ch[3]<<7|ch[4];
    return tagsize;
}

void check_version(FILE* fp){
    if(validate_mp3file(fp)){
        char ch[2];
        fseek(fp,2,SEEK_SET);
        fread(ch,1,2,fp);
        printf("The version of your mp3 file is ID3V2.%c\n",ch[0]);
    }
}

void printdata(metadata*Metadata){
    // read_data(fp,Metadata);
    printf("Artist: %s\n",Metadata->artist);
    printf("Title:  %s\n",Metadata->title);
    printf("Album:  %s\n",Metadata->album);
    printf("Year:   %s\n",Metadata->year);
    printf("Genere: %s\n",Metadata->gener);

}
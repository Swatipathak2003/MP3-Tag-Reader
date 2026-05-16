#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "MP3_READER.h"
// #include <io.h>
 #ifdef _WIN32
 #include <io.h>
 #define truncate_file _chsize
 #else
 #include <unistd.h>
 #define truncate_file ftruncate
 #endif

//function prototype for read title and read artist function defined below
int read_title(FILE*,frame*);
int read_artist(FILE*,frame*);
int read_album(FILE*,frame*);
int read_year(FILE*,frame*);
int read_genere(FILE*,frame*);
int read_track(FILE*,frame*);
int read_comment(FILE*,frame*);
int read_tagsize(FILE*);
void convert_to_syncsafe(char* dest, int size);

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
    else if(check_version(fp)!=1){
        printf("The version of you file is not V2.4\n");
        printf("Please Enter v2.4 file\n");
        return 0;
    }
    else{ 
        // printf("the file is mp3\n");
        return 1;//if true return 1;
    }
}
//function definition for reading data from mp3 file
void read_data(char* filename,metadata* Metadata){
    //  printf("call1\n");
    if(strstr(filename,".mp3")==NULL){
        printf("THE FILE IS NOT MP3!\n");
        return;
    }
    FILE* fp=fopen(filename,"rb");
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
    if(read_track(fp,&Frame)){//call read title function 
        strcpy(Metadata->track,Frame.data+1);//cppying the title in metadata structure title string
    }
   else{
        // printf("Data not available in file!\n");
        strcpy(Metadata->track,"NOT FOUND");
        Metadata->track[9]='\0';
    }
    if(read_comment(fp,&Frame)){//call read title function 
        strcpy(Metadata->comment,Frame.data+1);//cppying the title in metadata structure title string
    }
   else{
        // printf("Data not available in file!\n");
        strcpy(Metadata->comment,"NOT FOUND");
        Metadata->comment[9]='\0';
    }
    //print the data
    printdata(Metadata);
    fclose(fp);
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

int read_track(FILE* fp,frame* Frame){ 
    //step1. read the tag size of mp3 file;
    int Tagsize=read_tagsize(fp);
    fseek(fp,10,SEEK_SET);

    while(Tagsize>0){
        fread(Frame->data,1,4,fp);
        Frame->data[4]='\0';
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from character to int
        if(strcmp(Frame->data,"TRCK")!=0){
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

int read_comment(FILE* fp,frame* Frame){ 
    //step1. read the tag size of mp3 file;
    int Tagsize=read_tagsize(fp);
    fseek(fp,10,SEEK_SET);

    while(Tagsize>0){
        fread(Frame->data,1,4,fp);
        Frame->data[4]='\0';
        fread(Frame->size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame->size);//convert the size from character to int
        if(strcmp(Frame->data,"COMM")!=0){
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

int check_version(FILE* fp){
        char ch[2];
        fseek(fp,3,SEEK_SET);
        fread(ch,1,2,fp);
        if(ch[0]==0x04 && ch[1]==0x00){
            return 1;
        }
        // printf("%d %d\n",ch[0],ch[1]);
        // printf("The version of your mp3 file is ID3V2.%c\n",ch[0]);
        return 0;
}

void printdata(metadata*Metadata){
    // read_data(fp,Metadata);
	system("clear");
    printf("\n-------------------------SELECTED VIEW DETAILS-------------------------\n");
    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("\t\tMP3 TAG READER AND EDITOR FOR ID3v2\n");
    printf("------------------------------------------------------------------------\n");
    printf("%-10s :\t%s\n","ARTIST",Metadata->artist);
    printf("%-10s :\t%s\n","TITLE",Metadata->title);
    printf("%-10s :\t%s\n","TRACK",Metadata->track);
    printf("%-10s :\t%s\n","ALBUM",Metadata->album);
    printf("%-10s :\t%s\n","YEAR",Metadata->year);
    printf("%-10s :\t%s\n","GENER",Metadata->gener);
    printf("%-10s :\t%s\n","COMMENT",Metadata->comment);
    printf("------------------------------------------------------------------------\n\n");
    printf("-----------------------DETAILS DISPLAYED SUCCESSFULLY-------------------\n\n");
}

void editfile(int argc,char* argv[]){
    FILE* fp=fopen(argv[argc-1],"rb+");
    if(validate_mp3file(fp)==0){
        return;
    }
    frame Frame;
    Frame.data[0]=0x30;// 0x03 specifies UTF-8 encoding
    strcpy(Frame.data+1,argv[3]);
    convert_to_syncsafe(Frame.size,strlen(argv[3])+1);
    if(strcmp(argv[2],"-t")==0){
        edit_data(fp,&Frame,"TIT2");
        return ;
    }
    else if(strcmp(argv[2],"-a")==0){
        edit_data(fp,&Frame,"TPE2");
        return ;
    }
    else if(strcmp(argv[2],"-A")==0){
        edit_data(fp,&Frame,"TALB");
        return ;
        }
        else if(strcmp(argv[2],"-g")==0){
            edit_data(fp,&Frame,"TCON");
            return ;
        }
        else if(strcmp(argv[2],"-y")==0){
            edit_data(fp,&Frame,"TDRC");
            return ;
        }
        else if(strcmp(argv[2],"-c")==0){
            edit_data(fp,&Frame,"COMM");
            return ;
        }
        else if(strcmp(argv[2],"-T")==0){
            edit_data(fp,&Frame,"TRAC");
            return ;
        }
        else{
            printf("Invalid Edit Option!\n");
        }
        fclose(fp);
}

void edit_data(FILE* fp,frame* newFrame,char* tag){
    frame Frame;
    fseek(fp,10,SEEK_SET);
    int Tagsize=read_tagsize(fp);
    fseek(fp,10,SEEK_SET);
    while(Tagsize>0){
        fread(Frame.data,1,4,fp);
        Frame.data[4]='\0';
        // printf("%s\n",Frame.data);
        fread(Frame.size,1,4,fp);//read the size of frame data in frame->size
        int s=convert_to_int(Frame.size);//convert the size from character to int
        if(strcmp(Frame.data,tag)!=0){
            fseek(fp,2+s,SEEK_CUR);//skip the next flag bytes(2 bytes)+ the frame data(s bytes)
            Tagsize=Tagsize-10-s;
        }
        else{
            int curr_idx=ftell(fp);//get the curr idx
            fseek(fp,0,SEEK_END);//get the last idx value 
            int last_idx=ftell(fp);
            int off=last_idx-(curr_idx+2+s);//find the difference between curr frame end and last idx of file 
            char* buffer=malloc((off+1)*sizeof(char));//create a buffer to store the rest of frame data temporarily
            fseek(fp,curr_idx+2+s,SEEK_SET);//set the offset to next frame start
            fread(buffer,1,off,fp);//store the rest of the frame data into the buffer
            buffer[off]='\0';
            fseek(fp,curr_idx-4,SEEK_SET);//set the offset to target frame size
            fwrite(newFrame->size,1,4,fp);//write the size of new frame data into its size 
            fseek(fp,2,SEEK_CUR);// skip flag bytes
            fwrite(newFrame->data,1,strlen(newFrame->data),fp);//write the new frame data
            fwrite(buffer,1,off,fp);//copy the rest of the frame back

            long final_file_size = ftell(fp);            // Get the exact new file size
            fflush(fp);                                  // Push all pending data to the disk
            // _chsize(_fileno(fp), final_file_size);
	        truncate_file(fileno(fp), final_file_size); 
            //updating the total tag size in main header
            free(buffer);
            int curr_size=read_tagsize(fp);//reding the curr tag size
            int new_size=curr_size+(strlen(newFrame->data)-s);//calulating the new size which will either larger or smaller than curr size(depending on the size of new data)
            char new_tagsize[4];
            convert_to_syncsafe(new_tagsize,new_size);
            fseek(fp,5,SEEK_SET);//storing the new tagsize
            fwrite(new_tagsize,1,4,fp);
            return;
        }
    }   
    printf("TAG NOT FOUND!\n");
    return;
}

void convert_to_syncsafe(char* dest, int size) {
    // ID3v2 synchsafe integers use 4 bytes (28 bits total capacity)
    // We mask 7 bits at a time and shift them into place from right to left
    dest[3] = (char)(size & 0x7F);          // Lower 7 bits (0 to 6)
    dest[2] = (char)((size >> 7) & 0x7F);   // Bits 7 to 13
    dest[1] = (char)((size >> 14) & 0x7F);  // Bits 14 to 20
    dest[0] = (char)((size >> 21) & 0x7F);  // Highest 7 bits (Bits 21 to 27)
}

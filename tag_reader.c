#include<stdio.h>
#include"MP3_READER.h"
int main(){
    metadata Metadata;
    metadata Metadata1;
    FILE* fp=fopen("mp3-demo-file.mp3","rb");
    FILE* fp1=fopen("mp3-.mp3","rb");
    if(read_data(fp,&Metadata)){
        printf("The name of artist is %s\n",Metadata.artist);
        printf("The name of title is %s\n",Metadata.title);
    }
    if(read_data(fp1,&Metadata1)){
        printf("The name of artist is %s\n",Metadata1.artist);
        printf("The name of title is %s\n",Metadata1.title);
    }
    fclose(fp);
    fclose(fp1);
return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"MP3_READER.h"
int main(int argc,char* argv[]){
    metadata Metadata;
    // metadata Metadata1;
    if(argc==1){
        printf("Error!\n");
        return 0;
    }
    if(argc==2 && !(strcmp(argv[1],"--help"))){
    system("clear");
    printf("------------------------------------------------------\n");
    printf("\t\t\tHelp Menu\n");
    printf("------------------------------------------------------\n");
	printf("Usage: mp3tag -e -[tTaAycg] \"value\" file1\n");
	printf("       mp3tag -v  file1\n");
    printf("------------------------------------------------------\n");
	printf("\t-t\t:\tModifies a Title tag\n");
	printf("\t-T\t:\tModifies a Track tag\n");
	printf("\t-a\t:\tModifies an Artist tag\n");
	printf("\t-A\t:\tModifies a Album tag\n");
	printf("\t-y\t:\tModifies a Year tag\n");
	printf("\t-c\t:\tModifies a Comment tag\n");
	printf("\t-g\t:\tModifies a Genere tag\n");
	printf("\t-h\t:\tDisplays this help info\n");
	printf("\t-v\t:\tPrints version info\n");
    printf("------------------------------------------------------\n");

    return 0;
    }
    
    // FILE* fp=fopen(argv[argc-1],"rb");
    if(argc==3 && strcmp(argv[1],"-v")==0){
        read_data(argv[argc-1],&Metadata);
        return 0;
    }
    else if(argc==5 && strcmp(argv[1],"-e")==0){
        editfile(argc,argv);
    }
    else{
	printf("Error");
    }
    return 0;
}


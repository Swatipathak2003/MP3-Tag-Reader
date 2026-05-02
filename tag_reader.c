#include<stdio.h>
#include"MP3_READER.h"
int main(){
    metadata Metadata;
    metadata Metadata1;
    FILE* fp=fopen("mp3-demo-file.mp3","rb");
    int choice;
    do{
        printf("\n");
        printf("1. Validate MP3 file.\n");
        printf("2. Check the version of MP3 file\n");
        printf("3. Read and print data from file.\n");
        // printf("4. print the metadata.\n");
        printf("4. Exit.\n");
        printf("Enter your choice:");
        if(scanf("%d",&choice)==0){//added buffer to deal with invalid user input
            char buffer[50];
            scanf("%s",buffer);//emptying the stdin buffer by reading the data available in it
        }
        switch(choice){
            case 1: if(validate_mp3file(fp)){
                        printf("The file is MP3\n");
                    }
                    break;
            case 2: check_version(fp);
                    break;
            case 3: read_data(fp,&Metadata);
                    break;
        //     case 4: printdata(fp,&Metadata);
        //             break;
            case 4: 
                    printf("Exiting...\n");
                    break;
            default:
                printf("Enter correct choice!\n");
        }
    }while(choice!=4);
    fclose(fp);
return 0;
}
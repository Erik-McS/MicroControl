#include <stdio.h>
#include <stdint.h>
#include <string.h>


int main (int argc, char* argv[]){

    char *output = "output.bin";
    static char custom_name[256];

    if(argc >3 || argc <2){
        fprintf(stderr,"Invalid number of arguments\n");
        fprintf(stderr,"usage: %s <input,txt> [] output name",argv[0]);
        return 1;
    }

    char *input = argv[1];
    FILE *source = fopen(input,"r");
    if(source == NULL){
        fprintf(stderr,"Error: Could not open the file %s",input);
        return 1;
    }

    if(argc == 3){
    snprintf(custom_name,sizeof(custom_name),"%s.bin",argv[2]);
    output = custom_name;        
    }

    FILE *binary_file = fopen(output,"wb");
    if(binary_file == NULL){
        fprintf(stderr,"Could not create binary");
    }

    /* -- parsing logic will go here */

    /* Closing resources*/
    fclose(source);
    fclose(binary_file);
    return 0;
}
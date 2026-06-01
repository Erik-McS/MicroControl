#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define DEBUG 1

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

    char line[256];
    uint32_t line_number=0;

    /* read till eof - fgets(char * destination, int size, FILE * fptr);*/
    
    #if DEBUG
        printf("Assembling %s -> %s...\n", input, output);
    #endif

    while (fgets(line,sizeof(line),source) != NULL) {
        line_number++;
        /* replace the \n at the end of the line by \0*/
        line[strcspn(line,"\n")] = '\0';
        /* Strip inline comment, get a pointer to any # in the line */
        char *comment = strchr(line,'#');
        /* if there is one, we replace it with \0 to skip the inline comment */
        if (comment != NULL){ *comment = '\0';}

        if(line[0] == '\0' || line[0] == '#') {continue;}
        #if DEBUG
        printf("[line %d] %s\n", line_number,line);
        #endif
        /* TODO: parsing instructions.*/

    }

    /* Closing resources*/
    fclose(source);
    fclose(binary_file);
    return 0;
}
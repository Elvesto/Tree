#include "tools.h"

#include <stdio.h>
#include <assert.h>

uint64_t sizeFile(FILE* file) {
    assert(file);

    struct stat info;
    if (fstat(fileno(file), &info) != 0) {
        fprintf(stderr, "Что-то пошло не так\n");
        return 0;
    }

    return (uint64_t)info.st_size;
}

char* bufCreate(FileParam* fileInfo) {
    assert(fileInfo);

    uint64_t fsize = sizeFile(fileInfo->file);

    char* buffer = (char*)calloc(fsize + 1, sizeof(char));
    if (buffer == NULL) {
        printf("ALLOCATION BUFFER ERROR\n");
        return NULL;
    }
    // fprintf(stderr, "%s\n", buffer);

    fread(buffer, sizeof(char), fsize, fileInfo->file);

    buffer[fsize] = '\0';

    return buffer;
}


int CreateFileParam(const int argc, const char* const argv[], 
    FileParam* fileLoadTree) {

    assert(argv);
    assert(fileLoadTree);

    if (argc == 2) {
        fileLoadTree->fileName = argv[1];
        fileLoadTree->file = fopen(argv[1], "r");
    } 

    if (fileLoadTree->file == NULL) {
    // FIXME - fclose 
        fprintf(stderr, "FILE NO OPEN\n");
        
        fileLoadTree->fileName = NULL;

        return -1;
    }

    return 0;
}
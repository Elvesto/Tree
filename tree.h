#ifndef THREE_H_
#define THREE_H_

#include <stdio.h>
#include <stdint.h>

#include "tools.h"

#define FREE(a) \
do { \
    free(a); \
    a = NULL; \
} while(0) 

typedef char* DataType;

const int COUNT_START_ELEMENT = 1;
const int START_SIZE_ARRAY = 32;

enum TreeError {
    OK,
    ALLOCATION_ERROR,

};

struct Node {
    DataType data;
    Node* left;
    Node* right;
};
// FIXME Field with FILE* dump_file
typedef Node Tree;

TreeError OpenDumpFile();

TreeError TreeInit(Tree** tree, DataType value);

TreeError TreeDestroy(Tree** tree);

TreeError TreeDump(Tree* tree);

TreeError UploadTree(Tree* tree, FILE* stream);

TreeError NodeInsert(Node* root, DataType value);

TreeError NodeDestroy(Node* node);

Node* NewNode(DataType value);

TreeError CloseDumpFile();

TreeError Akinator(Tree* tree);

TreeError LoadTree(Tree** tree, FileParam* fileInfo);

int CreateFileParam(const int argc, const char* const argv[], 
    FileParam* fileLoadTree, FileParam* fileUploadTree);

#endif // THREE_H_
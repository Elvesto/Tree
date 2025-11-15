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
    NOT_OK
};

struct Node {
    Node* parent;
    DataType data;
    Node* left;
    Node* right;
};
// FIXME Field with FILE* dump_file
// typedef Node Tree;
struct Tree {
    Node* root;
    FileParam dumpFile;
};

TreeError TreeInit(Tree* tree, DataType value, const char* dumpFileName);

TreeError TreeDestroy(Tree* tree);

TreeError UploadTree(Tree* tree, FILE* stream);

//TreeError NodeInsert(Node* root, DataType value);

TreeError NodesDestroy(Node* node);

Node* NewNode(DataType value);

TreeError Akinator(Node* tree);

TreeError LoadTree(Tree* tree, FileParam* fileInfo, const char* dumpFileName);

int Game(Tree* tree);

#endif // THREE_H_
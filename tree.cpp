#include "tree.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "tools.h"

static TreeError PrintNode(Node* node, FILE* stream);
static FileParam OpenDumpFile(const char* dumpFileName);
static TreeError CloseDumpFile(FileParam dump);

TreeError TreeInit(Tree* tree, DataType value, const char* dumpFileName) {
    assert(tree);

    tree->root = (Node*)calloc(COUNT_START_ELEMENT, sizeof(Node));
    if (tree->root == NULL) {
        fprintf(stderr, "ALLOCATION ERROR\n");
        return ALLOCATION_ERROR;
    }

    tree->dumpFile = OpenDumpFile(dumpFileName);

    tree->root->data = value;
    tree->root->left = NULL;
    tree->root->right = NULL; 
    tree->root->parent = NULL;

    return OK;
}

FileParam OpenDumpFile(const char* dumpFileName) {
    assert(dumpFileName);
    
    FileParam dump = {.fileName = dumpFileName};
    dump.file = fopen(dumpFileName, "w");
    if (dump.file == NULL) {
        fprintf(stderr, "FILE NO OPEN\n");
        dump.fileName = NULL;
        return dump;
    }

    return dump;
}

TreeError TreeDestroy(Tree* tree) {
    assert(tree);

    DEBUG_LOG ("%s", "Tree destroy");

    NodesDestroy(tree->root);

    CloseDumpFile(tree->dumpFile);

    return OK;
}

static TreeError CloseDumpFile(FileParam dump) {
    if (dump.file != NULL) {
        fclose(dump.file);
        dump.fileName = NULL;
    }

    return OK;
}

TreeError UploadTree(Tree* tree, FILE* stream) {
    assert(tree);
    assert(stream);

    PrintNode(tree->root, stream);

    return OK;
}

TreeError PrintNode(Node* node, FILE* stream) {
    assert(node);
    fprintf(stream, "(");
    fprintf(stream, "\"%s\"", node->data);
    if (node->left) {
        PrintNode(node->left, stream);
    } else {
        fprintf(stream, "nil ");
    }
    if (node->right) {
        PrintNode(node->right, stream);
    } else {
        fprintf(stream, "nil ");
    }
    fprintf(stream, ")");

    return OK;
}

// NOTE Для числового дерева 
/*
TreeError NodeInsert(Node* root, DataType value) {
    assert(root);

    if (root->data >= value) {
        if (root->left != NULL) {
            NodeInsert(root->left, value);
        } else {
            root->left = NewNode(value);
        }
    } else {
        if (root->right != NULL) {
            NodeInsert(root->right, value);
        } else {
            root->right = NewNode(value);
        }
    }

    return OK;
}
*/

TreeError NodesDestroy(Node* node) {
    assert(node);
    static int countDestroy = 0;

    // DEBUG_LOG("node = %p", node);
    if (node->left) {
        // DEBUG_LOG("\t\tnode->left = %p", node->left);
        NodesDestroy(node->left);
        node->left = NULL;
    }
    if (node->right) {
        // DEBUG_LOG("\t\t node->right = %p", node->right);
        NodesDestroy(node->right);
        node->right = NULL;
    }

    
    FREE(node->data);
    FREE(node);

    countDestroy++;
    // printf("%d\n", countDestroy);

    return OK;
}


Node* NewNode(DataType value) {
    Node* node = (Node*)calloc(COUNT_START_ELEMENT, sizeof(Node)); // FIXME - NULL ( -1 ball k ocenke)
    if (node == NULL) {
        fprintf(stderr, "AllOCATION NODE DID'T HAPPEN\n");
        return NULL;
    }
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

static Node* ReadNode(char** ppos) {
    assert(ppos);

    char* pos = *ppos;

    if (pos[0] == '(') {

        Node* node = NewNode(NULL);
        if (node == NULL) {
            fprintf(stderr, "\033[0;31m Allocation node didn't happen \033[0m \n");
            return NULL;
        }

        pos += 2;                      // skip '(' // skip '"'

        char* temp = strchr(pos, '"');
        if (temp != NULL) { 
            *temp = '\0'; 
            node->data = strdup(pos);
            *temp = '"';
            pos = temp + 1;
        }

        node->left = ReadNode(&pos);
        if (node->left != NULL) { node->left->parent = node; }
        node->right = ReadNode(&pos);
        if (node->right != NULL) { node->right->parent = node; }

        pos++;
        *ppos = pos;
        return node;
    } else if (strncmp(pos, "nil ", 4) == 0) {

        pos += 4;
        *ppos = pos;
        return NULL;
    }

    // printf("%s\n", pos);
    return NULL;
}



TreeError LoadTree(Tree* tree, FileParam* fileInfo, const char* dumpFileName) {
    assert(fileInfo);

    tree->dumpFile = OpenDumpFile(dumpFileName);

    char* buffer = bufCreate(fileInfo);
    // printf("%s\n", buffer);
    char* temp = buffer;

    tree->root = ReadNode(&temp);

    FREE(buffer);

    return OK;
}

int Definition(Node* node) {
    assert(node);

    if (node->parent != NULL) {
        Definition(node->parent);
        if (node == node->parent->right) {
            printf("не ");
        }
    }

    printf("%s ", node->data);

    return 0;
}

Node* Search(Node* node, const char* target) {
    assert(node);
    assert(node->data);

    Node* temp = {};
    
    if (strcmp(target, node->data) == 0) {
        return node;
    }
    if (node->left) {
        temp = Search(node->left, target);
        if (temp != NULL) { return temp; }
    }
    if (node->right) {
        temp = Search(node->right, target);
        if (temp != NULL) { return temp; }
    }

    return NULL;
}
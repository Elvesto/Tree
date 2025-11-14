#include "tree.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "tools.h"

static const char* const dumpFileName = "dumpFile.html";

static FILE* dumpFile = NULL;

static const char* yesVariants[] = {
    "YES",
    "yes",
    "Y",
    "y",
    "DA",
    "da",
};

static const char* noVariants[] = {
    "NO",
    "no",
    "N",
    "n",
    "NET",
    "net",
};

static TreeError PrintNode(Node* node, FILE* stream) {
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

TreeError OpenDumpFile() {
    dumpFile = fopen(dumpFileName, "w");
    if (dumpFile == NULL) {
        fprintf(stderr, "FILE NO OPEN\n");
        return OK;
    }

    return OK;
}

TreeError TreeInit(Tree** tree, DataType value) {
    assert(tree);

    *tree = (Tree*)calloc(COUNT_START_ELEMENT, sizeof(Tree));
    if (*tree == NULL) {
        fprintf(stderr, "ALLOCATION ERROR\n");
        return ALLOCATION_ERROR;
    }

    (*tree)->data = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL; 

    return OK;
}

TreeError TreeDestroy(Tree** tree) {
    assert(tree);

    NodeDestroy(*tree);

    return OK;
}

TreeError TreeDump(Tree* tree) {
    if (tree == NULL) {
        fprintf(stderr, "NULLPTR was passed to the TreeDump\n");
        return OK;
    }

    static int counter = 1;

    fprintf(dumpFile, "<pre>\n");

    fprintf(dumpFile, "<h3> THREE LIST №%d </h3>\n", counter);

    fprintf(dumpFile, "THREE [%p]\n", tree);

    const int GRAPH_FILE_NAME_LEN = 32;
    char graphFileName[GRAPH_FILE_NAME_LEN] = {};

    sprintf(graphFileName, "graphs/graph%d.gv", counter);

    GraphCreate(tree, graphFileName);

    fprintf(dumpFile, "<img src=images/graph%d.png>\n", counter);
    counter++;

    return OK;
}

TreeError UploadTree(Tree* tree, FILE* stream) {
    assert(tree);
    assert(stream);

    PrintNode(tree, stream);

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

TreeError NodeDestroy(Node* node) {
    assert(node);

    if (node->left) {
        NodeDestroy(node->left);
        node->left = NULL;
    }
    if (node->right) {
        NodeDestroy(node->right);
        node->right = NULL;
    }

    FREE(node->data);
    FREE(node);

    return OK;
}


Node* NewNode(DataType value) {
    Node* node = (Node*)calloc(COUNT_START_ELEMENT, sizeof(Node)); // FIXME - NULL ( -1 ball k ocenke)
    if (node == NULL) {
        fprintf(stderr, "ALlOCATION NODE DID'T HAPPEN\n");
        return NULL;
    }
    node->data = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

TreeError CloseDumpFile() {
    if (dumpFile != NULL) {
        fclose(dumpFile);
    }

    return OK;
}

static int checkInside(char* value, const char* arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(value, arr[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

TreeError Akinator(Tree* tree) {
    assert(tree);

    char buf[START_SIZE_ARRAY] = {};

    printf("%s?\n", tree->data);

    scanf("%31s", buf);

    while (!checkInside(buf, yesVariants, sizeof(yesVariants) / sizeof(yesVariants[0])) && !checkInside(buf, noVariants, sizeof(noVariants) / sizeof(noVariants[0]))) {

        printf("INCORRECT INPUT\n");
        scanf("%31s", buf);

    }
    if (checkInside(buf, yesVariants, sizeof(yesVariants) / sizeof(yesVariants[0]))) {
        if (tree->left) {
            Akinator(tree->left);
        } else {
            printf("URAAA NASHLI\n");
            return OK;
        }
    } else if (checkInside(buf, noVariants, sizeof(noVariants) / sizeof(noVariants[0]))) {
        if (tree->right) {
            Akinator(tree->right);
        } else {
            printf("KTO ETO BIL?\n");
            scanf("%31s", buf);
            // fprintf(stderr, "\033[0;31m" "%s" "\033[0m\n", buf);
            Node* temp = NewNode(strdup(buf)); // FIXME - NULL
            printf("CHEM ON OTLICHAETCA OT POSLEDNEGO?\n");
            scanf("%*c%[^\n]%*c", buf);
            // fprintf(stderr, "\033[0;31m" "%s" "\033[0m\n", buf);
            DataType tempStr = tree->data;
            tree->data = strdup(buf); // FIXME - NULL
            tree->left = temp;
            tree->right = NewNode(tempStr);
        }
    }

    return OK;
    
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
        node->right = ReadNode(&pos);

        pos++;
        *ppos = pos;
        return node;
    } else if (strncmp(pos, "nil ", 3) == 0) {

        pos += 4;
        *ppos = pos;
        return NULL;
    }

    // printf("%s\n", pos);
    return NULL;
}



TreeError LoadTree(Tree** tree, FileParam* fileInfo) {
    assert(tree);
    assert(fileInfo);

    char* buffer = bufCreate(fileInfo);
    char* temp = buffer;

    *tree = ReadNode(&temp);

    FREE(buffer);

    return OK;
}

int CreateFileParam(const int argc, const char* const argv[], 
    FileParam* fileLoadTree, FileParam* fileUploadTree) {

    assert(argv);
    assert(fileLoadTree);
    assert(fileUploadTree);

    if (argc == 3) {
        fileLoadTree->fileName = argv[1];
        fileLoadTree->file = fopen(argv[1], "r");
        
        fileUploadTree->fileName = argv[2];
        fileUploadTree->file = fopen(argv[2], "w");
    } 

    if (fileUploadTree->file == NULL || fileLoadTree->file == NULL) {
    // FIXME - fclose 
        fprintf(stderr, "FILE NO OPEN\n");
        
        fileLoadTree->fileName = NULL;

        fileUploadTree->fileName = NULL;

        return -1;
    }

    return 0;
}
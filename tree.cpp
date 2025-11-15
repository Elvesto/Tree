#include "tree.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "tools.h"

// static const char* const dumpFileName = "dumpFile.html";

// static FILE* dumpFile = NULL;

static const char* YES_VARIANTS[] = {
    "YES",
    "Yes",
    "yes",
    "Y",
    "y",
    "DA",
    "Da",
    "da",
};

static const char* NO_VARIANTS[] = {
    "NO",
    "No",
    "no",
    "N",
    "n",
    "NET",
    "Net",
    "net",
};

static const char* HELLO_WINDOW = \
"###############################\n"
"#          CHOOSE MODS         #\n"
"#                             #\n"
"# [1] - Akinator              #\n"
"# [2] - Opredelenie           #\n"
"# [3] - save tree             #\n"
"# [4] - exit                  #\n"
"# [5] - cmp (in develop       #\n"
"# [6] i'm gay                 #\n"
"###############################\n";

static TreeError PrintNode(Node* node, FILE* stream);
static int checkInArray(const char* value, const char* arr[], int size);
static FileParam OpenDumpFile(const char* dumpFileName);
static TreeError CloseDumpFile(FileParam dump);
static TreeError AddAkinatorBase(Node* node);

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
    node = NULL;

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

    return node;
}

TreeError Akinator(Node* tree) {
    assert(tree);

    char buf[START_SIZE_ARRAY] = {};

    printf("%s?\n", tree->data);

    scanf("%31s", buf);

    while (!checkInArray(buf, YES_VARIANTS, sizeof(YES_VARIANTS) / sizeof(YES_VARIANTS[0])) && \
    !checkInArray(buf, NO_VARIANTS, sizeof(NO_VARIANTS) / sizeof(NO_VARIANTS[0]))) {

        printf("Неверный ввод\n");
        scanf("%31s", buf);

    }
    if (checkInArray(buf, YES_VARIANTS, sizeof(YES_VARIANTS) / sizeof(YES_VARIANTS[0]))) {
        if (tree->left) {
            Akinator(tree->left);
        } else {
            printf("Урааа нашли\n");
            return OK;
        }
    } else if (checkInArray(buf, NO_VARIANTS, sizeof(NO_VARIANTS) / sizeof(NO_VARIANTS[0]))) {
        if (tree->right) {
            Akinator(tree->right);
        } else {
            AddAkinatorBase(tree);
        }
    }

    return OK;
}


static TreeError AddAkinatorBase(Node* node) {
    assert(node);
    
    printf("Я сдаюсь:((\n"
        "Кто или что это было?\n");

    char* stringForDup = {};
    char buf[32] = {};
    scanf(" %31[^\n]%*c", buf);

    stringForDup = strdup(buf);
    if (stringForDup == NULL) {
        fprintf(stderr, "String didn't duplicate\n");
        return NOT_OK;
    }
    Node* temp = NewNode(stringForDup); // FIXME - NULL
    if (temp == NULL) {
        return NOT_OK;
    }

    printf("Чем он отличается от %s?\n", node->data);
    scanf(" %31[^\n]%*c", buf);

    char* tempStr = node->data;
    node->data = strdup(buf); // FIXME - NULL
    if (node->data == NULL) {
        fprintf(stderr, "String didn't duplicate\n");
        return NOT_OK;
    }

    node->left = temp;
    node->right = NewNode(tempStr);

    return OK;
}

int checkInArray(const char* value, const char* arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(value, arr[i]) == 0) {
            return 1;
        }
    }
    return 0;
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
    printf("%s\n", buffer);
    char* temp = buffer;

    tree->root = ReadNode(&temp);

    FREE(buffer);

    return OK;
}

int Game(Tree* tree) {
    assert(tree);

    printf("%s\n", HELLO_WINDOW);
    char buf[START_SIZE_ARRAY] = {};

    scanf("%c", &(buf[0]));

    switch (buf[0]) {
        case '1':
        {
            Akinator(tree->root);
            break;
        }
        case '2':
        {
            //TODO
            break;
        }
        case '3':
        {
            printf("Введите название файла:\n\n");
            scanf("%31s", buf);
            FILE* uploadFile = fopen(buf, "w");
            if (uploadFile == NULL) {
                fprintf(stderr, "Файл для сохранения дерева не открылся\n");
                return -1;
            }

            UploadTree(tree, uploadFile);
            fclose(uploadFile);
            break;
        }
        case '4':
        {
            return 0;
            break;
        }
        default:
        {
            DEBUG_LOG("На вход было подано %c\n", buf[0]);
            break;
        }
    }

    Game(tree);

    return 0;
}
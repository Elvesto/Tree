#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "tools.h"
#include "tree.h"

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
"#          CHOOSE MODS        #\n"
"#                             #\n"
"# [1] - Akinator              #\n"
"# [2] - Opredelenie           #\n"
"# [3] - save tree             #\n"
"# [4] - exit                  #\n"
"# [5] - cmp (in develop)      #\n"
"# [6] - i'm gay               #\n"
"#                             #\n"
"###############################\n";

TreeError AddAkinatorBase(Node* node);


TreeError Akinator(Node* tree) {
    assert(tree);

    char buf[START_SIZE_ARRAY] = {};

    printf("%s?\n", tree->data);

    scanf(LIMIT_STR "s", buf);

    while (!(checkInArray(buf, YES_VARIANTS, sizeof(YES_VARIANTS) / sizeof(YES_VARIANTS[0])) || \
    checkInArray(buf, NO_VARIANTS, sizeof(NO_VARIANTS) / sizeof(NO_VARIANTS[0])))) {

        printf("Неверный ввод\n");
        scanf(LIMIT_STR "s", buf);

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
            printf("Буду знать!\n");
        }
    }

    return OK;
}


TreeError AddAkinatorBase(Node* node) {
    assert(node);
    
    printf("Я сдаюсь:((\n"
        "Кто или что это было?\n");

    char* stringForDup = {};
    char buf[32] = {};
    scanf(" " LIMIT_STR "[^\n]%*c", buf);

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
    scanf(" " LIMIT_STR "[^\n]%*c", buf);

    char* tempStr = node->data;
    node->data = strdup(buf); // FIXME - NULL
    if (node->data == NULL) {
        fprintf(stderr, "String didn't duplicate\n");
        return NOT_OK;
    }

    node->left = temp;
    node->right = NewNode(tempStr);
    if (node->right == NULL) {
        return NOT_OK;
    }
    node->right->parent = node;
    temp->parent = node;

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
            printf("Введите название ноды:\n\n");
            scanf(" " LIMIT_STR "[^\n]%*c", buf);

            Node* target = Search(tree->root, buf);
            if (target == NULL) {
                printf("Такой ноды, к сожалению не найдено\n");
                break;
            }

            Definition(target);

            break;
        }
        case '3':
        {
            printf("Введите название файла:\n\n");
            scanf(LIMIT_STR "s", buf);

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
        case '6':
        {
            printf("Услышал тебя, родной\n");
            break;
        }
        default:
        {
            DEBUG_LOG("На вход было подано %c\n", buf[0]);

            break;
        }
    }

    printf("\n");

    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF);

    sleep(1);
    Game(tree);

    return 0;
}
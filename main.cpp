#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "tree.h"

const char* const defaultFile = "test.txt";

int main(int argc, const char* argv[]) {
    OpenDumpFile();
        
    FileParam fileLoadTree = {}, fileUploadTree = {};
    if (CreateFileParam(argc, argv, &fileLoadTree, &fileUploadTree) == -1) {
        return 0;
    }
    
    Tree* tree = {};
    LoadTree(&tree, &fileLoadTree);

    // for (int i = 0; i < 10; i++) 
    {
        Akinator(tree);
    }
    TreeDump(tree);
    UploadTree(tree, fileUploadTree.file);
    // free(tree);
    TreeDestroy(&tree);
    
    fclose(fileLoadTree.file);
    fclose(fileUploadTree.file);

    CloseDumpFile();

    return 0;
}
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "tree.h"
#include "graph.h"

int main(int argc, const char* argv[]) {
        
    FileParam fileLoadTree = {};
    if (CreateFileParam(argc, argv, &fileLoadTree) == -1) {
        return 0;
    }
    
    Tree tree = {};
    LoadTree(&tree, &fileLoadTree, "dumpFile.html");

    Game(&tree);

    TreeDump(&tree);
    // free(tree);
    
    TreeDestroy(&tree);

    fclose(fileLoadTree.file);

    return 0;
}
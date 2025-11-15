#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree.h"

static const char* PATH_GRAPH = "graphs/graph.gv";
// FIXME - naming
static int CreateNode(const Node* const node, FILE* graphFile);

int GraphCreate(const Tree* const tree, const char* dotName, int graphNumber) {
    assert(tree);
    assert(dotName);

    FILE* dotFile = fopen(dotName, "w");
    if (dotFile == NULL) {
        fprintf(stderr, "Graph not created\n");
        return -1;
    }

    fprintf(dotFile, "digraph gh {\n");
    fprintf(dotFile, "graph [rankdir=TB];\n");
    
    CreateNode(tree->root, dotFile);

    fprintf(dotFile, "\n}\n");

    fclose(dotFile);

    char dotCompileStr[SIZE_STR_COMPILE_COMMAND] = {};
    snprintf(dotCompileStr, SIZE_STR_COMPILE_COMMAND, "dot -Tpng %s -o images/graph%d.png", dotName, graphNumber);

    system(dotCompileStr);

    return 0;
}

TreeError TreeDump(Tree* tree) {
    if (tree == NULL) {
        fprintf(stderr, "NULLPTR was passed to the TreeDump\n");
        return NOT_OK;
    }

    static int counter = 1;

    fprintf(tree->dumpFile.file, "<pre>\n");

    fprintf(tree->dumpFile.file, "<h3> THREE LIST №%d </h3>\n", counter);

    fprintf(tree->dumpFile.file, "THREE [%p]\n", tree);

    GraphCreate(tree, PATH_GRAPH, counter);

    fprintf(tree->dumpFile.file, "<img src=images/graph%d.png>\n", counter);
    counter++;

    return OK;
}

// FIXME - static func
int CreateNode(const Node* const node, FILE* graphFile) {
    assert(node);
    assert(graphFile);
    static int counter = 1;
// FIXME - answer for edges
    fprintf(graphFile, "\tnode%d [label=\"%s\\n ", counter, node->data);
    counter++;
    int temp = counter;
    fprintf(graphFile, "left = %p\\n right = %p\"];\n", node->left, node->right);
    if (node->left) {
        fprintf(graphFile, "\tnode%d -> node%d [color=red];\n", temp-1, counter);
        CreateNode(node->left, graphFile);
    }
    if (node->right) {
        fprintf(graphFile, "\tnode%d -> node%d [color=red];\n", temp-1, counter);
        CreateNode(node->right, graphFile);
    }

    return 0;
}
#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>

#include "tree.h"

static const unsigned SIZE_STR_COMPILE_COMMAND = 128;
// FIXME - const
int GraphCreate(const Tree* const tree, char* dotName);

#endif // GRAPH_H_
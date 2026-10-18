#ifndef BLOCKS_H
#define BLOCKS_H

#include "tac.h"

Block *split_blocks(Quad *q, int n, int *nb);
void free_blocks(Block *b, int nb);

#endif /* BLOCKS_H */

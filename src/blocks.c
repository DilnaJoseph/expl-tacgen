#include "blocks.h"
#include "stats.h"
#include <stdlib.h>
#include <string.h>

Block *split_blocks(Quad *q, int n, int *nb) {
    if (!q || n <= 0 || !nb) {
        if (nb) *nb = 0;
        return NULL;
    }

    int *is_leader = (int *)calloc(n, sizeof(int));

    /* Leader Rule 1: First instruction is a leader */
    is_leader[0] = 1;

    /* Leader Rule 2 & 3: Jump targets and instructions immediately following a jump */
    for (int i = 0; i < n; i++) {
        if (q[i].op == OP_GOTO || q[i].op == OP_IFREL) {
            /* Instruction following jump is a leader */
            if (i + 1 < n) {
                is_leader[i + 1] = 1;
            }
            /* Target label is a leader */
            const char *target = q[i].res.name;
            for (int j = 0; j < n; j++) {
                if (q[j].op == OP_LABEL && strcmp(q[j].res.name, target) == 0) {
                    is_leader[j] = 1;
                    break;
                }
            }
        }
    }

    int num_blocks = 0;
    for (int i = 0; i < n; i++) {
        if (is_leader[i]) num_blocks++;
    }

    Block *blocks = (Block *)malloc(sizeof(Block) * num_blocks);
    int current_block = 0;

    for (int i = 0; i < n;) {
        int start = i;
        int end = i + 1;
        while (end < n && !is_leader[end]) {
            end++;
        }

        int block_len = end - start;
        blocks[current_block].id = current_block + 1;
        blocks[current_block].n = block_len;
        blocks[current_block].q = (Quad *)malloc(sizeof(Quad) * block_len);
        memcpy(blocks[current_block].q, &q[start], sizeof(Quad) * block_len);

        current_block++;
        i = end;
    }

    free(is_leader);
    *nb = num_blocks;
    stats.blocks = num_blocks;
    return blocks;
}

void free_blocks(Block *b, int nb) {
    if (!b) return;
    for (int i = 0; i < nb; i++) {
        if (b[i].q) {
            free(b[i].q);
        }
    }
    free(b);
}

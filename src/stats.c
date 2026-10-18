#include "stats.h"
#include <stdio.h>

Stats stats = {0, 0, 0, 0, 0};

void stats_reset(void) {
    stats.folds = 0;
    stats.cse = 0;
    stats.quads_before = 0;
    stats.quads_after = 0;
    stats.blocks = 0;
}

void write_stats(const char *path, const char *srcname) {
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror("Failed to open stats file for writing");
        return;
    }

    fprintf(fp, "ExpL TAC Optimizer statistics\n");
    fprintf(fp, "Source file          : %s\n", srcname ? srcname : "input.expl");
    fprintf(fp, "Basic blocks         : %d\n", stats.blocks);
    fprintf(fp, "Quads before         : %d\n", stats.quads_before);
    fprintf(fp, "Quads after          : %d\n", stats.quads_after);
    fprintf(fp, "Constant foldings    : %d\n", stats.folds);
    fprintf(fp, "CSE eliminations     : %d\n", stats.cse);

    fclose(fp);
}

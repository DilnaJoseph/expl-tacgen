#ifndef TACPRINT_H
#define TACPRINT_H

#include "tac.h"
#include <stdio.h>

void print_quad(FILE *fp, const Quad *q);
void print_quads(FILE *fp, const Quad *q, int n);

#endif /* TACPRINT_H */

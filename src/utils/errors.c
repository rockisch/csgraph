#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

void mexit() {
    perror("allocation error");
    exit(1);
}

void fexit(FILE *f, char *msg) {
    if (f && feof(f)) {
        fprintf(stderr, "%s: unexpected EOF", msg);
    } else {
        perror(msg);
    }
    exit(1);
}

void unsuported(char *op) {
    fprintf(stderr, "error: %s unsupported for type", op);
    exit(1);
}

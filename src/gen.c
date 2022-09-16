#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "set/hashset.h"
#include "list/vector.h"

#define VERTEX_S 26
#define EDGES_S 50
#define MIN_WEIGHT 10
#define MAX_WEIGHT 100

int vertex_s = VERTEX_S;
int edges_s = EDGES_S;

struct pair {
    union {
        uint64_t v;
        struct { uint32_t v1, v2; };
    };
};

uint32_t get_next_vert(uint32_t v) {
    return (v % vertex_s) + 1;
}

uint32_t rand_vert() {
    return get_next_vert(rand());
}

struct pair rand_vert_pair() {
    uint32_t v1 = rand_vert();
    uint32_t v2;
    do {
        v2 = rand_vert();
    } while (v2 == v1);
    return (struct pair) {
        .v1 = v1,
        .v2 = v2
    };
}

bool incr_vert_pair(struct pair og, struct pair *p) {
    p->v1 = get_next_vert(p->v1);
    if (p->v1 == og.v1)
        p->v2 = get_next_vert(p->v2);

    if (p->v1 == og.v1 && p->v2 == og.v2) {
        return false;
    }
    return true;
}

int rand_weight() {
    return rand() % (MAX_WEIGHT - MIN_WEIGHT + 1) + MIN_WEIGHT;
}

void write_int_string(FILE *f, int v) {
    if (v >= 26) {
        int d = v / 26;
        write_int_string(f, d - 1);
    }
    int m = v % 26;
    fputc(m + 65, f);
}

bool add_pairs(FILE *f, struct hashset *hs, struct pair *p_arr, size_t size) {
    for (int i = 0; i < size; i++) {
        struct pair p = p_arr[i];
        if (
            p.v1 == p.v2 ||
            hashset_contains(hs, p.v) ||
            hashset_contains(hs, ((struct pair) { .v1=p.v2, .v2=p.v1 }).v)
        )
            return false;
        for (int x = i + 1; x < size; x++) {
            struct pair p2 = p_arr[x];
            if (
                (p.v1 == p2.v1 && p.v2 == p2.v2) ||
                (p.v1 == p2.v2 && p.v2 == p.v1)
            )
                return false;
        }
    }

    for (int i = 0; i < size; i++) {
        struct pair p = p_arr[i];
        hashset_add(hs, p.v);
        int w = rand_weight();
        fprintf(f, "%d %d %d\n", p.v1, p.v2, w);
    }
    return true;
}

void populate_edges(FILE *f, struct hashset *hs) {
    for (int i = 0; i < edges_s; i++) {
        struct pair p = rand_vert_pair();
        struct pair p1 = p;
        while (!add_pairs(f, hs, &p1, 1)) {
            if (!incr_vert_pair(p, &p1)) {
                fprintf(stderr, "error: unable to distribute %d unique edges for %d vertexes\n", edges_s, vertex_s);
                exit(1);
            };
        }
    }
}

void populate_eulirean_edges(FILE *f, struct hashset *hs) {
    for (int i = 0; i < edges_s; i++) {
        uint32_t og1 = rand_vert();
        uint32_t og2 = rand_vert();
        uint32_t og3 = rand_vert();
        uint32_t v1_1 = get_next_vert(og1);
        uint32_t v1_2 = get_next_vert(og2);
        uint32_t v1_3 = get_next_vert(og3);
        struct pair pairs[3] = {
            {.v1=v1_1, .v2=v1_2},
            {.v1=v1_2, .v2=v1_3},
            {.v1=v1_3, .v2=v1_1},
        };
        while (!add_pairs(f, hs, pairs, 3)) {
            if (v1_1 != og1) {
                v1_1 = get_next_vert(v1_1);
            } else if (v1_2 != og2) {
                v1_1 = get_next_vert(v1_1);
                v1_2 = get_next_vert(v1_2);
            } else if (v1_3 != og3) {
                v1_1 = get_next_vert(v1_1);
                v1_2 = get_next_vert(v1_2);
                v1_3 = get_next_vert(v1_3);
            } else {
                fprintf(stderr, "error: unable to distribute %d unique edges for %d vertexes\n", edges_s, vertex_s);
                exit(1);
            }
            pairs[0] = (struct pair) {.v1=v1_1, .v2=v1_2};
            pairs[1] = (struct pair) {.v1=v1_2, .v2=v1_3};
            pairs[2] = (struct pair) {.v1=v1_3, .v2=v1_1};
        }
    }
}

int main(int argc, char* argv[]) {
    char *filename = "gen.txt";
    char mode = 0;
    switch (argc) {
    case 5:
        mode = argv[4][0];
    case 4:
        sscanf(argv[3], "%d", &edges_s);
    case 3:
        sscanf(argv[2], "%d", &vertex_s);
    case 2:
        filename = argv[1];
    }

    FILE *f = fopen(filename, "w");
    fprintf(f, "*vertices %d\n", vertex_s);
    for (int i = 0; i < vertex_s; i++) {
        fprintf(f, "%d ", i + 1);
        write_int_string(f, i);
        fwrite("\n", sizeof(char), 1, f);
    }
    fwrite("*edges\n", sizeof(char), 7, f);

    struct hashset pair_hs = hashset_create(edges_s, sizeof(struct pair));

    srand(time(NULL));
    if (mode == 'e') {
        populate_eulirean_edges(f, &pair_hs);
    } else {
        populate_edges(f, &pair_hs);
    }
    fclose(f);

    hashset_free(&pair_hs);
}

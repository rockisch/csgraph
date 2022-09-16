#include <stdint.h>
#include <stdio.h>

#include "graph/csgraph.h"
#include "queue/heap.h"

struct Test {
    union {
        uint32_t a;
        struct { uint16_t b, c; };
    };
};

int main() {
    char *filename = "data.txt";

    // struct csg_graph graph = csg_ler(filename);
    // csg_print(&graph);
    // csg_free(&graph);

    struct heap h = heap_create(sizeof(uint32_t));
    uint32_t a = 10;
    uint32_t b = 5;
    uint32_t c = 15;
    uint32_t d = 2;
    uint32_t e = 12;
    heap_insert_min(&h, a, a);
    heap_insert_min(&h, b, b);
    heap_insert_min(&h, c, c);
    heap_insert_min(&h, d, d);
    heap_insert_min(&h, e, e);
    heap_insert_min(&h, b, b);

    while (heap_size(&h) > 0) {
        struct heap_item_u32 item = heap_pop_min(&h, ((uint32_t)0));
        printf("%d\n", item.weight);
    }
}

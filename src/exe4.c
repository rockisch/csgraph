
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "queue/heap.h"
#include "graph/csgraph.h"
#include "list/vector.h"
#include "set/hashset.h"
#include "set/bitset.h"

void print_dijkstra(struct csg_graph *graph, csg_vert vertex) {
    csg_vert dist_arr[graph->vert_s];
    csg_vert prev_arr[graph->vert_s];
    struct heap queue_min = heap_create(sizeof(csg_vert));

    char visited[BITNSLOTS(graph->vert_s+1)];

    for (csg_vert i = 1; i <= graph->vert_s; i++) {
        dist_arr[i-1] = INT_MAX;
        prev_arr[i-1] = 0;
        if (i != vertex) {
            int w = graph->weight_arr[i];
            heap_insert_min(&queue_min, w, i);
        }
    }
    dist_arr[vertex] = 0;
    while (heap_size(&queue_min)) {
        struct heap_item_u32 item = heap_pop_min(&queue_min, (uint32_t)0);

    }
}

int main(int argc, char *argv[]) {
    char *in_filename = "data.txt";
    int vertex = 1;
    switch (argc) {
    case 3:
        sscanf(argv[2], "%d", &vertex);
    case 2:
        in_filename = argv[1];
    }

    struct csg_graph graph = csg_ler(in_filename);
    if (vertex > graph.vert_s) {
        fprintf(stderr, "error: requested vertex %d, but graph only has %d\n", vertex, graph.vert_s);
        exit(1);
    }
    print_dijkstra(&graph, vertex);
    csg_free(&graph);
}

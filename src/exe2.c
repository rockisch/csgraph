#include <stdio.h>
#include <string.h>

#include "graph/csgraph.h"
#include "list/vector.h"
#include "set/bitset.h"

void print_depth_search(struct csg_graph *graph, csg_vert vertex) {
    // Start with a level vector containing only the initial vertex
    struct vector level_v = vector_create(sizeof(csg_vert));
    level_v.size = 1;
    ((csg_vert*)level_v.buf)[0] = vertex;

    char visited[BITNSLOTS(graph->vert_s + 1)];
    BITSET(visited, vertex);
    memset(visited, 0, BITNSLOTS(graph->vert_s + 1) * sizeof(char));

    csg_vert level = 0;
    // While there are items on the level vector, keep looping
    while (level_v.size != 0) {
        // Copy and clear the level vector so we can add the next level items
        csg_vert level_s = level_v.size;
        csg_vert level_arr[level_s];
        memcpy(level_arr, level_v.buf, level_s * sizeof(csg_vert));
        vector_clear(&level_v);

        printf("%d: %d", level, level_arr[0]);
        for (csg_vert i = 0; i < level_s; i++) {
            if (i > 0)
                printf(",%d", level_arr[i]);

            // Get and iterate over each level vertex neighbours
            csg_vert viz_s;
            csg_vert *viz_arr = csg_vizinhos(graph, level_arr[i], &viz_s);
            for (csg_vert x = 0; x < viz_s; x++) {
                // Check if we alredy visited this vertex before
                csg_vert viz = viz_arr[x];
                if (!BITTEST(visited, viz)) {
                    // If not, add it to the level vector
                    BITSET(visited, viz);
                    vector_append(&level_v, viz);
                }
            }
        }
        printf("\n");
        level++;
    }
    vector_free(&level_v);
}

int main(int argc, char *argv[]) {
    char *in_filename = "data.txt";
    int vertex = 1;
    switch (argc) {
    // Argument 2: vertex to start
    case 3:
        sscanf(argv[2], "%d", &vertex);
    case 2:
        in_filename = argv[1];
    }

    struct csg_graph graph = csg_ler(in_filename);
    if (vertex > graph.vert_s) {
        fprintf(stderr, "error: requested vertex %d, but graph only has %zu\n", vertex, graph.vert_s);
        exit(1);
    }
    print_depth_search(&graph, vertex);
    csg_free(&graph);
}

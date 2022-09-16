#include <stdio.h>
#include <string.h>

#include "graph/csgraph.h"
#include "list/vector.h"
#include "set/bitset.h"

void print_eulerian_cycle(struct csg_graph *graph, csg_vert vertex) {
    // Check if all vertices have even degrees
    for (csg_vert i = 1; i <= graph->vert_s; i++) {
        csg_edge degree = csg_grau(graph, i);
        // If not, the graph is not eulerian
        if (degree % 2) {
            printf("0\n");
            return;
        }
    }
    // If all edges have even degrees, we know for sure following a path will
    // end up in an eulerian cycle
    printf("1\n");

    // Store the first item in a stack
    struct vector stack_v = vector_create(sizeof(csg_vert));
    stack_v.size++;
    ((csg_vert*)stack_v.buf)[0] = vertex;

    char visited[BITNSLOTS(graph->edge_s)];
    memset(visited, 0, BITNSLOTS(graph->edge_s) * sizeof(char));

    size_t cycle_s = 0;
    csg_vert cycle_arr[graph->edge_s + 1];
    // We'll use the stack as a way to store what path we've followed to get to
    // a specific path
    while (stack_v.size > 0) {
        // Try to get an unvisited outgoing edge from the stack's head
        csg_vert v1 = ((csg_vert*)stack_v.buf)[stack_v.size-1];
        csg_edge edge_s;
        csg_edge edge_first = csg_graph_get_edges(graph, v1, &edge_s);
        csg_vert v2 = 0;
        for (int i = 0; i < edge_s; i++) {
            csg_edge edge_i = edge_first + i;
            if (!BITTEST(visited, edge_i)) {
                v2 = graph->edge_arr[edge_i];
                BITSET(visited, edge_i);
                BITSET(visited, csg_graph_edge(graph, v2, v1));
                break;
            }
        }
        // If we found an unvisited edge, this means there's a path to follow
        if (v2) {
            vector_append(&stack_v, v2);
        // Otherwise, we either reached the end of a path or we're unwinding
        // the stack, so we need to add the vertex to our result
        } else {
            cycle_arr[cycle_s] = v1;
            cycle_s++;
            stack_v.size--;
        }
    }

    printf("%d", cycle_arr[0]);
    for (int i = 1; i < cycle_s; i++) {
        printf(",%d", cycle_arr[i]);
    }
    printf("\n");

    vector_free(&stack_v);
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
        fprintf(stderr, "error: requested vertex %d, but graph only has %zu\n", vertex, graph.vert_s);
        exit(1);
    }
    print_eulerian_cycle(&graph, vertex);
    csg_free(&graph);
}

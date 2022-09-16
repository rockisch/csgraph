#include <stdio.h>

#include "graph/csgraph.h"

inline static void show_functions(struct csg_graph *graph) {
    printf("vertices: %d\narestas %d\n", csg_qtdVertices(graph), csg_qtdArestas(graph));
    printf("grau do primeiro node: %d\n", csg_grau(graph, 1));
    printf("rotulo do primeiro node: %s\n", csg_rotulo(graph, 1));

    csg_vert target_node = 2;
    csg_vert size;
    csg_vert *data = csg_vizinhos(graph, target_node, &size);

    printf("vizinhos do node %d:", target_node);
    for (int i = 0; i < size; i++) {
        printf(" %d", data[i]);
    }
    printf("\n");

    printf("ha aresta entre 1 e 2? %d\n", csg_haAresta(graph, 1, 2));
    printf("ha aresta entre 1 e 3? %d\n", csg_haAresta(graph, 1, 3));
    printf("peso de entre 1 e 2: %d\n", csg_peso(graph, 1, 2));
    printf("peso de entre 1 e 3: %d\n", csg_peso(graph, 1, 3));
}

int main() {
    char *filename = "data.txt";

    struct csg_graph graph = csg_ler(filename);
    show_functions(&graph);
    csg_free(&graph);
}

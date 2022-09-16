#ifndef CSGRAPH_H
#define CSGRAPH_H

#include <stdbool.h>
#include <stdint.h>

#define BUFSIZE 512

typedef uint32_t csg_vert;
typedef uint32_t csg_edge;

struct csg_graph {
    size_t edge_s;
    size_t vert_s;
    csg_vert *edge_arr;
    csg_edge *vert_arr;
    uint32_t *weight_arr;
    char **label_arr;
};

void csg_print(struct csg_graph *graph);
void csg_free(struct csg_graph *graph);
csg_edge csg_graph_get_edges(struct csg_graph *graph, csg_vert v, csg_vert *size);
int csg_graph_edge(struct csg_graph *graph, csg_vert v1, csg_vert v2);

csg_vert csg_qtdVertices(struct csg_graph *graph);
csg_edge csg_qtdArestas(struct csg_graph *graph);
csg_edge csg_grau(struct csg_graph *graph, csg_vert v);
char *csg_rotulo(struct csg_graph *graph, csg_vert v);
csg_vert *csg_vizinhos(struct csg_graph *graph, csg_vert v, csg_vert *size);
bool csg_haAresta(struct csg_graph *graph, csg_vert v1, csg_vert v2);
int csg_peso(struct csg_graph *graph, csg_vert v1, csg_vert v2);
struct csg_graph csg_ler(char *file);
#endif

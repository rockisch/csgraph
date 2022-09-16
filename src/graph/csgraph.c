#define BUFSIZE 512

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csgraph.h"
#include "../utils/errors.h"
#include "../list/vector.h"

csg_edge csg_graph_vertex_degree(struct csg_graph *graph, csg_vert v) {
    return graph->vert_arr[v + 1] - graph->vert_arr[v];
}

csg_vert *csg_graph_get_neighbours(struct csg_graph *graph, csg_vert v, csg_vert *size) {
    csg_edge degree = csg_graph_vertex_degree(graph, v);
    *size = degree;
    return graph->edge_arr + graph->vert_arr[v];
}

csg_edge csg_graph_get_edges(struct csg_graph *graph, csg_vert v, csg_vert *size) {
    csg_edge degree = csg_graph_vertex_degree(graph, v);
    *size = degree;
    return graph->vert_arr[v];
}

int csg_graph_edge(struct csg_graph *graph, csg_vert v1, csg_vert v2) {
    csg_edge e_idx = graph->vert_arr[v1];
    while (e_idx < graph->vert_arr[v1 + 1]) {
        if (graph->edge_arr[e_idx] == v2)
            return e_idx;
        e_idx++;
    }
    return -1;
}

inline static void parse_graph_verts(FILE *f, struct csg_graph *graph) {
    char buf[BUFSIZE];

    // *vertices <n>
    if (fscanf(f, "*vertices %zu\n", &graph->vert_s) != 1)
        fexit(f, "vertex header");

    graph->vert_arr = calloc(graph->vert_s + 2, sizeof(csg_edge));
    if (!graph->vert_arr)
        mexit();
    graph->label_arr = malloc(sizeof(char*) * (graph->vert_s + 1));
    if (!graph->label_arr)
        mexit();

    for (int v_idx = 1; v_idx <= graph->vert_s; v_idx++) {
        // <index> <label>
        if (fscanf(f, "%*d %s\n", buf) != 1)
            fexit(f, "vertex lines");
        int label_s = strlen(buf);
        char *label = malloc(sizeof(char) * (label_s + 1));
        if (!label)
            mexit();
        if (!memcpy(label, buf, label_s + 1))
            mexit();
        graph->label_arr[v_idx] = label;
    }
}

struct edge {
    int weight;
    csg_vert src;
    csg_vert dst;
};
void vector_append_edge(struct vector *v, struct edge e) {
    size_t size = v->size;
    vector_prepare(v, 1, sizeof(e));
    v->size++;
    struct edge *buf = (struct edge*)v->buf;
    buf[size] = e;
}

inline static void parse_graph_edges(FILE *f, struct csg_graph *graph) {
    char buf[BUFSIZE];

    // *edges
    if (!fgets(buf, BUFSIZE, f))
        fexit(f, "edge header");


    struct vector iedge_vec = vector_create(sizeof(struct edge));
    struct edge e;
    while (fscanf(f, "%d %d %d", &e.src, &e.dst, &e.weight) == 3) {
        graph->vert_arr[e.src+1]++;
        graph->vert_arr[e.dst+1]++;
        vector_append_edge(&iedge_vec, e);
    }
    graph->edge_s = iedge_vec.size * 2;

    for (int i = 1; i <= graph->vert_s + 1; i++) {
        graph->vert_arr[i] = graph->vert_arr[i] + graph->vert_arr[i - 1];
    }

    graph->edge_arr = calloc(graph->edge_s, sizeof(csg_vert));
    if (!graph->edge_arr)
        mexit();
    graph->weight_arr = malloc(graph->edge_s * sizeof(int));
    if (!graph->weight_arr)
        mexit();

    for (int i = 0; i < iedge_vec.size; i++) {
        struct edge e = ((struct edge*)(iedge_vec.buf))[i];
        csg_edge src_i = graph->vert_arr[e.src];
        while (graph->edge_arr[src_i] != 0)
            src_i++;
        csg_edge dst_i = graph->vert_arr[e.dst];
        while (graph->edge_arr[dst_i] != 0)
            dst_i++;
        graph->edge_arr[dst_i] = e.src;
        graph->edge_arr[src_i] = e.dst;
        graph->weight_arr[src_i] = e.weight;
        graph->weight_arr[dst_i] = e.weight;
    }

}

struct csg_graph parse_graph_file(char *file) {
    struct csg_graph graph = {0};

    FILE *f = fopen(file, "r");
    if (!f)
        fexit(f, "opening file");

    parse_graph_verts(f, &graph);
    parse_graph_edges(f, &graph);

    fclose(f);
    return graph;
}

void csg_print(struct csg_graph *graph) {
    for (csg_vert v_idx = 1; v_idx <= graph->vert_s; v_idx++) {
        csg_edge e_idx = graph->vert_arr[v_idx];
        char *label = graph->label_arr[v_idx];
        printf("VERTEX <ID=%d, name=%s>\n", v_idx, label);
        while (e_idx < graph->vert_arr[v_idx + 1]) {
            csg_vert dst = graph->edge_arr[e_idx];
            int weight = graph->weight_arr[e_idx];
            printf("EDGE <ID=%d, src=%d, dst=%d, weight=%d>\n", e_idx, v_idx, dst, weight);
            e_idx++;
        }
    }
}

void csg_free(struct csg_graph *graph) {
    free(graph->edge_arr);
    free(graph->vert_arr);
    free(graph->weight_arr);
    free(graph->label_arr);
}

csg_vert csg_qtdVertices(struct csg_graph *graph) {
    return graph->vert_s;
}

csg_edge csg_qtdArestas(struct csg_graph *graph) {
    return graph->edge_s / 2;
}

csg_edge csg_grau(struct csg_graph *graph, csg_vert v) {
    return csg_graph_vertex_degree(graph, v);
}

char *csg_rotulo(struct csg_graph *graph, csg_vert v) {
    return graph->label_arr[v];
}

csg_vert *csg_vizinhos(struct csg_graph *graph, csg_vert v, csg_vert *size) {
    return csg_graph_get_neighbours(graph, v, size);
}

bool csg_haAresta(struct csg_graph *graph, csg_vert v1, csg_vert v2) {
    int edge = csg_graph_edge(graph, v1, v2);
    return edge != -1;
}

int csg_peso(struct csg_graph *graph, csg_vert v1, csg_vert v2) {
    int edge = csg_graph_edge(graph, v1, v2);
    if (edge == -1)
        return INT_MAX;
    return graph->weight_arr[edge];
}

struct csg_graph csg_ler(char *file) {
    return parse_graph_file(file);
}

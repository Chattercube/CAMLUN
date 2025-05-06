#ifndef GRAPH_H
#define GRAPH_H

// ==== Includes ====

#include <stdbool.h>
#include <stddef.h>

#include "hashmap.h"
#include "linkedlist.h"
#include "typemethods.h"

// ==== End of Includes ====

// ==== Type definitions ====

typedef struct GraphVertexNode {
    void *id;  // Should be same as the key in the vertices hashmap
    void *value;

    LinkedList *out_gv_nodes;
    LinkedList *in_gv_nodes;

} GraphVertexNode;

typedef struct GraphEdgeKey {
    GraphVertexNode *from;  // GraphVertexNode *
    GraphVertexNode *to;    // GraphVertexNode *
} GraphEdgeKey;

typedef struct GraphConnection {
    void *from_id;
    void *from_value;
    void *to_id;
    void *to_value;
} GraphConnection;

typedef struct GraphEdge {
    void *from_id;
    void *to_id;
    void *edge_value;
} GraphEdge;

typedef struct Graph {
    type_methods *id_methods;
    type_methods *value_methods;

    HashMap *vertices;  // <TYPE_ID, TYPE_SHALLOW>
    HashMap *edges;     // <TYPE_GRAPH_EDGE_KEY, TYPE_VALUE>

    size_t vertex_count;
    size_t edge_count;

} Graph;

// ==== End of Type definitions ====

// ==== Method Overview ====

// Constructors and destructors :

Graph *graph_create(type_methods *id_methods, type_methods *value_methods);
void graph_destroy(Graph *this);
void graph_init_edges(Graph *this);
void graph_uninit_edges(Graph *this);

// Access and iterate :

void *graph_get_vertex_id(Graph *this, void *id);
void *graph_get_vertex_value(Graph *this, void *id);
void *graph_get_edge_value(Graph *this, void *from, void *to);

bool graph_contains(Graph *this, void *id);
bool graph_adjacent(Graph *this, void *from, void *to);

// GraphVertexNode *graph_find_node(Graph *this, void *id);

// HashMap *graph_vertices(Graph *this);
// HashMap *graph_edges(Graph *this);

// Size and capacity :

size_t graph_vertex_count(Graph *this);
size_t graph_edge_count(Graph *this);

size_t graph_in_degree(Graph *this, void *id);
size_t graph_out_degree(Graph *this, void *id);

// Modifiers :

void graph_add(Graph *this, void *id);
void graph_remove(Graph *this, void *id);
void graph_set(Graph *this, void *id, void *value);
void graph_reset(Graph *this, void *id);

void graph_connect(Graph *this, void *from, void *to);
void graph_disconnect(Graph *this, void *from, void *to);
void graph_assign(Graph *this, void *from, void *to, void *value);
void graph_unassign(Graph *this, void *from, void *to);

void graph_clear_vertices(Graph *this);
void graph_clear_edges(Graph *this);

// ==== End of Method Overview ====

// ==== Macros ====

#define GRAPH_OUT_ID_FOREACH(graph, node_id, idname, code)                             \
    do {                                                                               \
        GraphVertexNode *_current_node = hashmap_get((graph)->vertices, (node_id));    \
        LINKEDLIST_FOREACH(_current_node->out_gv_nodes, GraphVertexNode * _out_node, { \
            idname = _out_node->id;                                                    \
            code;                                                                      \
        });                                                                            \
    } while (0)

#define GRAPH_IN_ID_FOREACH(graph, node_id, idname, code)                            \
    do {                                                                             \
        GraphVertexNode *_current_node = hashmap_get((graph)->vertices, (node_id));  \
        LINKEDLIST_FOREACH(_current_node->in_gv_nodes, GraphVertexNode * _in_node, { \
            idname = _in_node->id;                                                   \
            code;                                                                    \
        });                                                                          \
    } while (0)

#define GRAPH_OUT_PAIRS_FOREACH(graph, node_id, idname, valname, code)                 \
    do {                                                                               \
        GraphVertexNode *_current_node = hashmap_get((graph)->vertices, (node_id));    \
        LINKEDLIST_FOREACH(_current_node->out_gv_nodes, GraphVertexNode * _out_node, { \
            idname = _out_node->id;                                                    \
            valname = _out_node->value;                                                \
            code;                                                                      \
        });                                                                            \
    } while (0)

#define GRAPH_IN_PAIRS_FOREACH(graph, node_id, idname, valname, code)                \
    do {                                                                             \
        GraphVertexNode *_current_node = hashmap_get((graph)->vertices, (node_id));  \
        LINKEDLIST_FOREACH(_current_node->in_gv_nodes, GraphVertexNode * _in_node, { \
            idname = _in_node->id;                                                   \
            valname = _in_node->value;                                               \
            code;                                                                    \
        });                                                                          \
    } while (0)

#define GRAPH_CONNECTIONS_FOREACH(graph, connectionname, code)                             \
    do {                                                                                   \
        HASHMAP_VALUES_FOREACH((graph)->vertices, GraphVertexNode * _from_vertex, {        \
            LINKEDLIST_FOREACH(_from_vertex->out_gv_nodes, GraphVertexNode * _to_vertex, { \
                connectionname = (GraphConnection){                                        \
                    .from_id = _from_vertex->id,                                           \
                    .from_value = _from_vertex->value,                                     \
                    .to_id = _to_vertex->id,                                               \
                    .to_value = _to_vertex->value};                                        \
                code;                                                                      \
            });                                                                            \
        });                                                                                \
    } while (0)

#define GRAPH_VERTICES_FOREACH(graph, idname, valname, code)                   \
    do {                                                                       \
        HASHMAP_VALUES_FOREACH((graph)->vertices, GraphVertexNode * _vertex, { \
            idname = _vertex->id;                                              \
            valname = _vertex->value;                                          \
            code;                                                              \
        });                                                                    \
    } while (0)

#define GRAPH_EDGES_FOREACH(graph, edgename, code)                               \
    do {                                                                         \
        if ((graph)->edges == NULL) {                                            \
            graph_init_edges(graph);                                             \
        }                                                                        \
        HASHMAP_PAIRS_FOREACH((graph)->edges, GraphEdgeKey * key, void *value, { \
            edgename = (GraphEdge){                                              \
                .from_id = key->from->id,                                        \
                .to_id = key->to->id,                                            \
                .edge_value = value};                                            \
            code;                                                                \
        });                                                                      \
    } while (0)

#endif
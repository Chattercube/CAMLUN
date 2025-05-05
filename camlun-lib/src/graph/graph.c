#include "graph.h"

#include "graphedgekey_type.h"
#include "hashmap.h"
#include "typemethods.h"
#include "linkedlist.h"
#include <stddef.h>
#include <assert.h>

static type_methods TYPE_GRAPHEDGEKEY = TYPE_METHODS(graphedgekey);

static GraphVertexNode *graphvertexnode_create(Graph *this) {
    GraphVertexNode *vertex_node = malloc(sizeof(GraphVertexNode));
    if (vertex_node == NULL) {
        return NULL;
    }
    vertex_node->id = NULL;
    vertex_node->value = USE_CRT(this->value_methods);
    vertex_node->out_gv_nodes = linkedlist_create();
    vertex_node->in_gv_nodes = linkedlist_create();
    return vertex_node;
}

static void graphvertexnode_destroy(Graph *this, GraphVertexNode *node) {
    if (this == NULL) {
        return;
    }
    USE_DEL(this->value_methods, node->value);
    linkedlist_destroy(node->out_gv_nodes);
    linkedlist_destroy(node->in_gv_nodes);
    free(node);
    return;
}

static void graphvertexnode_remove_out_node(GraphVertexNode *this, GraphVertexNode *node) {
    if (this == NULL || node == NULL) {
        return;
    }
    LinkedListNode *current = this->out_gv_nodes->head;
    while (current != NULL) {
        if (current->data == node) {
            linkedlist_delete(this->out_gv_nodes, current);
            break;
        }
        current = current->next;
    }
}

static void graphvertexnode_remove_in_node(GraphVertexNode *this, GraphVertexNode *node) {
    if (this == NULL || node == NULL) {
        return;
    }
    LinkedListNode *current = this->in_gv_nodes->head;
    while (current != NULL) {
        if (current->data == node) {
            linkedlist_delete(this->in_gv_nodes, current);
            break;
        }
        current = current->next;
    }
}

static void graph_edges_vertex_remove(Graph *this, GraphVertexNode *node) {

    LinkedList *out_nodes = node->out_gv_nodes;
    LinkedList *in_nodes = node->in_gv_nodes;

    LINKEDLIST_FOREACH(out_nodes, GraphVertexNode *out_node {
        GraphEdgeKey key = {.from = node, .to = out_node};
        hashmap_remove(this->edges, &key);
    });

    LINKEDLIST_FOREACH(in_nodes, GraphVertexNode *in_node {
        GraphEdgeKey key = {.from = in_node, .to = node};
        hashmap_remove(this->edges, &key);
    });
    return;
}

// Remove the vertex from any adjacency lists
static void graph_adjacency_lists_vertex_remove(Graph *this, GraphVertexNode *node) {

    LinkedList *out_nodes = node->out_gv_nodes;

    LINKEDLIST_FOREACH(out_nodes, GraphVertexNode *out_node {
        graphvertexnode_remove_in_node(out_node, node);
    });

    linkedlist_clear(node->out_gv_nodes);
    return;
    
}

Graph *graph_create(type_methods *id_methods, type_methods *value_methods) {
    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->id_methods = id_methods;
    graph->value_methods = value_methods;

    graph->vertices = hashmap_create(id_methods, NULL);
    if (graph->vertices == NULL) {
        free(graph);
        return NULL;
    }

    graph->edges = NULL;

    graph->vertex_count = 0;
    graph->edge_count = 0;

    return graph;
}

void graph_destroy(Graph *this) {
    if (this == NULL) {
        return;
    }

    HASHMAP_VALUES_FOREACH(this->vertices, GraphVertexNode *node, {
        graphvertexnode_destroy(this, node);
    });

    hashmap_destroy(this->vertices);
    this->vertices = NULL;

    if (this->edges != NULL) {
        hashmap_destroy(this->edges);
        this->edges = NULL;
    }

    free(this);
}

void graph_init_edges(Graph *this) {
    if (this->edges != NULL) {
        return;
    }
    this->edges = hashmap_create(&TYPE_GRAPHEDGEKEY, this->value_methods);
    HASHMAP_VALUES_FOREACH(this->vertices, GraphVertexNode * from_vertex, {
        LINKEDLIST_FOREACH(from_vertex->out_gv_nodes, GraphVertexNode *to_vertex, {
            GraphEdgeKey key;
            key.from = from_vertex;
            key.to = to_vertex;
            hashmap_add(this->edges, &key);
        });
    });
}

void *graph_get_vertex_value(Graph *this, void *id) {
    GraphVertexNode *vertex_node = hashmap_get(this->vertices, id);
    if (vertex_node == NULL) {
        return NULL;
    }
    return vertex_node->value;
}

void *graph_get_edge_value(Graph *this, void *from, void *to) {
    if(this->edges == NULL) {
        graph_init_edges(this);
    }

    GraphEdgeKey key = {.from = from, .to = to};
    return hashmap_get(this->edges, &key);
}

GraphVertexNode *graph_find_node(Graph *this, void *id) {
    GraphVertexNode *vertex_node = hashmap_get(this->vertices, id);
    return vertex_node;
}

size_t graph_vertex_count(Graph *this) {
    return this->vertex_count;
}

size_t graph_edge_count(Graph *this) {
    return this->edge_count;
}

size_t graph_in_degree(Graph *this, void *id) {
    GraphVertexNode *vertex_node = hashmap_get(this->vertices, id);
    if(!vertex_node) return 0;
    return vertex_node->in_gv_nodes->size;
}

size_t graph_out_degree(Graph *this, void *id) {
    GraphVertexNode *vertex_node = hashmap_get(this->vertices, id);
    if(!vertex_node) return 0;
    return vertex_node->out_gv_nodes->size;
}

bool graph_contains(Graph *this, void *id) {
    return hashmap_contains(this->vertices, id);
}

bool graph_adjacent(Graph *this, void *from, void *to) {
    if(this->edges == NULL) {
        GraphVertexNode *from_vertex = hashmap_get(this->vertices, from);
        LINKEDLIST_FOREACH(from_vertex->out_gv_nodes, GraphVertexNode *neighbour_node, {
            if (USE_CMP(this->id_methods, neighbour_node->id, to) == 0) {
                return true;
            }
        });
        return false;
    } else {
        return hashmap_contains(this->edges, &(GraphEdgeKey){.from = from, .to = to});
    }
}

void graph_add(Graph *this, void *id) {
    GraphVertexNode *vertex_node = graphvertexnode_create(this);
    if (vertex_node == NULL) {
        graphvertexnode_destroy(this, vertex_node);
        return;
    }
    hashmap_set(this->vertices, vertex_node->id, vertex_node);
    void *key_ref = hashmap_find(this->vertices, id)->key;
    vertex_node->id = key_ref; // vertex node has no ownership of id
    this->vertex_count++;
}

void graph_remove(Graph *this, void *id) {
    if(!hashmap_contains(this->vertices, id)) {
        return;
    }
    GraphVertexNode *vertex_node = hashmap_get(this->vertices, id);
    if(this->edges != NULL) {
        graph_edges_vertex_remove(this, vertex_node);
    }
    size_t removed_edges_numof = vertex_node->out_gv_nodes->size + vertex_node->in_gv_nodes->size;
    graph_adjacency_lists_vertex_remove(this, vertex_node);
    graphvertexnode_destroy(this, vertex_node);
    hashmap_remove(this->vertices, id);
    this->edge_count -= removed_edges_numof;
    this->vertex_count -= 1;
    return;

}

void graph_set(Graph *this, void *id, void *value) {
    if(!hashmap_contains(this->vertices, id)) {
        graph_add(this, id);
    }
    GraphVertexNode *vertex_node = hashmap_get(this->vertices, id);
    USE_DEL(this->value_methods, vertex_node->value);
    vertex_node->value = USE_DUP(this->value_methods, value);
    return;
}

void graph_reset(Graph *this, void *id) {
    if(!hashmap_contains(this->vertices, id)) {
        return;
    }
    GraphVertexNode *vertex_node = hashmap_get(this->vertices, id);
    USE_DEL(this->value_methods, vertex_node->value);
    vertex_node->value = USE_CRT(this->value_methods);
}

void graph_connect(Graph *this, void *from, void *to) {
    assert(hashmap_contains(this->vertices, from));
    assert(hashmap_contains(this->vertices, to));
    GraphVertexNode *from_vertex = hashmap_get(this->vertices, from);
    linkedlist_push_front(from_vertex->out_gv_nodes, to);

    GraphVertexNode *to_vertex = hashmap_get(this->vertices, to);
    linkedlist_push_front(to_vertex->in_gv_nodes, from_vertex);

    if(this->edges != NULL) {
        GraphEdgeKey key = {.from = from_vertex, .to = to_vertex};
        hashmap_add(this->edges, &key);
    }
    this->edge_count++;
    return;
}

void graph_disconnect(Graph *this, void *from, void *to) {
    assert(hashmap_contains(this->vertices, from));
    assert(hashmap_contains(this->vertices, to));
    GraphVertexNode *from_vertex = hashmap_get(this->vertices, from);
    GraphVertexNode *to_vertex = hashmap_get(this->vertices, to);
    if (this->edges != NULL) {
        GraphEdgeKey key = {.from = from_vertex, .to = to_vertex};
        hashmap_remove(this->edges, &key);
    }
    graphvertexnode_remove_in_node(to_vertex, from_vertex);
    graphvertexnode_remove_out_node(from_vertex, to_vertex);
    this->edge_count--;
    return;
}

void graph_assign(Graph *this, void *from, void *to, void *value) {
    assert(hashmap_contains(this->vertices, from));
    assert(hashmap_contains(this->vertices, to));
    if(this->edges == NULL) {
        graph_init_edges(this);
    }
    if(!graph_adjacent(this, from, to)) {
        graph_connect(this, from, to);
    }
    GraphVertexNode *from_vertex = hashmap_get(this->vertices, from);
    GraphVertexNode *to_vertex = hashmap_get(this->vertices, to);
    GraphEdgeKey key = {.from = from_vertex, .to = to_vertex};
    hashmap_set(this->edges, &key, value);
    return;
}

void graph_unassign(Graph *this, void *from, void *to) {
    assert(hashmap_contains(this->vertices, from));
    assert(hashmap_contains(this->vertices, to));
    if(this->edges == NULL) { return; }
    GraphVertexNode *from_vertex = hashmap_get(this->vertices, from);
    GraphVertexNode *to_vertex = hashmap_get(this->vertices, to);
    GraphEdgeKey key = {.from = from_vertex, .to = to_vertex};
    if(!hashmap_contains(this->edges, &key)) {
        return;
    }
    hashmap_reset(this->edges, &key);
}






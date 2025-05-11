#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <assert.h>

#include "graph.h"
#include "treeset.h"
#include "tuple.h"
#include "typemethods.h"
#include "vector.h"
#include "vector_ext.h"

typedef union NodeValue {
    struct {
        float x;
        float y;
    };
    double distance;
} NodeValue;

typedef double (*heuristic_function)(NodeValue *, NodeValue *);

DEFINE_DEFAULT_CONSTRUCTOR(node_value, NodeValue, {})
DEFINE_DESTRUCTOR(node_value, NodeValue, {})
DEFINE_COPY_CONSTRUCTOR(node_value, NodeValue, {})
DEFINE_COMPARATOR(node_value, NodeValue, {
    return VALUE_COMPARE(double, _first->distance, _second->distance);
})
DEFINE_HASH_FUNCTION(node_value, NodeValue, {})

TYPE_INIT(static type_methods TYPE_NODE_VALUE, node_value);
TYPE_INIT(static type_methods TYPE_STRING, string);
TYPE_INIT(static type_methods TYPE_DOUBLE, double);

TUPLE_INIT(static type_methods TYPE_PQNODE, double priority; char *id;, pqnode, {
    return VALUE_COMPARE(double, _first->priority, _second->priority);
})

static Graph *load_graph_from_file(FILE *file) {
    Graph *graph = graph_create(&TYPE_STRING, &TYPE_NODE_VALUE);

    while (1) {
        char cmd_str[16];
        if (fscanf(file, "%s", cmd_str) <= 0) {
            break;
        }
        if (strcmp(cmd_str, "add") == 0) {
            char id[256];
            float x, y;
            if (fscanf(file, "%s %f %f", id, &x, &y) != 3) {
                break;
            }
            #if DEBUG
            // printf("Adding vertex %s\n", id);
            #endif
            graph_set(graph, id, &(NodeValue){.x = x, .y = y});
        } else if (strcmp(cmd_str, "link") == 0) {
            char from_id[256];
            char to_id[256];
            if (fscanf(file, "%s %s", from_id, to_id) != 2) {
                break;
            }
            if (!graph_contains(graph, from_id) || !graph_contains(graph, to_id)) {
                fprintf(stderr, "Error: Vertex not found %s %s\n", from_id, to_id);
                continue;
            }
            graph_connect(graph, from_id, to_id);
        } else {
            break;
        }
    }
    return graph;
}

static Graph *load_graph_from_filename(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }
    Graph *graph = load_graph_from_file(file);
    fclose(file);
    return graph;
}

static double l2_distance(NodeValue *from, NodeValue *to) {
    return sqrt(pow(from->x - to->x, 2) + pow(from->y - to->y, 2));
}

static double l1_distance(NodeValue *from, NodeValue *to) {
    return fabs(from->x - to->x) + fabs(from->y - to->y);
}

static double diagonal_distance(NodeValue *from, NodeValue *to) {
    return fmax(fabs(from->x - to->x), fabs(from->y - to->y));
}

static void calculate_edge_values(Graph *graph) {
    graph_init_edges(graph);
    GraphConnection conn;
    GRAPH_CONNECTIONS_FOREACH(graph, conn, {
#if DEBUG
        //printf("Calculating distance from %s to %s\n", conn.from_id, conn.to_id);
#endif
        double distance = l2_distance(
            graph_get_vertex_value(graph, conn.from_id),
            graph_get_vertex_value(graph, conn.to_id));
        graph_assign(graph, conn.from_id, conn.to_id, &(NodeValue){.distance = distance});
    });
}

static Vector *find_path_by_astar(Graph *graph, char *from, char *to, heuristic_function heur_fn) {
    // Initalize Open Lis

    if(!graph_contains(graph, from)) {
        printf("Graph does not contain %s\n", from);
        return NULL;
    }

    if(!graph_contains(graph, to)) {
        printf("Graph does not contain %s\n", to);
        return NULL;
    }

    Vector *frontier = vector_create(&TYPE_PQNODE);
    HashMap *came_from = hashmap_create(&TYPE_STRING, &TYPE_STRING);
    HashMap *cost_so_far = hashmap_create(&TYPE_STRING, &TYPE_DOUBLE);

    heap_offer(frontier, &(pqnode){.priority = 0, .id = from}, false);
    hashmap_set(came_from, from, "");
    hashmap_set(cost_so_far, from, &(double){0});

    while (vector_size(frontier) > 0) {
        pqnode *current_pqnode = heap_poll(frontier, false);
        char *current_id = current_pqnode->id;
        pqnode_destructor(current_pqnode);

        if (strcmp(current_id, to) == 0) {
            break;
        }

        char *neighbour;
        GRAPH_OUT_ID_FOREACH(graph, current_id, neighbour, {
            assert(graph_contains(graph, neighbour));
            // printf("Checking %s -> %s\n", current_id, neighbour);
            double new_cost = *(double *)hashmap_get(cost_so_far, current_id) + *(double *)graph_get_edge_value(graph, current_id, neighbour);
            if( !hashmap_contains(cost_so_far, neighbour) || new_cost < *(double *)hashmap_get(cost_so_far, neighbour)) {
                hashmap_set(cost_so_far, neighbour, &(double){new_cost});
                double priority = new_cost + heur_fn(graph_get_vertex_value(graph, neighbour), graph_get_vertex_value(graph, to));
                heap_offer(frontier, &(pqnode){.priority = priority, .id = neighbour}, false);
                hashmap_set(came_from, neighbour, current_id);
            }
        });
    }

    // Reconstruct Path
    Vector *path = vector_create(&TYPE_STRING);
    char *current = to;
    while (current != NULL) {
        vector_push_back(path, current);
        if (strcmp(current, from) == 0) break;
        current = hashmap_get(came_from, current);
    }

    for (size_t i = 0, j = vector_size(path) - 1; i < j; ++i, --j) {
        vector_swap(path, i, j);
    }

    
    #if DEBUG
    // printf("New size %lu\n",  map->size);
    // HASHMAP_PRINTF(came_from, char *key, void *value, "%p", key); printf("\n");
    // HASHMAP_PRINTF(cost_so_far, char *key, void *value, "%p", key); printf("\n");
    #endif
    hashmap_destroy(cost_so_far);
    vector_destroy(frontier);
    hashmap_destroy(came_from);
    

    return path;
}

static double path_distance(Graph *graph, Vector *path) {
    double total_distance = 0;
    for (size_t i = 0; i < vector_size(path) - 1; ++i) {
        char *from = vector_get(path, i);
        char *to = vector_get(path, i + 1);
        total_distance += *(double *)graph_get_edge_value(graph, from, to);
    }
    return total_distance;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <graph_filename> <from_id> <to_id> <heuristic_function>\n", argv[0]);
        fprintf(stderr, "heuristic_function: l2 | l1 | diag\n");
        return 1;
    }

    char *graph_filename = argv[1];
    char *from_id = argv[2];
    char *to_id = argv[3];
    char *heuristic_name = argv[4];

    heuristic_function heur_fn = NULL;
    if (strcmp(heuristic_name, "l2") == 0) {
        heur_fn = l2_distance;
    } else if (strcmp(heuristic_name, "l1") == 0) {
        heur_fn = l1_distance;
    } else if (strcmp(heuristic_name, "diag") == 0) {
        heur_fn = diagonal_distance;
    } else {
        fprintf(stderr, "Unknown heuristic function: %s\n", heuristic_name);
        return 1;
    }

    Graph *graph = load_graph_from_filename(graph_filename);

    if (!graph) {
        fprintf(stderr, "Failed to load graph from file: %s\n", graph_filename);
        return 1;
    }
    calculate_edge_values(graph);

    clock_t start_time = clock();
    Vector *path = find_path_by_astar(graph, from_id, to_id, heur_fn);
    if(path == NULL){
        printf("Path not found\n");
        graph_destroy(graph);
        return 1;
    }
    //return;
    clock_t end_time = clock();

    if (!path || vector_size(path) == 0 || strcmp(vector_get(path, 0), from_id) != 0 || strcmp(vector_get(path, vector_size(path) - 1), to_id) != 0) {
        printf("No path found from %s to %s.\n", from_id, to_id);
        if (path) vector_destroy(path);
        graph_destroy(graph);
        return 1;
    }

    printf("Optimal Path : ");
    for (size_t i = 0; i < vector_size(path); ++i) {
        printf("%s", (char *)vector_get(path, i));
        if (i + 1 < vector_size(path)) printf(" -> ");
    }
    printf("\n");

    double total_length = path_distance(graph, path);
    printf("Path Length : %f\n", total_length);

    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time Taken : %f\n", time_taken);

    vector_destroy(path);
    graph_destroy(graph);
    return 0;
}
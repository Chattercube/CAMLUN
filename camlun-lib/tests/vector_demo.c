#include "typemethods.h"
#include "vector.h"
#include <stddef.h>
#include <assert.h>
#include "tuple.h"
#include <math.h>
#include "treeset.h"


TUPLE_INIT(static type_methods TYPE_POINT, double x; double y;, tuple_point, {
    // Sort by distances from the origin
    double dist1 = hypot(_first->x, _first->y);
    double dist2 = hypot(_second->x, _second->y);
    return (dist1 > dist2) - (dist1 < dist2);
});

int main(int argc, char **argv) {

    assert(sizeof(float) <= sizeof(void*));

    float numbers_to_be_inserted[] = {2.0, 2.5, 7.0, 23.92, 19.21, 8.8};

    Vector *my_vec = vector_create(NULL);

    for(size_t i = 0; i < 6; i++) {
        float my_num = numbers_to_be_inserted[i];
        vector_push_back(my_vec, RAW(my_num));
    }

    VECTOR_PRINTF(my_vec, void *num, "%.2f", LIT(float, num)); printf("\n");

    TreeSet *points_set = treeset_create(&TYPE_POINT);

    tuple_point points[] = {
        {3.0, 4.0},
        {1.0, 2.0},
        {5.0, 6.0},
        {7.0, 8.0}
    };

    for(size_t i = 0; i < 4; i++) {
        tuple_point point = points[i];
        treeset_add(points_set, &point);
    }

    TREESET_PRINTF(points_set, tuple_point *point, "(%lf, %lf)", point->x, point->y); printf("\n");
    tuple_point *minimum = treeset_minimum(points_set);
    tuple_point *maximum = treeset_maximum(points_set);
    printf("Closest from origin = (%lf, %lf), Furthest from origin = (%lf, %lf)\n", minimum->x, minimum->y, maximum->x, maximum->y);

    return 0;
}
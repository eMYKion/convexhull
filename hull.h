#ifndef HULL_H
#define HULL_H

#include "prim.h"
#include <stdio.h>

size_t miny(point_t *points, size_t n);

void part_angle(point_t *points, size_t lo, size_t hi, size_t piv, point_t lop);

void sort_angle_helper(point_t *points, size_t lo, size_t hi, point_t lop);

void sort_angle(point_t *points, size_t n, point_t lop);

#endif

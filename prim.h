/* Mayank Mali, May 2020
 */

#ifndef PRIM_H
#define PRIM_H

#include <math.h>

typedef struct point {
  double x;
  double y;
} point_t;

typedef struct seg {
  point_t start;
  point_t end;
  double slope;
  double yint;
} seg_t;

typedef seg_t ray_t;//ray interpretation of segment
typedef seg_t vec_t;//vector interpretation of segment

double cross(vec_t a, vec_t b);

int sideof(point_t a, ray_t v);

double vabs(vec_t a);

double dot(vec_t a, vec_t b);

double angle(ray_t a, ray_t b);

#endif

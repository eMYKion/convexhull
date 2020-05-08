/* Mayank Mali, May 2020
 * implementation of 2d primitives in prim.h
 * TODO:: comparisons of float near 0 only need
 * to be within some small error
 */


#include "prim.h"
#include <math.h>
#include <stdio.h>

/* returns the magnitude of the cross
 * product between vectors a and b
 */
double cross(vec_t a, vec_t b) {
  double a1 = a.end.x - a.start.x;
  double a2 = a.end.y - a.start.y;
  
  double b1 = b.end.x - b.start.x;
  double b2 = b.end.y - b.start.y;

  return a1*b2 - a2*b1;
}

/* returns +1 if point a is on the left
 * side of ray v, else -1.
 * returns 0 if point a is on ray v
 */
int sideof(point_t a, ray_t v) {
  vec_t u = v;
  u.end = a;

  double det = cross(v, u);

  if (det == 0.f) {//a on v
    return 0;
  } else if (det > 0.f) {//a left
    return 1;
  } else {//a right
    return -1;
  }
}

double dot(vec_t a, vec_t b) {
  double a_dx = a.end.x - a.start.x;
  double a_dy = a.end.y - a.start.y;

  double b_dx = b.end.x - b.start.x;
  double b_dy = b.end.y - b.start.y;

  return a_dx*b_dx + a_dy*b_dy;
}

/* gets the length of the 
 * vector
 */
double vabs(vec_t a) {
  return sqrt(dot(a, a));
}

/* gets the counterclockwise angle of the
 * angle from ray a to ray b
 * in radians
 */
double angle(ray_t a, ray_t b) {
  double arg = dot(a,b)/(vabs(a)*vabs(b));
  return acos(arg);
}


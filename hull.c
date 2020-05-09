/* Mayank Mali, May 2020
 */

#include "hull.h"
#include "prim.h"
#include <stdio.h>

/* returns the point in the array points
 * with the lowest y-coordinate
 * runs in O(n)
 *
 * requires points has n points
 * requires that n >= 1
 * ensures that return point has lowest y-coordinate
 */
size_t miny(point_t *points, size_t n) {
  double ly = points[0].y;
  size_t li = 0;

  for (size_t i = 1; i < n; i++) {
    if (points[i].y < ly) {
      ly = points[i].y;
      li = i;
    }
  }

  return li;
}

/* partition: O(n)
 * rearranges points[lo,...,hi) so that
 * forall x in points[lo,...,piv), 
 *  ray(lop,x) forms a larger angle with x-axis than
 *  ray(lop, points[piv]) does
 * forall y in points[piv+1,...,hi), 
 *  ray(lop,y) forms a lower angle with x-axis than
 *  ray(lop, points[piv]) does
 *
 * requires lo < piv < hi
 * requires that lop is not in points[lo,...,hi)
 */
void part_angle(point_t *points, size_t lo, size_t hi, size_t piv, point_t lop) {
  
  //unit vector in x-axis
  vec_t unit;
  unit.start = lop;
  unit.end.x = lop.x + 1.0f;
  unit.end.y = lop.y;
  unit.slope = 0;
  unit.yint = lop.y;

  vec_t lop_piv;
  lop_piv.start = lop;
  lop_piv.end = points[piv];

  double piv_angle = angle(unit, lop_piv);

  size_t i = lo;
  size_t j = hi-1;

  vec_t vi;
  vi.start = lop;

  /*loop invariant
   * forall x in points[lo,...,i]
   *  angle(vec(lop,x), unit) < piv_angle
   * forall y in points[j,...,hi)
   *  angle(vec(lop,x), unit) >= piv_angle
   */
  while (i < j) {
    vi.end = points[i];//bug fix
    double vi_angle = angle(vi, unit);
    //vi is in the wrong partition
    if (vi_angle >= piv_angle) {
      point_t tmp = points[j];
      points[j] = points[i];
      points[i] = tmp;
      j--;
    } else {//vi is in the right partition
      i++;
    }
  }
}

/* sorts points[lo,...,hi) by angle with respect to lowest 
 * point in points, lop
 * runs expected O(n log n) time where n = hi - lo
 * (quicksort)
 *
 * requires hi >= lo
 * requires lop is not in points[lo,...,hi)
 *
 * ensures that if x,y are points in points,
 * and vector (lop, x) has smaller angle w.r.t. x-axis
 * than (lop, y), then
 * x comes before y in points.
 *
 */
void sort_angle_helper(point_t *points, size_t lo, size_t hi, point_t lop) {

  //base case
  if (hi <= lo + 1) {
    return;
  }
  

  size_t mid = lo + (hi - lo)/2;

  //O(n)
  part_angle(points, lo, hi, mid, lop);

  sort_angle_helper(points, lo, mid, lop);
  sort_angle_helper(points, mid+1, hi, lop);
}

/* sorts points by angle with respect to lowest 
 * point in points, lop
 * in expected O(n log n) time
 *
 * requires lop is not in the points
 *
 * ensures that if x,y are points in points,
 * and vector (lop, x) has smaller angle w.r.t. x-axis
 * than (lop, y), then
 * x comes before y in points.
 */
void sort_angle(point_t *points, size_t n, point_t lop) {
  sort_angle_helper(points, 0, n, lop);
}

/* stores points in c_hull array so that
 * the points form a polygon that is 
 * convex hull of points in points.
 *
 * requires that c_hull has size at least n and n >= 3.
 *
 * Requires that points is an array of size n
 * that is sorted by increasing angle w.r.t.
 * to positive x-axis.
 *
 * requires that points[0] is the lowest point (by y-coordinate)
 * in points.
 *
 * ensures that *m is the number of points in the convex hull.
 */
void sorted_to_convex(point_t *points, size_t n, point_t *c_hull, size_t *m) {
  c_hull[0] = points[0];
  size_t j = 1;

  seg_t next;//the last segment to be in the convex hull
  next.start = points[0];
  next.end = points[1];
  
  for (size_t i = 2; i < n; i++) {
    point_t curr = points[i];

    //if curr on left side of next vector, include it in hull
    if (sideof(curr, next) >= 0 ) {
      c_hull[j] = next.end;
      j++;

      next.start = next.end;
      next.end = curr;
    } else {//if curr on right side of last vector, substitute
      next.end = curr;
    }
  }
  c_hull[j] = next.end;
  j++;

  *m = j;
}


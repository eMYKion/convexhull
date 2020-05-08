#include "hull.h"
#include <stdio.h>

/* returns the point in the array points
 * with the lowest y-coordinate
 * runs in O(n)
 *
 * requires points has n points
 * requires that n >= 1
 * ensures that return point has lowest y-coordinate
 */
point_t miny(point_t *points, size_t n) {
  double ly = points[0].y;
  size_t li = 0;

  for (size_t i = 1; i < n; i++) {
    if (points[i].y < ly) {
      ly = points[i].y;
      li = i;
    }
  }

  return points[li];
}


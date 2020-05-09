#include "prim.h"
#include "hull.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#ifdef DEBUG
#define dbg_assert(expr) assert(expr)
#else
#define dbg_assert(expr) (void)sizeof(expr);
#endif

typedef struct args{
  size_t num_points;
  point_t points[0];//unknown number of points at runtime
} args_t;

int getargs(args_t **args);
void printpoints(point_t *points, size_t n);

int main(int argc, char *argv[]) {
 
  args_t *args;
  
  //get arguments and store in args
  if (getargs(&args) < 0) {
    perror("getargs error\n");
    return -1;
  }

  printpoints(args->points, args->num_points);

  //get the index of the lowest point (by y-coordinate) O(n)
  size_t li = miny(args->points, args->num_points);
  point_t lowest = args->points[li];

  printf("lowest point = (%g, %g)\n", lowest.x, lowest.y);

  point_t tmp = args->points[0];
  args->points[0] = args->points[li];
  args->points[li] = tmp;

  //sort rest of the points by angle (exclude lowest)
  sort_angle(&(args->points[1]), args->num_points-1, args->points[0]);

  printf("sorted %zu points by angle:\n", args->num_points);
  printpoints(args->points, args->num_points);

  point_t *c_hull;
  if ((c_hull = malloc(args->num_points * sizeof(point_t))) == NULL) {
    perror("malloc error");
    free(args);
    return -1;
  }

  size_t num_hull;
  sorted_to_convex(args->points, args->num_points, c_hull, &num_hull);

  printf("convex hull (%zu points):\n", num_hull);
  printpoints(c_hull, num_hull);

  free(c_hull);
  free(args);

  return 0;
}

/* gets input set of points from stdin
 * and stores in a args_t struct.
 *
 * Ensures that *args points to the
 * malloced struct in which the input
 * will reside.
 *
 * Requires args must be the address of a valid args_t* pointer.
 * Requires that caller will free *args.
 *
 * format:
 * n
 * x1 y1
 * x2 y2
 * ...
 * xn yn
 *
 * On failure, returns -1. On success, returns 0.
 */
int getargs(args_t **args) {
  dbg_assert(args != NULL);

  size_t num_points;

  if (scanf("%zu", &num_points) != 1) {
    perror("scanf error\n");
    return -1;
  }

  if ((*args = malloc(sizeof(args_t) + num_points*sizeof(point_t))) == NULL) {
    perror("malloc error\n");
    return -1;
  }

  args_t *pargs = *args;

  pargs->num_points = num_points;

  //parse input for num points
  for (size_t i = 0; i < num_points; i++) {
    point_t tmp;
    if (scanf("%lf %lf", &(tmp.x), &(tmp.y)) != 2) {
      perror("scanf error\n");
      return -1;
    }

    //store point in our args
    (pargs->points)[i].x = tmp.x;
    (pargs->points)[i].y = tmp.y;
  }

  return 0;
}

void printpoints(point_t *points, size_t n) {
  dbg_assert(points != NULL);
  printf("n = %zu\n", n);
  point_t curr;
  for (size_t i = 0; i < n; i++) {
    curr = points[i];
    printf("%lf %lf\n", curr.x, curr.y);
  }

}


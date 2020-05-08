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
void printargs(args_t *args);

int main(int argc, char *argv[]) {
 
  args_t *args;
  
  //get arguments and store in args
  if (getargs(&args) < 0) {
    perror("getargs error\n");
    return -1;
  }

  printargs(args);

  //get the lowest point (by y-coordinate) O(n)
  point_t lowest = miny(args->points, args->num_points);

  printf("lowest point = (%g, %g)\n", lowest.x, lowest.y);

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

void printargs(args_t *args) {
  dbg_assert(args != NULL);
  printf("n = %zu\n", args->num_points);
  point_t *curr;
  for (size_t i = 0; i < args->num_points; i++) {
    curr = &(args->points[i]);
    printf("(%lf, %lf)\n", curr->x, curr->y);
  }

}


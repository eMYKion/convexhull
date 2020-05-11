#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>

#include "prim.h"
#include "hull.h"

#ifdef DEBUG
#define dbg_assert(expr) assert(expr)
#else
#define dbg_assert(expr) (void)sizeof(expr);
#endif

/*single linked-list for points*/
typedef struct pointll {
  point_t point;
  struct pointll *next;
} pointll_t;

void x_init(Display **dis, int *screen, Window *win, GC *gc);

void pointll_free(pointll_t *points);

int convex_hull(pointll_t *points, size_t num_points, point_t **pc_hull, size_t *m);

int main (int argc, char *argv[]) {

  printf("Click on the window to add points.\n");
  printf("Press \'c\' to generate convex hull.\n");
  printf("Press \'q\' to quit.\n");

  Display *dis;
  int screen;
  Window win;
  GC gc;

  x_init(&dis, &screen, &win, &gc);
  
  //colors
  Colormap screen_colormap;
  XColor red, brown, blue, yellow, green;

  screen_colormap = DefaultColormap(dis, DefaultScreen(dis));

  XAllocNamedColor(dis, screen_colormap, "red", &red, &red);
  XAllocNamedColor(dis, screen_colormap, "brown", &brown, &brown);
  XAllocNamedColor(dis, screen_colormap, "blue", &blue, &blue);
  XAllocNamedColor(dis, screen_colormap, "yellow", &yellow, &yellow);
  XAllocNamedColor(dis, screen_colormap, "green", &green, &green);

  XSetForeground(dis, gc, red.pixel);

  //event loop
  XEvent event;
  KeySym key;
  char text[255];

  pointll_t *in_points = NULL;
  pointll_t *curr_point = NULL;
  size_t num_points = 0;

  while (1) {
    XNextEvent(dis, &event);
    if (event.type == Expose && event.xexpose.count == 0) {
      //redraw
      //TODO
    }

    if (event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
      if (text[0] == 'q') {
        break;
      }
      if (text[0] == 'h') {
        
        printf("drawing convex hull...\n");

        point_t *c_hull;
        size_t num_hull_points;
        if (convex_hull(in_points, num_points, &c_hull, &num_hull_points) < 0) {
          perror("convex_hull failure\n");
          exit(1);
        }

        //display the hull in green
        XSetForeground(dis, gc, green.pixel);
        for (size_t i = 1; i < num_hull_points; i++) {
          point_t start = c_hull[i-1];
          point_t end = c_hull[i];
          
          XDrawLine(dis, win, gc, (int)(start.x), (int)(start.y), (int)(end.x), (int)(end.y));
        }
        point_t first = c_hull[0];
        point_t last = c_hull[num_hull_points-1];
        XDrawLine(dis, win, gc, (int)(first.x), (int)(first.y), (int)(last.x), (int)(last.y));

        free(c_hull);


      }
      printf("You pressed the %c key!\n", text[0]);
    }

    if (event.type == ButtonPress) {
      int x = event.xbutton.x;
      int y = event.xbutton.y;

      printf("added point (%i, %i)\n", x, y);

      //draw point
      XFillRectangle(dis, win, gc, x, y, 4, 4);

      //add to point input
      num_points++;
      if (in_points == NULL) {
        if ((in_points = malloc(sizeof(pointll_t))) == NULL) {
          perror("malloc error\n");
          exit(1);
        }

        in_points->point.x = (double)x;
        in_points->point.y = (double)y;
        in_points->next = NULL;

        curr_point = in_points;
      } else {
        pointll_t *tmp;
        if ((tmp = malloc(sizeof(pointll_t))) == NULL) {
          perror("malloc error\n");
          exit(1);
        }
        tmp->point.x = (double)x;
        tmp->point.y = (double)y;
        tmp->next = NULL;

        curr_point->next = tmp;
        curr_point = tmp;
      }
    }
  }

  //now create convex hull


  pointll_free(in_points);

  //close everything
  XFreeGC(dis, gc);
  XDestroyWindow(dis, win);
  XCloseDisplay(dis);


  return 0;
}

void x_init(Display **dis, int *screen, Window *win, GC *gc) {

  *dis = XOpenDisplay(NULL);
  *screen = DefaultScreen(*dis);

  unsigned long black, white;
  black = BlackPixel(*dis, *screen);
  white = WhitePixel(*dis, *screen);

  *win = XCreateSimpleWindow(*dis, DefaultRootWindow(*dis), 0,0, 200, 300, 5, white, black);

  XSetStandardProperties(*dis, *win, "My Window", "Hi!", None, NULL, 0, NULL);

  XSelectInput(*dis, *win, ExposureMask | ButtonPressMask | KeyPressMask);

  *gc = XCreateGC(*dis, *win, 0, 0);
  
  XSetBackground(*dis, *gc, white);
  XSetForeground(*dis, *gc, black);


  XClearWindow(*dis, *win);

  XMapRaised(*dis, *win);

  XFlush(*dis);
}

/* takes a linked list of points with n points
 * and finds convex hull
 * and stores in malloced *pc_hull with *m
 * points
 * also store number of hull points in *m
 * on success return 0
 * on failure return -1
 */
int convex_hull(pointll_t *pointsll, size_t num_points, point_t **pc_hull, size_t *m) {
  //get the index of the lowest point (by y-coordinate) O(n)
  
  //give points n point_t structs
  point_t *points;
  if ((points = malloc(num_points * sizeof(point_t))) == NULL) {
    perror("malloc error\n");
    return -1;
  }

  size_t idx = 0;
  for (pointll_t *p = pointsll; p; p = p->next) {
    points[idx] = p->point;//copy point_t struct
    idx++;
  }

  //index of lowest point
  size_t li = miny(points, num_points);
  point_t lowest = points[li];

#ifdef DEBUG
  printf("lowest point = (%g, %g)\n", lowest.x, lowest.y);
#endif

  //swap lowest point to the front of the array
  point_t tmp = points[0];
  points[0] = points[li];
  points[li] = tmp;

  //sort rest of the points by angle (exclude lowest)
  sort_angle(&points[1], num_points-1, points[0]);

#ifdef DEBUG
  printf("sorted %zu points by angle:\n", num_points);
  printpoints(points, num_points);
#endif

  
  if ((*pc_hull = malloc(num_points * sizeof(point_t))) == NULL) {
    perror("malloc error");
    return -1;
  }

  sorted_to_convex(points, num_points, *pc_hull, m);
#ifdef DEBUG
  printf("convex hull (%zu points):\n", *m);
#endif
  printf("%zu\n", *m);
  printpoints(*pc_hull, *m);

  free(points);

  return 0;

}

/* frees a linked list of points
 * requires points is non-null and all elems are malloced
 */
void pointll_free(pointll_t *points) {
  dbg_assert(points != NULL);

  pointll_t *prev = points;
  for (pointll_t *p = prev->next; p; p = p->next) {
    free(prev);
    prev = p;
  }
  free(prev);
}


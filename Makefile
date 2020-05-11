all:
	gcc main.c prim.c hull.c -Wall -lm
graph:
	gcc graph.c prim.c hull.c -Wall -lX11 -lm

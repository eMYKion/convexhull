# Convex Hull

## Usage:

```bash
$ make
$ ./a.out < tests/input.txt
```

The input text file must be in the following format:

```
n
x1 y1
x2 y2
...
xn yn
```

`n` must be a positive integer.
`n` must be followed by n lines of points,
each line is the x-coordinate followed by a space, followed
by the y-coordinate. The x,y coordinates must be floating-point
numbers parsable by `scanf`.

For example, `input.txt` could be:

```
3
1 2
3 3.5
-1.4 5.6
```

## Output

The output is in the following format:
```
m
x1 y1
x2 y2
...
xm ym
```

where `m` is the number of points in the convex hull
and the points are in counter-clockwise order.
`x1 y1` is the lowest (y-coordinate) of the points.

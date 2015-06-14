/*                     Registration Mark

This library supports an image registration mark list. It allows
packed RGB image arrays to be modified to display a list of
registration marks recodered during an image sequence.

(c) 2008 Scott & Linda Wills                         */

#define EXTENT 4
#define BLACK 0
#define YELLOW 0xffff00
#define MAGENTA 0xff00ff
#define POINTBLOCKSIZE 50

typedef struct Point {
   int                  X;
   int                  Y;
   struct Point         *Next;
} Point;

extern Point *Add_Mark(Point *MarkHistory, unsigned char *Frame, int X, int Y, unsigned int Width, unsigned int Height);


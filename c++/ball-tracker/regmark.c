/*                     Registration Mark

This library supports an image registration mark list. It allows
packed RGB image arrays to be modified to display a list of
registration marks recodered during an image sequence.

(c) 2008 Scott & Linda Wills                         */

#include <stdlib.h>
#include <stdio.h>
#include "regmark.h"

Point                   *FreePoints = NULL;

/*              Draw HV Line

This routine draws either a horizontal or vertical line in a frame
array. It is assumed that the line is already cropped. */

void Draw_HV_Line(unsigned char *Frame, unsigned int Width, int Xmin, int Ymin, int Xmax, int Ymax, int Color) {

   int                Index, Step = 0;

   if (Xmin == Xmax)
      Step = 3 * Width;
   else if (Ymax == Ymax)
      Step = 3;
   else
      fprintf(stderr, "HV lines must be either horizontal or vertical\n");
   if (Step != 0)
     for (Index = (Ymin * Width + Xmin) * 3; Index <= (Ymax * Width + Xmax) * 3; Index += Step) {
       Frame[Index] = (unsigned char) (Color >> 16) & 255;
       Frame[Index+1] = (unsigned char) (Color >> 8) & 255;
       Frame[Index+2] = (unsigned char) Color & 255;
      }
}

/*            Draw Mark

This routine draws a registration mark in a frame array. If the mark
falls partially or completly outside the frame, it is cropped. */

void Draw_Mark(unsigned char *Frame, int X, int Y, int Color, unsigned int Width, unsigned int Height) {

   int                  MinX = X - EXTENT, MinY = Y - EXTENT, MaxX = X + EXTENT, MaxY = Y + EXTENT;

   if (X >= 0 && X < Width && Y >= 0 && Y < Height) {
      if (MinX < 0)
         MinX = 0;
      if (MinY < 0)
         MinY = 0;
      if (MaxX >= Width)
         MaxX = Width - 1;
      if (MaxY >= Height)
         MaxY = Height - 1;
      if (Y > 1)
	Draw_HV_Line(Frame, Width, MinX, Y-1, MaxX, Y-1, Color);
      if (Y < Height - 1)
	Draw_HV_Line(Frame, Width, MinX, Y+1, MaxX, Y+1, Color);
      if (X > 1)
	Draw_HV_Line(Frame, Width, X-1, MinY, X-1, MaxY, Color);
      if (X < Width - 1)
	Draw_HV_Line(Frame, Width, X+1, MinY, X+1, MaxY, Color);
      Draw_HV_Line(Frame, Width, MinX, Y, MaxX, Y, BLACK);
      Draw_HV_Line(Frame, Width, X, MinY, X, MaxY, BLACK);
   }
}

/*            Make Point

This routine returns a new point object. If no free points are
available, it allocates a block from the heap and adds them to the
free list. */

Point *Make_Point(int X, int Y) {

   Point                *NewPoint;

   if (FreePoints == NULL) {
      FreePoints = (Point *) malloc(POINTBLOCKSIZE * sizeof(Point));
      if (FreePoints == NULL) {
         fprintf(stderr, "Unable to allocate points\n");
         exit (1);
      }
      for (NewPoint = FreePoints; NewPoint < &(FreePoints[POINTBLOCKSIZE - 1]); NewPoint++)
         NewPoint->Next = (NewPoint + 1);
      (FreePoints[POINTBLOCKSIZE - 1]).Next = NULL;
   }
   NewPoint = FreePoints;
   FreePoints = FreePoints->Next;
   NewPoint->X = X;
   NewPoint->Y = Y;
   NewPoint->Next = NULL;
   return (NewPoint);
}

/*             Add Mark

This routine adds a registration mark to the mark history. It also
draws all marks in the specified frame. All but the most recent mark
is drawn in magenta. The most recent mark is drawn in yellow. The
revised mark history is returned. */

Point *Add_Mark(Point *MarkHistory, unsigned char *Frame, int X, int Y, unsigned int Width, unsigned int Height) {

   Point                *Mark = MarkHistory;

   if (MarkHistory == NULL) {
      Draw_Mark(Frame, X, Y, YELLOW, Width, Height);
      return (Make_Point(X, Y));
   } else {
      while(Mark->Next != NULL)
         Mark = Mark->Next;
      Mark->Next = Make_Point(X, Y);
      Mark = MarkHistory;
      while(Mark->Next != NULL) {
         Draw_Mark(Frame, Mark->X, Mark->Y, MAGENTA, Width, Height);
         Mark = Mark->Next;
      }
      Draw_Mark(Frame, X, Y, YELLOW, Width, Height);
      return (MarkHistory);
   }
}

/*             Print Mark History

This routine prints the mark history. */

void Print_Mark_History(Point *Mark) {

   printf("   mark history: ");
   while(Mark != NULL) {
      printf("(%d,%d) ", Mark->X, Mark->Y);
      Mark = Mark->Next;
   }
   printf("\n");
}

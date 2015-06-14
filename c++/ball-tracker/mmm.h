/*                     Multi Modal Mean

This library implements a multimodal mean foreground/background
separator. It operates on packed RGB images.

(c) 2008 Scott & Linda Wills                         */

#include <stdlib.h>
#include <stdio.h>

#define TRUE            1
#define FALSE           0

#define DEBUG           0    /* set to 1 for debug, reset to 0 for speed */

typedef struct          Pixel {
   unsigned char        R, G, B;
} Pixel;

typedef struct          Cell {
   int                  R, G, B, Count;
   struct Cell          *Next;
}  Cell;

#define                 FREECELLSBLOCKSIZE 100

extern Cell             *FreeCells;


extern Cell **Create_Initial_BGM(unsigned char *InFrame, unsigned int Width, unsigned int Height);
extern unsigned char *Find_Foreground(Cell **BGM, unsigned char *Frame, unsigned int Width, \
				      unsigned int Height, unsigned int Epsilon, unsigned int Cth);
extern unsigned int Decimate_BGM(Cell **BGM, unsigned int Cth, unsigned int NumSets);
extern Pixel Rainbow_Lookup(unsigned int Index);

/*                     Multi Modal Mean

This library implements a multimodal mean foreground/background
separator. It operates on packed RGB images.

(c) 2008 Scott & Linda Wills                         */

#include "mmm.h"

Cell                    *FreeCells = NULL;

/*            Allocate Cell

This routine returns a new cell. If none are available, it allocates a
block from the heap and adds them to the free list. */

Cell *Allocate_Cell() {

   Cell                 *NewCell;

   if (FreeCells == NULL) {
      FreeCells = (Cell *) malloc(FREECELLSBLOCKSIZE * sizeof(Cell));
      if (FreeCells == NULL) {
         fprintf(stderr, "Unable to allocate cell\n");
         exit (1);
      }
      for (NewCell = FreeCells; NewCell < &(FreeCells[FREECELLSBLOCKSIZE - 1]); NewCell++)
 	 NewCell->Next = (NewCell + 1);
      (FreeCells[FREECELLSBLOCKSIZE - 1]).Next = NULL;
   }
   NewCell = FreeCells;
   FreeCells = FreeCells->Next;
   return (NewCell);
}

/*           Last Cell

This routine returns a pointer to the last cell in the set. */

Cell *Last_Cell(Cell *ThisCell) {

   while (ThisCell->Next != NULL)
      ThisCell = ThisCell->Next;
   return (ThisCell);
}

/*           Set Length

This routine returns the number of cells in the set. */

unsigned int Set_Length(Cell *ThisCell) {
   unsigned int         Length = 0;     

   while (ThisCell != NULL) {
      ThisCell = ThisCell->Next;
      Length += 1;
   }
   return (Length);
}

/*           Print Cell

This routine prints the contents of a cell including both ratiometric
and reduced values. */

void Print_Cell(Cell *ThisCell) {

   if (ThisCell->Count != 0)
      printf("%u: [R= %3d (%3d), %3d (%3d), %3d (%3d), %d, %u]\n",
             ThisCell,
	     ThisCell->R, ThisCell->R / ThisCell->Count,
	     ThisCell->G, ThisCell->G / ThisCell->Count,
	     ThisCell->B, ThisCell->B / ThisCell->Count,
	     ThisCell->Count, ThisCell->Next);
   else
      printf("%u: [R= %3d (%3d), %3d (%3d), %3d (%3d), %d, %u]\n",
	     ThisCell,
             ThisCell->R, 0, ThisCell->G, 0, ThisCell->B, 0,
	     ThisCell->Count, ThisCell->Next);
}

/*           Print Set

This routine prints a set composed of a cell list. */

void Print_Set(unsigned int Index, Cell *ThisCell) {

   printf("Cell %d:\n", Index);
   while (ThisCell != NULL) {
      printf("   ");
      Print_Cell(ThisCell);
      ThisCell = ThisCell->Next;
   }
}

/*           Print Free Cells

This debuging routine prints the free list. */

void Print_Free_Cells() {

   Cell                 *ThisCell;

   printf("Free Cells\n\n");
   for (ThisCell = FreeCells; ThisCell != NULL; ThisCell = ThisCell->Next)
      Print_Cell(ThisCell);
   printf("\n");
}

/*           Create Initial BGM

This routine creates the inital background model using a single frame
from the sequence. It also returns the image width and height. */

Cell **Create_Initial_BGM(unsigned char *InFrame, unsigned int Width, unsigned int Height) {

   Cell                 **BGM;
   int                  I;

   if (DEBUG)
      printf("   building %d entry BGM ...\n", Width * Height);
   BGM = (Cell **) malloc(Width * Height * sizeof(Cell *));
   if (InFrame == NULL) {
      fprintf(stderr, "Unable to allocate BGM memory\n");
      exit (1);
   }
   for (I = 0; I < Width * Height; I++) {
      BGM[I] = Allocate_Cell();
      BGM[I]->R = (unsigned int) InFrame[3*I];
      BGM[I]->G = (unsigned int) InFrame[3*I + 1];
      BGM[I]->B = (unsigned int) InFrame[3*I + 2];
      BGM[I]->Count = 1;
      BGM[I]->Next = NULL;
   }
   return (BGM);
}   

/*              Match

This routine compares an input RGB pixel to a cell value. If it is
within epsilon in all three color components, it matches and the cell
pointer is returned. If the assimilate flag is set, it is also
assimilated into the matched cell. if it does not match, NULL is
returned. */

Cell *Match(Pixel *P, Cell *ThisCell, unsigned char Epsilon, int Assimilate) {

   if (abs(P->R - ThisCell->R / ThisCell->Count) <= Epsilon &&
       abs(P->G - ThisCell->G / ThisCell->Count) <= Epsilon &&
       abs(P->B - ThisCell->B / ThisCell->Count) <= Epsilon) {
      if (Assimilate == TRUE) {
 	 ThisCell->R += P->R;
	 ThisCell->G += P->G;
	 ThisCell->B += P->B;
	 ThisCell->Count += 1;
      }
      return(ThisCell);
   }
   return(NULL);
}

/*             Match All

This routine compares the input RGB pixel value to each Cell in the
set. The pixel value is added to a cell when matched. When matched, a
pointer the matching cell is returned. Otherwise, a NULL pointer is
returned. */

Cell *Match_All(Pixel *P, Cell *Cells, unsigned char Epsilon) {

   Cell                *ThisCell, *Result;

   for (ThisCell = Cells; ThisCell != NULL; ThisCell = ThisCell->Next) {
      if (ThisCell->Count == 0)
	Print_Set(0, Cells);
      Result = Match(P, ThisCell, Epsilon, TRUE);
      if (Result != NULL)
 	 return (Result);
   }
   return (NULL);
}

/*              Decimate BGM

This routine decimates (divided by two) every cell value and count in
the background model. If the cell falls below the cell threshold, it
is removed from the set's cell list and added to the free cell
list. The number of removed cells is returned. Note that last cells
(where the most recent growing cell resides) are handled specially. */

unsigned int Decimate_BGM(Cell **BGM, unsigned int Cth, unsigned int NumSets) {

   unsigned int         I, Freed = 0;
   Cell                 *ThisCell, **TrailingNext;

   for (I = 0; I < NumSets; I++) {
      ThisCell = BGM[I];
      TrailingNext = &(BGM[I]);
      while (ThisCell != NULL) {
	 if (ThisCell->Next != NULL || ThisCell->Count > 3 * Cth) {  /* last cell not processed if too small */
	    ThisCell->R >>= 1;
	    ThisCell->G >>= 1;
	    ThisCell->B >>= 1;
            ThisCell->Count >>= 1;
	 }
         if (ThisCell->Count < Cth && ThisCell->Next != NULL) {  /* if count becomes too small */
	    *TrailingNext = ThisCell->Next;                      /* splice out invalid cell */
            ThisCell->Next = FreeCells;
            FreeCells = ThisCell;
            ThisCell = *TrailingNext;
            Freed += 1;
	 } else {
	    TrailingNext = &(ThisCell->Next);                    /* else move to next cell */
            ThisCell = ThisCell->Next;
         }
      }
   }
   return (Freed);
}

/*                       Find_Foreground

This routine processes an image frame, blacking out background
pixels. Foreground pixels are not modified. */

unsigned char *Find_Foreground(Cell **BGM, unsigned char *Frame, unsigned int Width, \
			       unsigned int Height, unsigned int Epsilon, unsigned int Cth) {

   Cell                 *Result, *LastCell, *NewCell;
   Pixel                *P;
   unsigned int         I;


   for (I = 0; I < Width * Height; I++) {
      P = (Pixel *) &(Frame[I * 3]);
      Result = Match_All(P, BGM[I], Epsilon);
      if (Result == NULL) {
	 LastCell = Last_Cell(BGM[I]);
	 if (LastCell->Count >= Cth) {           /* if previous last cell is big enough */
	    NewCell = Allocate_Cell();           /* then make and append a new cell */
	    NewCell->R = (unsigned int) P->R;
	    NewCell->G = (unsigned int) P->G;
	    NewCell->B = (unsigned int) P->B;
	    NewCell->Count = 1;
	    NewCell->Next = NULL;
	    LastCell->Next = NewCell;
	 } else {                                /* else replace previous last cell */
	    LastCell->R = (unsigned int) P->R;
	    LastCell->G = (unsigned int) P->G;
	    LastCell->B = (unsigned int) P->B;
	    LastCell->Count = 1;
         }
      } else if (Result->Count >= Cth) {
	 P->R = 0;
	 P->G = 0;
	 P->B = 0;
      }
   }
   return (Frame);
}

/*                     Rollers

This library supports image rollers; a fast horizontal linear density
scanner for locating large contiguous non-zero regions. A routine
"rolls" across an image measuring the number of non-blackened pixels
in a specified window size. A rainbow paint function provide
artificial colorization for windows up to twelve pixel long.

(c) 2008 Scott & Linda Wills                         */

#include <stdlib.h>
#include <stdio.h>
#include "mmm.h"
#include "rollers.h"


unsigned char Rainbow12[] = {   0,   0,   0,
			        127, 0, 255,
                                0,   0, 255,
                                0, 127, 255,
                                0, 255, 255,
                                0, 255, 127,
			        0, 255,   0,
                              127, 255,   0,
                              255, 127,   0,
                              255, 255,   0,
                              255,   0,   0,
			      255, 255, 255 };

/*               Rainbow Lookup

This routine looks up an RGB rainbow color in a twelve color
pallet. */

Pixel Rainbow_Lookup(unsigned int Index) {

   Pixel                P;

   P.R = Rainbow12[3*Index];
   P.G = Rainbow12[3*Index + 1];
   P.B = Rainbow12[3*Index + 2];
   return (P);
}

/*              Scan Image Density

This routine scans an input frame contaning salient regions with
blackened pixels elsewhere. It returns a preallocated map of
horizontal window fill levels (contigous salient pixels). */

void Scan_Image_Density(unsigned char *Frame, unsigned int Width, unsigned int Height, \
			unsigned char *WinMap, unsigned char WinSize) {

  unsigned int         Sum = 0, Window = 0, I, Edge;

   Edge = 1 << (int) (WinSize - 1);                   /* place one in left edge of window */
   for (I = 0; I < Width * Height; I++) {             /* for each pixel in frame */
      Sum -= Window & 1;                              /* remove outgoing pixel count */
      Window >>= 1;                                   /* shift window for new pixel */
      if (Frame[3*I] | Frame[3*I+1] | Frame[3*I+2] != 0) {  /* if pixel contains non-blackened value */
	 Sum += 1;                                    /* increment count */
	 Window |= Edge;                              /* and insert new edge pixel count */
      }
      if (I % Width >= (WinSize>>1))                    /* wait till fully into row */
	WinMap[I - (WinSize>>1)] = Sum;                /* write current sum to winmap */
      if (I % Width == 0)                             /* if new row */
	 Sum = Window = 0;                            /* clear sum and window */
   }
}

/*              Paint Frame

This routine uses a WinMap to paint a frame using a rainbow paint
function. */

void Paint_Frame(unsigned char *Frame, unsigned int Width, unsigned int Height, unsigned char *WinMap) {

   unsigned int         I;
   Pixel                P;

   for (I = 0; I < Width * Height; I++) {             /* for each pixel */
      P = Rainbow_Lookup(WinMap[I]);                  /* lookup winmap value */
      Frame[3*I] = P.R;                               /* write red component */
      Frame[3*I+1] = P.G;                             /* write green component */
      Frame[3*I+2] = P.B;                             /* write blue component */
   }
}

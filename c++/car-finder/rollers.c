/*               Rollers (Lite)

This library supports image rollers; fast linear and area density
scanner for locating large contiguous non-zero regions. A routine
"rolls" across an image measuring the number of non-blackened pixels
in a specified linear or area window size. The library also includes
blob detection. A paint function for artificial colorization and a
thresholding function are provided.


(c) 2008-2009 Scott & Linda Wills

Documentation:

This library provides support high efficiency linear and array density
functions, plus fast blob finders. It also supports frame rendering in
artificial color, grayscale, and binary. Below is a usage example.

Key Model Parameters:

Spatial Mask Resolution (WheelSize): This parameter sets the linear or
area mask size for density assessment. Small sizes yield finder detail
in density measures. Larger sizes blur fine details, favoring area
density.

Blob Threshold (Bth): This parameter sets the blob threshold (between
0 and mask size WheelSize^2). Small values expand blob bounds and
reduce voids. Larger values tend to sharpen blob-bloat.

Key Data Structures:

DensityMap:  An array  of  unsigned chars  holding  per pixel  density
values produced by linear and  area density scans. Should be allocated
by caller.

Blob Object: A blob object contains an area (Count), Bounding Box
(Xmin, Xmax, Ymin, Ymax), and ratiometric maintained center of mass
(Xsum/Count, Ysum/Count). Blobs also contain an internal forwarding
pointer.

Key Usage Functions:


Horizontal_Image_Density(): Compute horizontal linear non-blackened
pixel density. WheelSize is mask length.

Vertical_Image_Density(): Compute vertical linear non-blackened pixel
density. WheelSize is mask length.

Area_Image_Density(): Compute area non-blackened pixel density
WheelSize is window edge size.

Blob_List_Length(): Print length of Blob List.

Print_Blob(): Print Single blob object

Print_Blobs(): Print list of blobs.

Mark_Blob_CoM(): Mark center of mass in frame for blob list.

Mark_Blob_BB(): Mark bounding box in frame for blob list.

Free_Blobs(): Free list of blobs.

Blob_Finder(): Find blobs in density map. Bth is threshold.

Paint_Frame(): Colorize frame based on density map value.

Grayscale_Frame(): Grayscale frame based on density map.

Threshold_Frame(): Make frame binary at specified threshold.

Example:

   int		        Height, Width;
   int		        Bth = BTH, WSIZE = Wsize, I, NumBlobs = 0;
   unsigned char 	*Frame;
   unsigned char 	*DensityMap;
   Blob                 *Blobs;

   ...
   DensityMap = (unsigned char *) malloc(Width * Height * sizeof(unsigned char));
   ...
   if (DensityMap == NULL) {
   for (...) {
      ...
      Area_Image_Density(Frame, Width, Height, DensityMap, Wsize);
      Blobs = Blob_Finder(DensityMap, Width, Height, Bth);
      Paint_Frame(Frame, Width, Height, Wsize*Wsize, DensityMap);
      Mark_Blob_CoM(Blobs, Frame, Width);
      Mark_Blob_BB(Blobs, Frame, Width);
      Free_Blobs(Blobs);
   }

*/

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "rollers.h"

Pixel W2C16up[] = {{  0,   0,   0},
		   {  0,   0, 128},
		   {128,   0,   0},
		   {  0, 128,   0},
		   {128,   0, 128},
		   {128, 128,   0},
		   {  0, 128, 128},
		   {128, 128, 128},
		   {  0,   0, 255},
		   {192, 192, 192},
		   {255,   0, 255},
		   {  0, 255, 255},
		   {  0, 255,   0},
		   {255, 255,   0},
		   {255,   0,   0},
		   {255, 255, 255}};

Blob                    *FreeBlobs = NULL;            /* free blob list */

/*               Horizontal Image Density

This routine horizontally scans an input frame containing salient
regions with blackened pixels elsewhere. It returns a preallocated map
of horizontal window fill levels (contiguous salient pixels). */

void Horizontal_Image_Density(unsigned char *Frame, int Width, int Height, \
			      unsigned char *DensityMap, unsigned char WheelSize) {

   int                   Sum = 0, Wheel = 0, WheelOff, I, J, Edge;

   Edge = 1 << (int) (WheelSize - 1);                 /* initialize one in left edge of window */
   WheelOff = WheelSize >> 1;                         /* window offset = half window size */
   for (I = 0; I < Width * Height; I++) {             /* for each pixel in frame */
      Sum -= Wheel & 1;                               /* remove outgoing pixel count */
      Wheel >>= 1;                                    /* shift window for new pixel */
      if ((Frame[3*I] | Frame[3*I+1] | Frame[3*I+2]) != 0) {  /* if pixel contains non-blackened value */
	 Sum += 1;                                    /* increment count */
	 Wheel |= Edge;                               /* and insert new edge pixel count */
      }
      if (I % Width >= WheelOff)                      /* wait until fully into row */
	 DensityMap[I - WheelOff] = Sum;              /* write current sum to density map */
      if (I % Width == Width - 1) {                   /* if at end of row */
	 for (J = 1; J <= WheelOff; J++) {            /* write out end of row */
            Sum -= Wheel & 1;                         /* remove outgoing pixel count */
            Wheel >>= 1;                              /* shift window for new pixel */
	    DensityMap[I - WheelOff + J] = Sum;       /* write diminishing sum */
         }
	 Sum = Wheel = 0;                             /* clear sum and window at row's end */
      }
   }
}

/*               Vertical Image Density

This routine vertically scans an input frame containing salient
regions with blackened pixels elsewhere. It returns a preallocated map
of vertical window fill levels (contiguous salient pixels). */

void Vertical_Image_Density(unsigned char *Frame, int Width, int Height, \
			    unsigned char *DensityMap, unsigned char WheelSize) {

   int                   Sum = 0, Wheel = 0, WheelOff, X, Y, I, J, Edge;

   Edge = 1 << (int) (WheelSize - 1);                 /* initialize one in left edge of window */
   WheelOff = (WheelSize >> 1) * Width;               /* window offset = half window size x Width */
   for (X = 0; X < Width; X++) {                      /* for each column */
      for (Y = 0; Y < (Width * Height); Y += Width) { /* for each row */
	 I = X + Y;                                   /* compute index */
         Sum -= Wheel & 1;                            /* remove outgoing pixel count */
         Wheel >>= 1;                                 /* shift window for new pixel */
         if ((Frame[3*I] | Frame[3*I+1] | Frame[3*I+2]) != 0) {  /* if pixel contains non-blackened value */
	    Sum += 1;                                 /* increment count */
	    Wheel |= Edge;                            /* and insert new edge pixel count */
         }
         if (Y >= WheelOff)                           /* wait until fully into column */
	    DensityMap[I - WheelOff] = Sum;           /* write current sum to density map */
         if (Y == (Height - 1) * Width) {             /* if at end of column */
	    for (J = Width; J <= WheelOff; J += Width) {   /* write out end of column */
               Sum -= Wheel & 1;                      /* remove outgoing pixel count */
               Wheel >>= 1;                           /* shift window for new pixel */
	       DensityMap[I - WheelOff + J] = Sum;    /* write diminishing sum */
            }
	    Sum = Wheel = 0;                          /* clear sum and window at row's end */
         }
      }
   }
}

/*               Area Image Density

This routine area (two dimensionally) scans an input frame containing
salient regions with blackened pixels elsewhere. It returns a
preallocated map of window fill levels (contiguous salient pixels).

ToDo: reverse scan order to better exploit spacial locality in data
cache. */

void Area_Image_Density(unsigned char *Frame, int Width, int Height, \
			unsigned char *DensityMap, unsigned char WheelSize) {

   int                   Wheels[Height];
   unsigned char         Sums[Height];
   int                   Vwheel[WheelSize];
   int                   Vsum, Vptr, HalfWheel, WheelOff, X, Y, I, Edge;

   Edge = 1 << (int) (WheelSize - 1);                 /* initialize one in left edge of window */
   HalfWheel = WheelSize >> 1;                        /* half wheel size */
   WheelOff = HalfWheel * (Width + 1);                /* window offset = half window size x Width */
   for (Y = 0; Y < Height; Y++) {                     /* for all rows */
      Wheels[Y] = 0;                                  /* clear the wheels */
      Sums[Y] = 0;                                    /* clear the sums */
   }
   for (X = 0; X < Width + HalfWheel; X++) {          /* for each column plus write out rows */
      for (Vptr = 0; Vptr < WheelSize; Vptr++)        /* for all positions in vertical wheel */
         Vwheel[Vptr] = 0;                            /* clear vertical wheel */
      Vsum = Vptr = 0;                                /* clear vertical sum and ptr */
      for (Y = 0; Y < Height + HalfWheel; Y++) {      /* for each row */
	 I = X + Y * Width;                           /* compute index */
         Sums[Y] -= Wheels[Y] & 1;                    /* remove outgoing pixel count */
         Wheels[Y] >>= 1;                             /* shift window for new pixel */
         Vsum -= Vwheel[Vptr];                        /* removing outgoing vertical wheel count */
         if (Y < Height) {                            /* while in image column */
	    if (X < Width )                           /* while in image row */
               if (Frame[3*I] | Frame[3*I+1] | Frame[3*I+2]) { /* if pixel contains non-blackened value */
	          Sums[Y] += 1;                       /* increment count */
	          Wheels[Y] |= Edge;                  /* and insert new edge pixel count */
               }
            Vwheel[Vptr] = Sums[Y];                   /* insert row sum on vertical wheel */
            Vsum += Sums[Y];                          /* and add to vertical sum */
         }
         Vptr = (Vptr + 1) % WheelSize;               /* increment vertical ptr */
         if (X >= HalfWheel && Y >= HalfWheel)        /* wait until fully into row and column */
	    DensityMap[I - WheelOff] = Vsum;          /* write current vertical sum to density map */
      }
   }
}

/*               New Blob

This routine returns a new blob. If none are available, it allocates a
block from the heap and adds them to the free list. */

Blob *New_Blob() {

   Blob                 *NewBlob;

   if (FreeBlobs == NULL) {
      FreeBlobs = (Blob *) malloc(FREEBLOBSBLOCKSIZE * sizeof(Blob));
      if (FreeBlobs == NULL) {
         fprintf(stderr, "Unable to allocate blob\n");
         exit (1);
      }
      for (NewBlob = FreeBlobs; NewBlob < &(FreeBlobs[FREEBLOBSBLOCKSIZE - 1]); NewBlob++)
 	 NewBlob->Next = (NewBlob + 1);
      (FreeBlobs[FREEBLOBSBLOCKSIZE - 1]).Next = NULL;
   }
   NewBlob = FreeBlobs;
   FreeBlobs = FreeBlobs->Next;
   NewBlob->Xmin = NewBlob->Xmax = NewBlob->Ymin = NewBlob->Ymax = 0;
   NewBlob->Xsum = NewBlob->Ysum = NewBlob->Count = 0;
   NewBlob->Expire = -1;
   NewBlob->Next = NewBlob->FwdPtr = NULL;
   return (NewBlob);
}

/*               Blob List Length

This routine returns the length of a blob list. */

int Blob_List_Length(Blob *Blobs) {

   int                  L = 0;

   while(Blobs) {
      L += 1;
      Blobs = Blobs->Next;
   }
   return (L);
}

/*               Print Blob

This routine prints the contents of a blob. */

void Print_Blob(Blob *ThisBlob) {

   printf("   %u: [BB= (%3d,%3d)x(%3d,%3d) CoM= (%3d,%3d), area= %6d, exp= %3d, FP= %u, Next= %u]\n",
	  (int) ThisBlob, ThisBlob->Xmin, ThisBlob->Ymin, ThisBlob->Xmax, ThisBlob->Ymax,
	  ThisBlob->Xsum / ThisBlob->Count, ThisBlob->Ysum / ThisBlob->Count,
	  ThisBlob->Count, ThisBlob->Expire, (int) ThisBlob->FwdPtr, (int) ThisBlob->Next);
}

/*               Print Blobs

This recursive routine prints a blob list. */

void Print_Blobs(Blob *Blobs) {

   if (Blobs) {                                       /* if non-NULL list */
      Print_Blob(Blobs);                              /* print first blob */
      Print_Blobs(Blobs->Next);                       /* then print rest of list */
   }
}

/*               Mark Pixel

This routine marks a pixel with an RGB value. */

void Mark_Pixel(int X, int Y, unsigned char R, unsigned char G, unsigned char B, \
		unsigned char *Frame, int Width) {

   int                  I;

   I = 3 * (Y * Width + X);
   Frame[I]   = R;
   Frame[I+1] = G;
   Frame[I+2] = B;
}


/*               Draw Horizontal Line

This routine draws a horizontal line in a specified color */

void Draw_H_Line(int Xmin, int Xmax, int Y, unsigned char R, unsigned char G, unsigned char B, \
		unsigned char *Frame, int Width) {

  int                 X;

  for(X = Xmin; X <= Xmax; X++)
     Mark_Pixel(X, Y, R, G, B, Frame, Width);
}

/*               Draw Vertical Line

This routine draws a vertical line in a specified color */

void Draw_V_Line(int Ymin, int Ymax, int X, unsigned char R, unsigned char G, unsigned char B, \
		unsigned char *Frame, int Width) {

  int                 Y;

  for(Y = Ymin; Y <= Ymax; Y++)
     Mark_Pixel(X, Y, R, G, B, Frame, Width);
}

/*               Mark_Blob_CoM

This routine marks the center of mass for each blob on a frame. The
mark is a green plus. Forwarding pointers are marked as a red pixel. */

void Mark_Blob_CoM(Blob *Blobs, unsigned char *Frame, int Width) {

  int                  X, Y;

   while (Blobs) {
      X = Blobs->Xsum / Blobs->Count;
      Y = Blobs->Ysum / Blobs->Count;
      if (Blobs->FwdPtr)
	 Mark_Pixel(X, Y, 255, 0, 0, Frame, Width);
      else {
	 Mark_Pixel(X, Y,   0, 255, 0, Frame, Width);
	 Mark_Pixel(X-1, Y, 0, 255, 0, Frame, Width);
	 Mark_Pixel(X, Y-1, 0, 255, 0, Frame, Width);
	 Mark_Pixel(X+1, Y, 0, 255, 0, Frame, Width);
	 Mark_Pixel(X, Y+1, 0, 255, 0, Frame, Width);
      }
      Blobs = Blobs->Next;
   }
}

/*               Mark_Blob_BB

This routine marks the bounding box for each blob on a frame. The
bounding box is drawn in yellow. Forward pointer bounding boxes are
drawn in red. */

void Mark_Blob_BB(Blob *Blobs, unsigned char *Frame, int Width) {

   while (Blobs) {
      if (Blobs->FwdPtr) {
	 Draw_H_Line(Blobs->Xmin, Blobs->Xmax, Blobs->Ymin, 255, 0, 0, Frame, Width);
	 Draw_H_Line(Blobs->Xmin, Blobs->Xmax, Blobs->Ymax, 255, 0, 0, Frame, Width);
	 Draw_V_Line(Blobs->Ymin, Blobs->Ymax, Blobs->Xmin, 255, 0, 0, Frame, Width);
	 Draw_V_Line(Blobs->Ymin, Blobs->Ymax, Blobs->Xmax, 255, 0, 0, Frame, Width);
      } else {
	 Draw_H_Line(Blobs->Xmin, Blobs->Xmax, Blobs->Ymin, 255, 255, 0, Frame, Width);
	 Draw_H_Line(Blobs->Xmin, Blobs->Xmax, Blobs->Ymax, 255, 255, 0, Frame, Width);
	 Draw_V_Line(Blobs->Ymin, Blobs->Ymax, Blobs->Xmin, 255, 255, 0, Frame, Width);
	 Draw_V_Line(Blobs->Ymin, Blobs->Ymax, Blobs->Xmax, 255, 255, 0, Frame, Width);
      }
      Blobs = Blobs->Next;
   }
}

/*               Reduce Forwarding Pointer

This recursive routine reduces a forwarding pointer, returning a
pointer to the vectored blob object. */

Blob *Reduce_FP(Blob *ThisBlob) {

   if (ThisBlob->FwdPtr)                              /* if forwarding pointer */
      return (Reduce_FP(ThisBlob->FwdPtr));           /* then reduce to vectored blob */
   else 
      return (ThisBlob);                              /* else return ptr to blob */
}

/*              Merge Blobs

This routine merges one blob into another. The first blob should be
reduced (not a fwd ptr) before it is merged. The second blob assumes
the first blobs positions. The first blob is then converted into a
forwarding pointer with an expiration value (for reclaiming). */

void Merge_Blobs(Blob *Blob1, Blob *Blob2, int Expire) {

   if (Blob1 != Blob2) {                              /* if first blob is not second blob */
      if (Blob1->Xmin < Blob2->Xmin)                  /* if new minimum X */
         Blob2->Xmin = Blob1->Xmin;                   /* update */
      if (Blob1->Ymin < Blob2->Ymin)                  /* if new minimum X */
         Blob2->Ymin = Blob1->Ymin;                   /* update */
      if (Blob1->Xmax > Blob2->Xmax)                  /* if new maximum X */
         Blob2->Xmax = Blob1->Xmax;                   /* update */
      if (Blob1->Ymax > Blob2->Ymax)                  /* if new maximum X */
         Blob2->Ymax = Blob1->Ymax;                   /* update */
      Blob2->Xsum += Blob1->Xsum;                     /* add center of mass sum */
      Blob2->Ysum += Blob1->Ysum;                     /* add center of mass sum */
      Blob2->Count += Blob1->Count;                   /* add area count */
      Blob1->FwdPtr = Blob2;                          /* vector second blob */
      Blob1->Expire = Expire;                         /* set expiration date */
   }
}

/*              Add Position

This routine adds a position to a blob. */

void Add_Position(Blob *ThisBlob, int X, int Y) {

   if (ThisBlob->Count == 0) {                        /* if first position */
      ThisBlob->Xmin = ThisBlob->Xmax = X;            /* then initialize X BB */
      ThisBlob->Ymin = ThisBlob->Ymax = Y;            /* then initialize Y BB */
   } else {                                           /* otherwise check min/max */
      if (X < ThisBlob->Xmin)                         /* if new minimum X */
         ThisBlob->Xmin = X;                          /* update */
      if (Y < ThisBlob->Ymin)                         /* if new minimum Y */
         ThisBlob->Ymin = Y;                          /* update */
      if (X > ThisBlob->Xmax)                         /* if new maximum X */
         ThisBlob->Xmax = X;                          /* update */
      if (Y > ThisBlob->Ymax)                         /* if new maximum Y */
         ThisBlob->Ymax = Y;                          /* update */
   }
   ThisBlob->Xsum += X;                               /* add X to center of mass sum */
   ThisBlob->Ysum += Y;                               /* add Y to center of mass sum */
   ThisBlob->Count += 1;                              /* increment area count */
}

/*              Free Blobs

This routine reclaims a list of blobs, adding them to the free list */

void Free_Blobs(Blob *Blobs) {

   Blob                  *Head = Blobs;

   if (Blobs) {
      while (Blobs->Next)                             /* find last blob in list */
         Blobs = Blobs->Next;
      Blobs->Next = FreeBlobs;                        /* point to head of blob free list */
      FreeBlobs = Head;                               /* update free blob ptr */
   }
}

/*              Reap Blobs

This routine collects expired blob objects, adding them to the blob
free list. Blobs are collected when their expiration time matches the
current X value. This guarantees that all column blob fwd ptrs have
been reduced before the blob object is reclaimed. The active blob list
is returned. */

Blob *Reap_Blobs(Blob *Blobs, int Now) {

   Blob                  *Head = Blobs, **Trail = &Head;

   while (Blobs)
      if(Now == Blobs->Expire) {                      /* if expired blob object */
	 *Trail = Blobs->Next;                        /* splice it out of blob list */
         Blobs->Next = FreeBlobs;                     /* push on free blob list */
         FreeBlobs = Blobs;                           /* update free blob ptr */
         Blobs = *Trail;                              /* move to next blob in active blob list */
      } else {
	 Trail = &(Blobs->Next);                      /* advance trailing ptr */
	 Blobs = Blobs->Next;                         /* advance active blob list ptr */
      }
   return (Head);                                     /* return head of active blob list */
}

/*              Blob Finder

This routine identifies and returns blob of salient regions in an area
density map. Blob threshold Bth defines density threshold . A list of
blob objects is returned. */

Blob *Blob_Finder(unsigned char *DensityMap, int Width, int Height, int Bth) {

   Blob                  *Blobs = NULL, *RowBlob;
   Blob                  *ColBlobs[Width];
   int                   X, Y;

   for (X = 0; X < Width; X++)                        /* for all columns */
      ColBlobs[X] = NULL;                             /* clear column blobs */
   for (Y = 0; Y < Height; Y++) {                     /* for each row */
      RowBlob = NULL;                                 /* clear row blob */
      for (X = 0; X < Width; X++) {                   /* for each row offset */
         if (ColBlobs[X] && ColBlobs[X]->FwdPtr)      /* if column blob is a fwd ptr */
	    ColBlobs[X] = Reduce_FP(ColBlobs[X]);     /* then reduce to the vectored blob */
         if (DensityMap[X + Y * Width] >= Bth) {      /* if blob-worthy */
	    if (RowBlob && ColBlobs[X])               /* if two blobs touch */
	       Merge_Blobs(ColBlobs[X], RowBlob, Y+1); /* then merge column blob into rowblob */
            else if (ColBlobs[X])                     /* if only column blob exists */
	       RowBlob = ColBlobs[X];                 /* copy to row blob */
            else if (RowBlob == NULL) {               /* if no current blobs */
	       RowBlob = New_Blob();                  /* allocate a new blob */
               RowBlob->Next = Blobs;                 /* push new blob onto blob list */
               Blobs = RowBlob;                       /* update blob list */
            }
	    Add_Position(RowBlob, X, Y);              /* add position to current blob */
            ColBlobs[X] = RowBlob;                    /* update column blob */
	 } else                                       /* else leaving blob */
	    RowBlob = ColBlobs[X] = NULL;             /* clear current blob pointers */
      }
      Blobs = Reap_Blobs(Blobs, Y);                   /* reap expired blobs */
   }
   Blobs = Reap_Blobs(Blobs, Y+1);                    /* eliminate residual fwd ptrs from last row */
   return (Blobs);                                    /* return blob list */
}

/*              Paint Frame

This routine uses a DensityMap to paint a frame using a rainbow paint
function. */

void Paint_Frame(unsigned char *Frame, int Width, int Height, int MaxCount, unsigned char *DensityMap) {

   int                  I;
   Pixel                P;

   for (I = 0; I < Width * Height; I++) {             /* for each pixel */
      P = W2C16up[DensityMap[I] * 15 / MaxCount];     /* compute scaled color */
      Frame[3*I] = P.R;                               /* write red component */
      Frame[3*I+1] = P.G;                             /* write green component */
      Frame[3*I+2] = P.B;                             /* write blue component */
   }
}

/*              Grayscale Frame

This routine uses a DensityMap to produce a grayscale map of the image
values. The values are scaled to the max count value. */

void Grayscale_Frame(unsigned char *Frame, int Width, int Height, int MaxCount, unsigned char *DensityMap) {

   int                  I;

   for (I = 0; I < Width * Height; I++)               /* for each pixel */
      Frame[3*I] = Frame[3*I+1] = Frame[3*I+2] = DensityMap[I] * 255 / MaxCount;
}

/*              Threshold Frame

This routine uses a DensityMap to threshold a frame into a binary image. */

void Threshold_Frame(unsigned char *Frame, int Width, int Height, int Threshold, unsigned char *DensityMap) {

  int                  I;

   for (I = 0; I < Width * Height; I++) {             /* for each pixel */
      if (DensityMap[I] >= Threshold)                 /* if density above threshold */
	 Frame[3*I] = Frame[3*I+1] = Frame[3*I+2] = 255; /* pixel on */
      else
	 Frame[3*I] = Frame[3*I+1] = Frame[3*I+2] = 0;   /* pixel off */
   }
}

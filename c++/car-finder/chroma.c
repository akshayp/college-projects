/*                     Chroma Filter (Lite)

This library implements a color filter / detection function.
It operates on packed RGB images.

(c) 2004-2009 Scott & Linda Wills

Documentation:

This library provides basic chroma filtering on RGB pixels in an
image. This filtering is performed in an integer HSI
representation. Below is a usage example.

Key Model Parameters:

Selected Hue (Hue): This integer parameter defines the target color
(0 - 255).  This is spectral color, wheel that wraps around.  Three
regions of 85 integers divide the hue space. Reference center points
include red (42), green (127), and blue (212).

Hue Threshold (Hth): When selecting a hue, this parameter defines the
match threshold. The selected range is defined -Hth <= Hx-Hue <= Hth.

Minimum Intensity (Imin): For low illumination (average R,G,B), hue
becomes noisy. Hue matches are inhibited when illumination is below
Imin.

Minimum Saturation (Smin): For low variance of R, G, and B values (low
hue saturation), hue is poorly defined and noisy. Hue matches are
inhibited when saturation is below Smin.

Saturation (Sat): For full chroma filtering, this integer parameter
specified the target saturation (0-255). Saturation is defined by the
range of RGB values normalized to the maximum RGB value.

Hue Threshold (Hth): When selecting a chroma, this parameter defines
the saturation match threshold. The selected range is defined -Sth <=
Sx-Sat <= Sth.


Key Data Structures:

Frame: An array of unsigned chars equal to the number of image pixels
times three (RGB). Typically dynamically allocated in the heap once
the image size is known.

Pixel: A struct that is defined by three unsigned char integers
(0-255) representing red, green, and blue components of a color
pixel.

Key Usage Functions:

RGB2HSIin(): Replaces a RGB pixel (specified by a pixel pointer) with
its corresponding HSI representation (inframe).

RGB2HSIout(): Computes and returns HSI components for an RGB pixel
specified with a a pixel pointer. Vectored pixel is unchanged.

HueRGB(): Transforms a specified hue into a fully saturated, mid
intensity RGB value. Used to display hue maps.

Hue_Filter(): Uses Hue, Hth, Imin, and Smin to hue filter an
image. Non-matching hues are blackened (zeroed) while matching pixel
are unchanged.


   unsigned char 	*Frame;
   int                  Width, Height;
   int                  Hue=HUE, Hth=HTH, Smin=SMIN, Imin=IMIN;

   ...
   for (...) {
      Load_Image(Path, Frame);
      Hue_Filter(Frame, Width, Height, Hue, Hth, Smin, Imin);
      ...
   }
*/

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "chroma.h"

/*           RGB to HSI (in frame)

This routine replaces an RGB value to an HSI value. All fields are
limited to an eight bit unsigned integer. The resulting HSI values
replace the vectored RGB pixel (returning the value in frame). */

void RGB2HSIin(Pixel *P) {

   unsigned char               Max = P->R, Mid = P->G, Min = P->B, Delta;

   if (Mid > Max) {
      Mid ^= Max;
      Max ^= Mid;
      Mid ^= Max;
   }
   if (Min > Mid) {
      Min ^= Mid;
      Mid ^= Min;
      Min ^= Mid;
      if (Mid > Max) {
         Mid ^= Max;
         Max ^= Mid;
         Mid ^= Max;
      }
   }
   Delta = Max - Min;
   if (Max == 0)                                     // black pixel
      P->R = P->G = P->B = 0;
   else if (Delta == 0) {                            // gray pixel
      P->R = P->G = 0;
      P->B = (Max + Mid + Min) / 3;
   } else {
      if (P->R == Max)
         P->R = ((P->G - P->B) * 42 / Delta) + 42;   // range from 0 - 84
      else if (P->G == Max)
         P->R = ((P->B - P->R) * 42 / Delta) + 127;  // range from 85 - 169
      else
         P->R = ((P->R - P->G) * 42 / Delta) + 212;  // range from 170 - 254
      P->G = Delta * 255 / Max;
      P->B = (Max + Mid + Min) / 3;
   }
}

/*           RGB to HSI (out of frame)

This routine replaces an RGB value to an HSI value. All fields are
limited to an eight bit unsigned integer. The returned values are
through call by reference H,S, and I parameters. The in frame
pixel is unchanged. */

void RGB2HSIout(Pixel *P, unsigned char *H, unsigned char *S, unsigned char *I) {

   unsigned char               Max = P->R, Mid = P->G, Min = P->B, Delta;

   if (Mid > Max) {
      Mid ^= Max;
      Max ^= Mid;
      Mid ^= Max;
   }
   if (Min > Mid) {
      Min ^= Mid;
      Mid ^= Min;
      Min ^= Mid;
      if (Mid > Max) {
         Mid ^= Max;
         Max ^= Mid;
         Mid ^= Max;
      }
   }
   Delta = Max - Min;
   if (Max == 0)                                   // black pixel
      *H = *S = *I = 0;
   else if (Delta == 0) {                          // gray pixel
      *H = *S = 0;
      *I = (Max + Mid + Min) / 3;
   } else {
      if (P->R == Max)
         *H = ((P->G - P->B) * 42 / Delta) + 42;   // range from 0 - 84
      else if (P->G == Max)
         *H = ((P->B - P->R) * 42 / Delta) + 127;  // range from 85 - 169
      else
         *H = ((P->R - P->G) * 42 / Delta) + 212;  // range from 170 - 254
      *S = Delta * 255 / Max;
      *I = (Max + Mid + Min) / 3;
   }
}

/*           Hue RGB

This routine decodes a hue value (0 - 255) to an RGB value
that is written to a pixel pointer. */

void HueRGB(unsigned char Hue, Pixel *P) {

   if (Hue < 85) {
      P->R = 255;
      if (Hue <= 42) {
	  P->B = (42 - Hue) * 255 / 42;
          P->G = 0;
      } else {
	 P->G = (Hue - 42) * 255 / 42;
         P->B = 0;
      }
   } else if (Hue < 170) {
      P->G = 255;
      if (Hue <= 127) {
	 P->R = (127 - Hue) * 255 / 42;
         P->B = 0;
      } else {
	 P->B = (Hue - 127) * 255 / 42;
         P->R = 0;
      }
   } else {
      P->B = 255;
      if (Hue <= 212) {
	 P->G = (212 - Hue) * 255 / 42;
         P->R = 0;
      } else {
         P->R = (Hue - 212) * 255 / 42;
         P->G = 0;
      }
   }
}

/*               Hue Filter

This routine filters an image based on hue. A specified hue value and
match threshold is used to search RGB pixels in the image. Since low
illumination and low saturation often result in erroneous hue values,
limits on these parameters are specified. Pixels in the input frame
are blackened (zeroed) unless the hue matches the specified value
along with other parameters requirements. */

void Hue_Filter(unsigned char *Frame, int Width, int Height, \
		int Hue, int Hth, int Smin, int Imin) {

   int                    FrameSize = 3 * Width * Height * sizeof(unsigned char);
   unsigned char          H, S, I;
   Pixel                  *P;

   for (P = (Pixel *) Frame; P < (Pixel *) (Frame + FrameSize); P++) {
      RGB2HSIout(P, &H, &S, &I);
      if (!(H-Hue <= Hth  && H-Hue >= -Hth && S >= Smin && I >= Imin))
	 P->R = P->G = P->B = 0;
   }
}

/*               Chroma Filter

This routine filters an image based on chromaticity (hue &
saturation). Specified hue and saturation values and match thresholds
are used to search RGB pixels in the image. Since low illumination
often results in erroneous hue and saturation values, limits on these
parameters are specified. Pixels in the input frame are blackened
(zeroed) unless the hue and saturation matches the specified values
along with minimum illumination. */

void Chroma_Filter(unsigned char *Frame, int Width, int Height, \
		   int Hue, int Hth, int Sat, int Sth, int Imin) {

   int                    FrameSize = 3 * Width * Height * sizeof(unsigned char);
   unsigned char          H, S, I;
   Pixel                  *P;

   for (P = (Pixel *) Frame; P < (Pixel *) (Frame + FrameSize); P++) {
      RGB2HSIout(P, &H, &S, &I);
      if (!(H-Hue <= Hth  && H-Hue >= -Hth && S-Sat <= Sth  && S-Sat >= -Sth && I >= Imin))
	 P->R = P->G = P->B = 0;
   }
}

/*                     Chroma Filter (Lite)

This library implements a color filter / detection function.
It operates on packed RGB images.

(c) 2004-2009 Scott & Linda Wills                         */

extern void RGB2HSIin(Pixel *P);
extern void RGB2HSIout(Pixel *P, unsigned char *H, unsigned char *S, unsigned char *I);
extern void HueRGB(unsigned char Hue, Pixel *P);
extern void Hue_Filter(unsigned char *Frame, int Width, int Height, \
		       int Hue, int Hth, int Smin, int Imin);



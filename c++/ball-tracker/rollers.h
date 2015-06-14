/*                     Rollers

This library supports image rollers; a fast horizontal linear density
scanner for locating large contiguous non-zero regions. A routine
"rolls" across an image measuring the number of non-blackened pixels
in a specified window size. A rainbow paint function provide
artificial colorization for windows up to twelve pixel long.

(c) 2008 Scott & Linda Wills                         */

extern void Scan_Image_Density(unsigned char *Frame, unsigned int Width, unsigned int Height, \
			       unsigned char *WinMap, unsigned char WinSize);

extern void Paint_Frame(unsigned char *Frame, unsigned int Width, unsigned int Height, unsigned char *WinMap);

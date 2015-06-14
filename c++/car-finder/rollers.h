/*                     Rollers (Lite)

This library supports image rollers; a fast horizontal linear density
scanner for locating large contiguous non-zero regions. A routine
"rolls" across an image measuring the number of non-blackened pixels
in a specified window size. A rainbow paint function provide
artificial colorization for windows up to 16 pixel long.

(c) 2008-2009 Scott & Linda Wills                         */

typedef struct          Blob {
   int                  Xsum, Ysum, Xmin, Ymin, Xmax, Ymax, Count, Expire;
   struct Blob          *FwdPtr, *Next;
}  Blob;

#define                 FREEBLOBSBLOCKSIZE 20

extern Blob *FreeBlobs;

extern void Horizontal_Image_Density(unsigned char *Frame, int Width, int Height, \
				    unsigned char *DensityMap, unsigned char WinSize);
extern void Vertical_Image_Density(unsigned char *Frame, int Width, int Height, \
				   unsigned char *DensityMap, unsigned char WinSize);
extern void Area_Image_Density(unsigned char *Frame, int Width, int Height, \
			       unsigned char *DensityMap, unsigned char WheelSize);
extern Blob *New_Blob();
extern int Blob_List_Length(Blob *Blobs);
extern void Print_Blob(Blob *ThisBlob);
extern void Print_Blobs(Blob *Blobs);
extern void Mark_Pixel(int X, int Y, unsigned char R, unsigned char G, unsigned char B, \
		       unsigned char *Frame, int Width);
extern void Mark_Blob_CoM(Blob *Blobs, unsigned char *Frame, int Width);
extern void Mark_Blob_BB(Blob *Blobs, unsigned char *Frame, int Width);
extern Blob *Reduce_FP(Blob *ThisBlob);
extern void Merge_Blobs(Blob *Blob1, Blob *Blob2, int Expire);
extern void Add_Position(Blob *ThisBlob, int X, int Y);
extern void Free_Blobs(Blob *Blobs);
extern Blob *Reap_Blobs(Blob *Blobs, int Now);
extern Blob *Blob_Finder(unsigned char *DensityMap, int Width, int Height, int Bth);
extern void Paint_Frame(unsigned char *Frame, int Width, int Height, int MaxCount, unsigned char *DensityMap);
extern void Grayscale_Frame(unsigned char *Frame, int Width, int Height, int MaxCount, unsigned char *DensityMap);
extern void Threshold_Frame(unsigned char *Frame, int Width, int Height, int Threshold, unsigned char *DensityMap);

/*                  Vision Utilities

This library adds support functions for the Independent JPEG Group
image access library. It also supports sequence library access
functions.

     Linda & Scott Wills                       (c) 2008-2009
*/


typedef struct          Pixel {
   unsigned char        R, G, B;
} Pixel;


#define BASE_DIR        "./"
#define SEQ_DIR         "./seqs"
#define TRIAL_DIR       "./trials"
#define TRUE            1
#define FALSE           0
#define DEBUG           0    /* set to 1 for debug, reset to 0 for speed */
#define QUALITY        75    /* default image quality */

#define NUMTILEX        2    /* size of tiled image (X) */
#define NUMTILEY        2    /* size of tiled image (Y) */

extern void Read_Header(char *FileName, int *Width, int *Height);
extern void Load_Image(char *FileName, unsigned char *Array);
extern void Store_Image(unsigned char *Array, char *FileName, int Width, int Height);
extern void Copy_Image(unsigned char *InArray, unsigned char *OutArray, int Width, int Height);
extern void Place_Tile(unsigned char *InArray, int Width, int Height, unsigned char *OutArray, int Offset);
extern int InDir(char *Name, char *Dir);

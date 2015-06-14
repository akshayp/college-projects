#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jpeglib.h>

#define QUALITY     75             // default image quality

extern void ReadHeader(char *FileName, int *Width, int *Height);
extern void LoadImage(char *FileName, unsigned char *Array);
extern void StoreImage(unsigned char *Array, char *FileName, int Width, int Height);

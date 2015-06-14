/*                  Vision Utilities

This library adds support functions for the Independent JPEG Group
image access library. It also supports sequence library access
functions.

     Linda & Scott Wills                       (c) 2008-2009

Key Data Structures:

Frame: An array of unsigned integers equal to the number of image
pixels times three (RGB). Typically dynamically allocated in the heap
once the image size is known.

Key Functions:

Read_Header(): This function opens an image and returns its height and
width.

Load_Image(): This function loads and decodes an image into a
preallocated frame buffer.

Store_Image(): This function encodes and stores a frame into an output
image file.

Copy_Image(): This function copies an input image to a preallocated
output array.

*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <jpeglib.h>
#include "utils.h"

/* This routine returns an jpeg image header, returning its width and height. */

void Read_Header(char *FileName, int *Width, int *Height) {
   struct jpeg_decompress_struct       cinfo;
   struct jpeg_error_mgr               jerr;
   FILE                                *FP;

   cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_decompress(&cinfo);
   FP = fopen(FileName, "rb");
   if (FP == NULL) {
      fprintf(stderr, "ERROR: %s cannot be opened\n", FileName);
      exit(1);
   }
   jpeg_stdio_src(&cinfo, FP);
   jpeg_read_header(&cinfo, TRUE);
   jpeg_start_decompress(&cinfo);
   *Width = cinfo.output_width;
   *Height = cinfo.output_height;
   if (cinfo.out_color_components != 3) {
      fprintf(stderr, "ERROR: %d color components; wrong image type\n", cinfo.out_color_components);
      exit(1);
   }
   jpeg_destroy_decompress(&cinfo);
   fclose(FP);
}

/* This routine reads and decompresses a jpeg image into a image array. */

void Load_Image(char *FileName, unsigned char *Array) {
   struct jpeg_decompress_struct       cinfo;
   struct jpeg_error_mgr               jerr;
   FILE                                *FP;
   int                                 Width, Height, Row;
   JSAMPROW                            *RowPtr;

   cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_decompress(&cinfo);
   FP = fopen(FileName, "rb");
   if (FP == NULL) {
      fprintf(stderr, "ERROR: %s cannot be opened\n", FileName);
      exit(1);
   }
   jpeg_stdio_src(&cinfo, FP);
   jpeg_read_header(&cinfo, TRUE);
   cinfo.out_color_space = JCS_RGB;
   jpeg_start_decompress(&cinfo);
   Width = cinfo.output_width;
   Height = cinfo.output_height;
   for (Row = 0; Row < 3 * Height * Width; Row += 3 * Width) {
      RowPtr = (JSAMPROW *) &(Array[Row]);
      jpeg_read_scanlines(&cinfo, (JSAMPARRAY) &RowPtr, 1);
   }
   jpeg_finish_decompress(&cinfo);
   fclose(FP);
}

/* This routine writes an image array as a compressed jpeg image file. */

void Store_Image(unsigned char *Array, char *FileName, int Width, int Height) {
   struct jpeg_compress_struct		cinfo;
   struct jpeg_error_mgr		jerr;
   FILE					*FP;
   JSAMPROW 				RowPtr[1];
   int					RowStride;

   cinfo.err = jpeg_std_error(&jerr);
   jpeg_create_compress(&cinfo);
   if ((FP = fopen(FileName, "wb")) == NULL) {
      fprintf(stderr, "can't open %s\n", FileName);
      exit(1);
   }
   jpeg_stdio_dest(&cinfo, FP);
   cinfo.image_width = Width; 			/* image width and height, in pixels */
   cinfo.image_height = Height;
   cinfo.input_components = 3;			/* # of color components per pixel */
   cinfo.in_color_space = JCS_RGB; 		/* colorspace of input image */
   jpeg_set_defaults(&cinfo);
   jpeg_set_quality(&cinfo, QUALITY, TRUE);	/* limit to baseline-JPEG values */
   jpeg_start_compress(&cinfo, TRUE);
   RowStride = Width * 3;			/* JSAMPLEs per row in image_buffer */
   while (cinfo.next_scanline < cinfo.image_height) {
      RowPtr[0] = (JSAMPROW) &(Array[cinfo.next_scanline * RowStride]);
      (void) jpeg_write_scanlines(&cinfo, RowPtr, 1);
   }
   jpeg_finish_compress(&cinfo);
   fclose(FP);
   jpeg_destroy_compress(&cinfo);
}

/* This routine copies an image array to another preallocated array. */

void Copy_Image(unsigned char *InArray, unsigned char *OutArray, int Width, int Height) {

   int					I;

   for (I = 0; I < 3 * Width * Height; I++)
      OutArray[I] = InArray[I];                /* copy image data */
}

/* This routine places a copy of an input image into a larger tiled
image. The input image, its size, the tiled image array, and the
offset into the tiled image are specified. */

void Place_Tile(unsigned char *InArray, int Width, int Height, unsigned char *OutArray, int Offset) {

   int                                  Tx = (Offset % NUMTILEX) * Width;
   int                                  Ty = (Offset / NUMTILEX) * Height;
   int                                  X, Y, I, J;

   for (Y = 0; Y < Height; Y++)
      for (X = 0; X < Width; X++) {
	 I = (Y * Width + X) * 3;
         J = ((Ty + Y) * NUMTILEX * Width + Tx + X) * 3;
         OutArray[J] = InArray[I];             /* copy image data */
         OutArray[J+1] = InArray[I+1];
         OutArray[J+2] = InArray[I+2];
      }
}

/* This routine checks whether a file/subdirectory is in a specified directory/ */

int InDir(char *Name, char *Dir) {

   struct dirent                        **NameList;
   int                                  N, Match = FALSE;

   N = scandir(Dir, &NameList, 0, alphasort);
   if (N < 0)
      perror("scandir");
   else {
      while(N--) {
         if (strcmp(NameList[N]->d_name, Name) == 0)
	    Match = TRUE;
         free(NameList[N]);
      }
      free(NameList);
   }
   return (Match);
}

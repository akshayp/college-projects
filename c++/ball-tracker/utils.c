/*                  JPEG Access Utilities

This file contains utility routines to access jpeg images through the
JPEG library.

     Scott Wills                             31 October 2008
*/

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

/* This routine writes a image array as a comressed jpeg image file. */

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

/*                  Shell Image Sequence Processing Program

This rpogram contains a shell image sequence processing program that
	uses the JPEG users Group routines (through utility functions in
	util.c) and a multi modal mean foreground extractor.
*/

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "mmm.h"
#include "regmark.h"
#include "rollers.h"

#define EPSILON      35		/* maximum component difference threshold for match */
#define CTH           4		/* minimum observance for BG absorbance */
#define WINSIZE		  9		/* must be 1 - 32 in this implementation */

// Declaring Function Prototypes
Pixel Pixel_Lookup(unsigned char *, int, int, int, int);
int	  Pixel_Tester(Pixel);

//Main
int main(int argc, char *argv[]) {
	char                Path[128];
	unsigned int		Height, Width, Start, End, Epsilon = EPSILON, Cth = CTH, N;
	unsigned char 		*Frame, *WinMap;
	Cell                **BGM;
	Pixel               P,Ref_Pixel,Top, Side1, Side2;	// Pixels declarations for Center,Ref Pixel,top,either side of the reference pixel
	Point 				*MarkHistory = NULL;			// Marking list
	int 				Row, Col;						// Counter Variables
	int					Jump = 0;						// Flag used to set when to break;
	FILE				*Outfile;						// File Pointer

//File Input Check Statments
	if(argc !=4) {
		fprintf(stderr, "usage: %s seqdir start end\n", argv[0]);
		exit(1);
	}
// Create Output File Required for Grading
	Outfile = fopen("results.txt", "w"); // Change flag from w to a to obtain a complete results file for all test cases

//Input Parameter Check Statements

	if(sscanf(argv[2], "%d", &Start) != 1 || sscanf(argv[3], "%d", &End) != 1) {
		fprintf(stderr, "[%s:%s] are invalid start/end numbers\n", argv[2], argv[3]);
		exit(1);
	}
/*
Debug Statements controlled by flags set in mmm.h
*/
	if(DEBUG)
		printf("trial: Start= %d, End= %d, Epsilon= %d, Cth= %d ...\n", Start, End, Epsilon, Cth);

	sprintf(Path, "%s/%05d.jpg", argv[1], Start);
	if(DEBUG)
		printf("reading header of %s ...\n", Path);

	Read_Header(Path, &Width, &Height);
	if(DEBUG)
		printf("image size (%ux%u)\n", Width, Height);
//Allocate Memory for Frame
	Frame = (unsigned char *) malloc(3 * Width * Height * sizeof(unsigned char));
	if(Frame == NULL) {
		fprintf(stderr, "Unable to allocate image frame memory\n");
		exit (1);
	}
// Allocate Memory for Winmap
	WinMap = (unsigned char *) malloc(Width * Height * sizeof(unsigned char));
	if(Frame == NULL) {
		fprintf(stderr, "Unable to allocate image window map memory\n");
		exit (1);
	}
//Debug Statements
	if(DEBUG)
		printf("loading %s ...\n", Path);
//Create BGM for Image Processing
	Load_Image(Path, Frame);
	BGM = Create_Initial_BGM(Frame, Width, Height);

	// Store Frame 1 Data in Output File
	// -1 set as co-ordinates since the values are not used for grading.
	fprintf(Outfile, "%5d, %5d, %5d\n", 1, -1, -1); 

//Loop through frames given as user input

	for(N = Start + 1; N < End + 1; N++) 
	{
		Jump = 0; // Set Jump Flag which is used as break point to zero

		sprintf(Path, "%s/%05d.jpg", argv[1], N);
		if (DEBUG)
			printf("processing %s ...\n", Path);
//Load Image , Detect Forefround and Scan the image for Density Changes

		Load_Image(Path, Frame);
		Find_Foreground(BGM, Frame, Width, Height, Epsilon, Cth);
		Scan_Image_Density(Frame, Width, Height, WinMap, WINSIZE);
		Paint_Frame(Frame, Width, Height, WinMap);

//	Check From the center of the image	
// Find the center Pixel of the Frame, if it is yellow return -1 as this is not graded

		P = Pixel_Lookup(Frame, Width/2, Height/2, Width/2, Height/2);
		// Print Data For Frames where the ball is not found or
		if(Pixel_Tester(P))
		{
			fprintf(Outfile, "%5d, %5d, %5d\n", N, -1, -1);
			continue;
		}

// Go from Center to top and left to right of the image

		for(Col = (Height/2); Col >=0 ; Col--)	//Parses the Height of the Picture
		{
			for(Row =0; Row <= Width; Row++) 	// Parses the Width of the picture from left to right
			{
				Ref_Pixel = Pixel_Lookup(Frame, Row, Col, Height/2, Width); // Set Ref Pixel as temp Pixel from Parsing the Image
				
				// If the determined pixel is yellow check pixels around it
				
				if(Pixel_Tester(Ref_Pixel))
				{
					// Load pixels offset by the radius of 10 assuming a worst case diameter of 20 for the ball.
					Top = Pixel_Lookup(Frame, Row, Col - 10, Height/2, Width); 
					// Check for yellow on both sides of the Reference Pixel
					Side1 = Pixel_Lookup(Frame, Row - 10, Col - 10, Height/2, Width);
					Side2 = Pixel_Lookup(Frame, Row + 10, Col - 10, Height/2, Width);
					
					//Compare color information of the nearby Pixels to see if they are yellow
					if(Pixel_Tester(Top) || Pixel_Tester(Side1) || Pixel_Tester(Side2))
					{
						
						//MarkHistory = Add_Mark(MarkHistory, Frame, Row + 5, Col - 15, Width, Height); // Regmark commented to improve performace
						// Write coordnates to file
						fprintf(Outfile, "%5d, %5d, %5d\n", N, Row + 5 , Col - 10); // Offset the computed pixels so that the ball is towards the center
						
						
						//printf("%5d, %5d, %5d\n", N, Row + 5, Col - 20); //Debug Statement
						Jump = 1; // Set Jump Flag to break out of image loop.
						break;
					}
				}
			}
			if(Jump)
				break;// Break out of loop.
		}

		// Image information for frame where the ball was not found.
		if(!Jump)
		{
			fprintf(Outfile, "%5d, %5d, %5d\n", N, -1, -1); // Store cordinates as -1 since this part is either an error or not graded
			//printf("%5d, %5d, %5d\n", N, -1, -1); //Debug Statement
		}
//Sequence to store Output Image set to debug mode		

		if(DEBUG)
			printf("storing output sequence %s ...\n", Path);		
			sprintf(Path, "%sout/%05d.jpg", argv[1], N);
			//Store_Image(Frame, Path, Width, Height); // Store sequence commented out to improve performance
	}
	fclose(Outfile);// Close the result.txt file
	free(Frame);
	exit(0);
}

// Helper Functions used in the Program to Identify and Process Pixels

// This function looks up a specific pixel in the Frame and stores the RGB information in the Pixel struct
Pixel Pixel_Lookup(unsigned char *Frame, int X, int Y, int Height, int Width)
{
	Pixel P;
	int Index;
	Index = 3 * (Y * Width + X);
	P.R = Frame[Index];
	P.G = Frame[Index + 1];
	P.B = Frame[Index + 2];	
	return P;
}

// This Function checks if the given Pixel is Yellow 
int Pixel_Tester(Pixel P)
{

	if(P.R == 255 && P.G == 255 && P.B == 0)
	{
		return 1;
	}
	else
		return 0;
}


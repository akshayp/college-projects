/*					   Chroma Collision Avoidance

This program implements real-time color-base collision avoidance
monitor.

Algorithm by Akshay Patel gth708n

(c) 2008-2009 Scott & Linda Wills								*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "utils.h"
#include "report.h"
#include "chroma.h"
#include "rollers.h"
#include <math.h>

typedef struct          myBlob {
   int                  Xcom, Ycom, Xmin, Ymin, Xmax, Ymax, Count;
}  myBlob;

/* This program reads a specified sequence and runs the chroma
   collision avoidance monitor. */

int main(int argc, char *argv[]) {

   	char				Path[128], *SeqName;
	int					Height, Width, Start, End, MSpF, FrameSize, N, I;
   	FILE				*Report;
   	unsigned char		*Frame,*Hfrm, *Sfrm, *Ifrm,*DensityMap;
	//int					Hue=32, Hth=18, Smin=100,Sat = 100, Sth=20, Imin=50;  //For Hue Filter
	int					Wsize = 8,Bth = 2,Threshold = 15,Yth = 20, Xth = 240, Cth = 300, Xmth = 400, length,skipBlobs;	
	Blob 				*Blobs;
	myBlob 				tmpBlob;
	
	/*Implicit Function Declartions. Ideal use in its own header file*/
	void 				Clean_Frame(unsigned char *Frame, int Height, int Width);
	void 				Chroma_Filter(unsigned char *Frame, int Width, int Height, int Hue, int Hth, int Sat, int Sth, int Imin);
	void			 	BlobtomyBlob(myBlob *nBlob, Blob *Blobs, int length);
	void 				Sort_Blobs(myBlob *nBlob , int length);
	void 				Mark_myBlob_BB(myBlob Blobs, unsigned char *Frame, int Width);
	void 				Black_Blob(myBlob *nBlob,unsigned char *Frame, int Width,int t);
	void 				Write_Merged_Entry(myBlob nBlob, myBlob tBlob, FILE *Report);

   /*Begin Sequence Processing*/
   if (argc !=5) {
	  fprintf(stderr, "Usage: %s seqname start end, mspf\n", argv[0]);
	  exit(1);
   }
   SeqName = argv[1];

   if (sscanf(argv[2], "%d", &Start) != 1 || sscanf(argv[3], "%d", &End) != 1) {
	  fprintf(stderr, "[%s:%s] are invalid start/end numbers\n", argv[2], argv[3]);
	  exit(1);
   }

   if (sscanf(argv[4], "%d", &MSpF) != 1) {
	  fprintf(stderr, "[%s] is an invalid mS per frame\n", argv[4]);
	  exit(1);
   }

   /*If any of the arguments fail, Initialize Debug Sequences*/
   if (DEBUG)
	 printf("Trial: Seq= %s, Start= %d, End= %d, MSpF= %d ...\n", \
		SeqName, Start, End, MSpF);
   sprintf(Path, "%s/%s/%05d.jpg", SEQ_DIR, SeqName, Start);

   if (DEBUG)
	  printf("Reading header of %s ...\n", Path);
   Read_Header(Path, &Width, &Height);

   if (DEBUG)
	  printf("Image size (%ux%u)\n", Width, Height);

   /*Allocate memory for Frame*/
   	FrameSize = 3 * Width * Height * sizeof(unsigned char);
   	Frame = (unsigned char *) malloc(FrameSize);

   if (Frame == NULL) {
	  fprintf(stderr, "Unable to allocate image frame memory\n");
	  exit (1);
   }

   /*Allocate memory for Density Frame*/
	DensityMap = (unsigned char *) malloc(Width * Height * sizeof(unsigned char));
	
	if (DensityMap == NULL) {
	  fprintf(stderr, "Unable to allocate Density Map memory\n");
	  exit (1);
   }

   /*Allocate memory for Hue Frame*/
   Hfrm = (unsigned char *) malloc(3 * FrameSize);

   if (Hfrm == NULL) {
	  fprintf(stderr, "Unable to allocate HSI image frame memory\n");
	  exit (1);
   }

	/*Define Saturation and Intensity Frames*/
   Sfrm = Hfrm + FrameSize;
   Ifrm = Sfrm + FrameSize;
   
   /*Check for trial directories*/
   if (InDir("trials", BASE_DIR) == FALSE) {
	  if (DEBUG)
		 printf("	creating %s ...\n", TRIAL_DIR);
		
	  mkdir(TRIAL_DIR, (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH));
   }

   if (InDir(SeqName, TRIAL_DIR) == FALSE) {
	  sprintf(Path, "%s/%s", TRIAL_DIR, SeqName);
	  if (DEBUG)
		 printf("	creating %s ...\n", Path);
		
	  mkdir(Path, (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH));
   }

	/*Save Report for Test Sequence*/
	
   Report = Create_Report(SeqName, Start, End, MSpF);

	for (N = Start; N < End + 1; N++) {
	  
	  	Mark_Frame_Number(Report, N);
	  	sprintf(Path, "%s/%s/%05d.jpg", SEQ_DIR, SeqName, N);
		if (DEBUG)
		 	printf("Processing frame %05d.jpg ...\n", N);
		Load_Image(Path, Frame);
		/*Black Out the top 1/3 of the Frame for efficiency as it is beyond the scope of most cars*/
		Clean_Frame(Frame,Height/2.75,Width);
		
		/*Saving Original*/
		if (DEBUG){
			printf("Saving original frame %05d.jpg ...\n", N);
			sprintf(Path, "%s/%s/%05d-org.jpg", TRIAL_DIR, SeqName, N);
		  	Store_Image(Frame, Path, Width, Height);
			
		}

		/*  
			Tried Using Hue Filter but it has less constraints
			Hue_Filter(Frame, Width, Height, Hue, Hth, Smin, Imin);	
		*/
			
		/*Chroma Filter With Threshold Values as described in chroma.c*/
		Chroma_Filter(Frame,Width,Height,32,18,150,120,25);
		
		/*Write Values for Chroma or Hue Filter for Debugging*/
		if (DEBUG){
			printf("Saving filter frame %05d.jpg ...\n", N);
			sprintf(Path, "%s/%s/%05d-filter.jpg", TRIAL_DIR, SeqName, N);
			Store_Image(Frame, Path, Width, Height);
		}
				
		/*Writing HSI frames for debugging thresholds*/
	  	if (DEBUG){
		
			for (I = 0; I < FrameSize; I += 3) {
		 		RGB2HSIin((Pixel *) (Frame + I));
		 		HueRGB(Frame[I], (Pixel *) (Hfrm + I));
			 	Sfrm[I] = Sfrm[I+1] = Sfrm[I+2] = Frame[I+1];
			 	Ifrm[I] = Ifrm[I+1] = Ifrm[I+2] = Frame[I+2];
		  	}
		
			/*Saving Hue Frame*/
			printf("Saving hue frame %05d.jpg ...\n", N);
			sprintf(Path, "%s/%s/%05d-hue.jpg", TRIAL_DIR, SeqName, N);
		  	Store_Image(Hfrm, Path, Width, Height);
		
			/*Saving Saturation Frame*/	 
			printf("Saving saturation frame %05d.jpg ...\n", N);
			sprintf(Path, "%s/%s/%05d-sat.jpg", TRIAL_DIR, SeqName, N);
		  	Store_Image(Sfrm, Path, Width, Height);
		
			/*Saving Intensity Frame*/
			printf("Saving intensity frame %05d.jpg ...\n", N);
			sprintf(Path, "%s/%s/%05d-int.jpg", TRIAL_DIR, SeqName, N);
			Store_Image(Ifrm, Path, Width, Height);			
		}

		/*Algorithm:
		  Evaluate the Area Image Density and Find the Thershold Frame.
		  The Threshold Frame undergoes Area Image Density Once Again 
		  to use filtered Frame with the Blob_Finder Function to find 
		  potential Blobs.
		*/
		
		Area_Image_Density(Frame, Width, Height, DensityMap, Wsize);
		Threshold_Frame(Frame, Width, Height, Threshold, DensityMap);
		Area_Image_Density(Frame, Width, Height, DensityMap, Wsize);
		Blobs = Blob_Finder(DensityMap, Width, Height, Bth);
		
		length = Blob_List_Length(Blobs);
		
		/*Initialize Custom Blob Struct*/
		myBlob	nBlob[length];
		
		/*Copy Values to myBlob Structure*/		
		BlobtomyBlob(nBlob,Blobs,length);
		
		/*Sorting Blobs based on Size of the Blob*/
		Sort_Blobs(nBlob,length);
		
		/*Cleanup unwanted Blobs*/
		
		/*First Check for Large Red  Objects and if very large objects are found skip 
		the rest of the processing under the assumption that the car is close */
		int bigRed=0;
		myBlob trashBlob[length];
		int t=0;
		int z=0;
		for(t=0;t<length;t++){
			if(nBlob[t].Count > 5000 ){
				if(bigRed < 1){
					tmpBlob = nBlob[t];
				}
				bigRed++;
				if(bigRed >= 2) {
					Write_Merged_Entry(nBlob[t], tmpBlob,Report);
					if(DEBUG){
						Mark_myBlob_BB(nBlob[t], Frame,Width);
						Mark_myBlob_BB(tmpBlob, Frame,Width);
					}					
					skipBlobs = 1;
					break;
				}
			}
		}
		
		/*Additional Filtering for Smaller Blobs is based on tested threshold values 
		for the difference in the center of mass of each Blob, their Area covered and the Distance between them*/
		
		for(t = 0; t<length ;t++) {
			for(z = t+1; z < length;z++){
				if( abs(nBlob[t].Ycom - nBlob[z].Ycom) < Yth && abs(nBlob[t].Xcom - nBlob[z].Xcom) < Xth && skipBlobs != 1) {
					if(abs(nBlob[t].Count -nBlob[z].Count) < Cth && abs(nBlob[t].Xmin-nBlob[z].Xmin) < Xmth) {
						Write_Merged_Entry(nBlob[t], nBlob[z],Report);
						/*Mark Blobs for Debugging*/
						if(DEBUG){
							Mark_myBlob_BB(nBlob[t], Frame,Width);
							Mark_myBlob_BB(nBlob[z], Frame,Width);
						}
						t++;
					}
		 	 		else {
						if(DEBUG)
							Black_Blob(nBlob,Frame,Width,t); //Blacken Blobs that do no meet the criteria
					}
				}
				else {
					if(DEBUG)
						Black_Blob(nBlob,Frame,Width,t); //Blacken Blobs that do not meet the criteria
					trashBlob[t] = nBlob[t];
				}
			}
		}

		//Check to see if there was only one large Red Blob and assume it is a car and write it to the file
		if(bigRed == 1){
			Write_Contact_Entry(Report, tmpBlob.Xmin, tmpBlob.Ymin+100, tmpBlob.Xmax, tmpBlob.Ymax-50,tmpBlob.Count);
		}
		
		/*Make a quick runthrough to check for missed blobs from the Trashblob Object 
		using the same values as before with stricter constraints*/
		for(t = 0; t<length; t++){
			for(z = t+1; z < length;z++){
				if(abs(trashBlob[t].Ycom -trashBlob[z].Ycom) < 10 && abs(trashBlob[t].Ycom -trashBlob[z].Ycom) !=0 && abs(trashBlob[z].Xcom -trashBlob[t].Xcom) != 0 && abs(trashBlob[t].Count -trashBlob[z].Count) < 150 && skipBlobs != 1 && abs(trashBlob[t].Xmin-trashBlob[z].Xmin) < 100){
					Write_Merged_Entry(nBlob[t], nBlob[z],Report);
					
					if(DEBUG){
						Mark_myBlob_BB(trashBlob[t], Frame,Width);
						Mark_myBlob_BB(trashBlob[z], Frame,Width);
					}
					break;
				}				
			}
		}
		
		/*Write Blob Frame for Debugging*/
		if(DEBUG){
			printf("Report Written for Frame %05d \n",N);
			sprintf(Path, "%s/%s/%05d-blob.jpg", TRIAL_DIR, SeqName, N);
			Store_Image(Frame, Path, Width, Height);
		}
		
      	Free_Blobs(Blobs);		
   }
	/*Cleaning the Pipes*/	
   	Close_Report(Report);
   	free(Frame);
   	free(Hfrm);
	free(DensityMap);
   	exit(0);
}

/*Functions Used for the Tail Light Finding Program*/

/*Combine Headlight Pair and Write to Report File*/
void Write_Merged_Entry(myBlob nBlob, myBlob tBlob, FILE *Report) {
	int rXmin,rXmax,rYmin,rYmax;
	
	if(nBlob.Xmin < tBlob.Xmin)
		rXmin = nBlob.Xmin;
	else
		rXmin = tBlob.Xmin;
		
	if(nBlob.Ymin < tBlob.Ymin)
		rYmin = nBlob.Ymin;
	else
		rYmin = tBlob.Ymin;
		
	if(nBlob.Xmax > tBlob.Xmax)
		rXmax = nBlob.Xmax;
	else
		rXmax = tBlob.Xmax;		
	
	if(nBlob.Ymax > tBlob.Ymax)
		rYmax = nBlob.Ymax;
	else
		rYmax = tBlob.Ymax;
		
	Write_Contact_Entry(Report, rXmin, rYmin, rXmax, rYmax,(rXmax-rXmin)*(rYmax-rYmin));										
}

/*Used for Debugging to Black out Blobs*/
void Black_Blob(myBlob *nBlob,unsigned char *Frame, int Width,int t){
	int Row,Col;
	for(Row = nBlob[t].Xmin; Row < nBlob[t].Xmax; Row++) {
		for(Col = nBlob[t].Ymin; Col <= nBlob[t].Ymax; Col++) {
			Mark_Pixel(Row,Col, 0, 0, 0, Frame,Width);
		}
	}
}
/*Convert Blobs to Custom Blob Structure*/
void BlobtomyBlob(myBlob *nBlob, Blob *Blobs, int length) {
	int initlength=0;
	while(Blobs) {
		nBlob[initlength].Xmin = Blobs->Xmin;
		nBlob[initlength].Ymin = Blobs->Ymin;
		nBlob[initlength].Xmax = Blobs->Xmax;
		nBlob[initlength].Ymax = Blobs->Ymax;
		nBlob[initlength].Xcom = Blobs->Xsum/Blobs->Count;
		nBlob[initlength].Ycom = Blobs->Ysum/Blobs->Count;			
		nBlob[initlength].Count = Blobs->Count;
		initlength = initlength+1;
		Blobs = Blobs->Next;
	}
}

/*Mark myBlob on the Frame for Debugging*/
void Mark_myBlob_BB(myBlob Blobs, unsigned char *Frame, int Width) {
	//Below declarations are not necessary but are provided to supress warnings
	void Draw_V_Line(int Ymin, int Ymax, int X, unsigned char R, unsigned char G, unsigned char B, unsigned char *Frame, int Width);
	void Draw_H_Line(int Xmin, int Xmax, int Y, unsigned char R, unsigned char G, unsigned char B, unsigned char *Frame, int Width);
	Draw_H_Line(Blobs.Xmin, Blobs.Xmax, Blobs.Ymin, 255, 0, 0, Frame, Width);
	Draw_H_Line(Blobs.Xmin, Blobs.Xmax, Blobs.Ymax, 255, 0, 0, Frame, Width);
	Draw_V_Line(Blobs.Ymin, Blobs.Ymax, Blobs.Xmin, 255, 0, 0, Frame, Width);
	Draw_V_Line(Blobs.Ymin, Blobs.Ymax, Blobs.Xmax, 255, 0, 0, Frame, Width);
}

/*Insertion Sort Function based on class material from CS 1372*/
void Sort_Blobs(myBlob *nBlob , int length) {
	int t;
	for(t = 1; t<length; t++) {
		myBlob key = nBlob[t];
		int value = nBlob[t].Count;
		int j;
		for (j = t-1; j>=0; j--) {
			if(nBlob[j].Count <= value) break;
			nBlob[j+1] = nBlob[j];
		}
		nBlob[j+1] = key;
	}
}
/*Black out top third of the frame for efficiency*/
void Clean_Frame(unsigned char *Frame, int Height, int Width) {
	int I;
	for (I = 0; I < Width * Height; I++) {
	 Frame[3*I] = Frame[3*I+1] = Frame[3*I+2] = 0;
	}
}
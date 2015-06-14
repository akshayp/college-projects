/* 29 August 2009	               <Akshay Patel>
								   <gth708n>
This program finds the median value in a 100 element integer array. */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   int	Nums[100];
   int	NumNums, Median;
   int  Load_Mem(char *, int *);

   if (argc != 2) {
     printf("usage: ./P1-2 valuefile\n");
     exit(1);
   }
   NumNums = Load_Mem(*++argv, Nums);
   if (NumNums != 100) {
      printf("valuefiles must contain 100 entries\n");
      exit(1);
   }

   /* Your program goes here */
   
	int i,j,temp;
	/*Applying a Bubble Sort on the Input Array*/
	for(i=0;i<NumNums;i++) {
		for(j=i+1;j<NumNums;j++) {
			if(Nums[i]>Nums[j]) {
				temp = Nums[j];
				Nums[j] = Nums[i];
				Nums[i] = temp;
			}
		}
	}
	/*Adding a conditional check to calculate median in case input is not even*/
	if(NumNums%2 == 0) {
		Median = (Nums[NumNums/2]+Nums[NumNums/2-1])/2;	
	}
	else {
		Median = (Nums[NumNums/2]);
	}

   printf("The median of the array is %d\n", Median);
   exit(0);
}

/* This routine loads in up to 100 newline delimited integers from
a named file in the local directory. The values are placed in the
passed integer array. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int IntArray[]) {
   int	N, Addr, Value, NumVals;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL) {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   } else {
      for (N=0; N < 100; N++) {
         NumVals = fscanf(FP, "%d: %d", &Addr, &Value);
         if (NumVals == 2)
            IntArray[N] = Value;
         else
            break;
      }
      fclose(FP);
      return N;
   }
}

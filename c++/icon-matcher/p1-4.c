/* 

This program loads 12 icon maps into a linear array.
It may contain bugs ... at no additional charge! 

*/

#include <stdio.h>

// Define Total Array Size
#define IconArraySize 1728

// Initialize explicit functions to avoid Compilation Warnings

int Load_Mem(char *InputFileName, int IntArray[]);
void exit(int status);
int compare(int array1[], int array2[],int col);

//Main Method

int main(int argc, char *argv[])
{
   int	Icons[IconArraySize];
   int	Length, Match;

   if (argc != 2)
   {
     printf("usage: P1-4 valuefile\n");
     exit(1);
   }

   Length = Load_Mem(*++argv, Icons);
   if (Length != IconArraySize)
   {
	  printf("valuefiles does not contain enough data\n");
      exit(1);
   }

   /* Your icon matching program goes here */

   //Define Icon variables 0-7 and A-D
	int Icon_0[144],Icon_1[144],Icon_2[144],Icon_3[144],Icon_4[144],Icon_5[144],Icon_6[144],Icon_7[144];
	int Icon_A[144],Icon_B[144],Icon_C[144],Icon_D[144];
	int K;
	// Parse Icon Array to store each Icon seperately
	for (K=0; K < 144; K++)
	{
		Icon_0[K]=Icons[K];
		Icon_1[K]=Icons[K+144];
		Icon_2[K]=Icons[K+(144*2)];
		Icon_3[K]=Icons[K+(144*3)];
		Icon_4[K]=Icons[K+(144*4)];
		Icon_5[K]=Icons[K+(144*5)];
		Icon_6[K]=Icons[K+(144*6)];
		Icon_7[K]=Icons[K+(144*7)];
		Icon_A[K]=Icons[K+(144*8)];
		Icon_B[K]=Icons[K+(144*9)];
		Icon_C[K]=Icons[K+(144*10)];
		Icon_D[K]=Icons[K+(144*11)];
	}

	// Icon Compare Condtionals.
	// Matching reference icon is returned is conditions are satisfied by the compare function.
		if(compare(Icon_A,Icon_0,144)==1||compare(Icon_B,Icon_0,144)==1||compare(Icon_C,Icon_0,144)==1||compare(Icon_D,Icon_0,144)==1)
			Match=0;
		else if(compare(Icon_A,Icon_1,144)==1||compare(Icon_B,Icon_1,144)==1||compare(Icon_C,Icon_1,144)==1||compare(Icon_D,Icon_1,144)==1)
			Match=1;
		else if(compare(Icon_A,Icon_2,144)==1||compare(Icon_B,Icon_2,144)==1||compare(Icon_C,Icon_2,144)==1||compare(Icon_D,Icon_2,144)==1)
			Match=2;
		else if(compare(Icon_A,Icon_3,144)==1||compare(Icon_B,Icon_3,144)==1||compare(Icon_C,Icon_3,144)==1||compare(Icon_D,Icon_3,144)==1)
			Match=3;
		else if (compare(Icon_A,Icon_4,144)==1||compare(Icon_B,Icon_4,144)==1||compare(Icon_C,Icon_4,144)==1||compare(Icon_D,Icon_4,144)==1)
			Match=4;
		else if (compare(Icon_A,Icon_5,144)==1||compare(Icon_B,Icon_5,144)==1||compare(Icon_C,Icon_5,144)==1||compare(Icon_D,Icon_5,144)==1)
			Match=5;
		else if (compare(Icon_A,Icon_6,144)==1||compare(Icon_B,Icon_6,144)==1||compare(Icon_C,Icon_6,144)==1||compare(Icon_D,Icon_6,144)==1)
			Match=6;
		else
			Match=7;

	/*Program Ends Here*/
	printf("Icon Match:\n");
	printf("The matching reference icon is %d\n", Match);
	return Match;
}

/* This routine loads in up to IconArraySize newline delimited integers from
a named file in the local directory. The values are placed in the
passed integer array. The number of input integers is returned. */

int Load_Mem(char *InputFileName, int IntArray[]) {
   int	N, Addr, Value, NumVals;
   FILE	*FP;

   FP = fopen(InputFileName, "r");
   if (FP == NULL)
   {
      printf("%s could not be opened; check the filename\n", InputFileName);
      return 0;
   }
   else
   {
      for (N=0; N < IconArraySize; N++)
      {
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
/*
 * Compare() function
 * This function takes in two arrays and the number of elements in them
 * to test if the two are exactly the same and returns 1 as the output if
 * the array's match and 0 otherwise.
 */
int compare(int array1[], int array2[],int col)
{
int result=1;
int c;
for(c=0; c<col; c++)
	{
		if(array1[c]!=array2[c])
		{
			result=0;
			return result;
		}
	}
return result;
}

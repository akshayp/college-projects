/*                  Chroma Collision Avoidance Report Writer

This library contains functions that generate a collision avoidance report.

(c) 2009 Scott & Linda Wills

Documentation:

This library contains functions to generate a properly formated plane
tracker report.  The report is formated as follows:

Seq:     "sequence name"
Start:   0001
End:     0400
MSpF:     250

FN: 1
CE: Xmin, Ymin, Xmax, Ymax, Area
CE: Xmin, Ymin, Xmax, Ymax, Area

FN: 2
CE: Xmin, Ymin, Xmax, Ymax, Area
CE: Xmin, Ymin, Xmax, Ymax, Area

...

Key Data Structures:

Report: A file pointer to the currently generated report.

Key Functions:

Create_Report(): This routine creates a new report file in the trial
directory. It returns a file pointer.

Mark_Frame_Number(): This routine marks the current frame number in
the report.

Write_Contact_Entry(): This routine writes a contact entry with
position.

Close_Report(): This routine closes the report.

Here's a typical usage example:
foo() {
   FILE                 *Report;

   ...
   Report = Create_Report(SeqName, Start, End, MSpF);
   ...
   for (each frame) {
      Mark_Frame_Number(Report, framenumber);

      for(each contact) {
         ...
         Write_Contact_Entry(Report, Xmin, Ymin, Xmax, Ymax, Area);
         ...
      }
   }
   Close_Report(Report);
   ...
}

*/

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "report.h"

/* This routine generates a collision avoidance report. */

FILE *Create_Report(char *SeqName, int Start, int End, int MSpF) {

   char                 ReportPath[128];
   FILE                 *Report;

   sprintf(ReportPath, "%s/%s/report.txt", TRIAL_DIR, SeqName);
   Report = fopen(ReportPath, "w");
   if (Report == NULL) {
      printf("report file cannot be opened at %s\n", ReportPath);
      exit(1);
   }
   fprintf(Report, "Seq:     \"%s\"\n", SeqName);
   fprintf(Report, "Start:   %4d\n", Start);
   fprintf(Report, "End:     %4d\n", End);
   fprintf(Report, "MSpF:     %3d\n", MSpF);
   return (Report);
}

/* This routine marks a frame number. */

void Mark_Frame_Number(FILE *Report, int FrameNum) {

   fprintf(Report, "\nFN: %04d\n", FrameNum);
}

/* This routine writes a contact entry (Xpos, Ypos). */

void Write_Contact_Entry(FILE *Report, int Xmin, int Ymin, int Xmax, int Ymax, int Area) {

   fprintf(Report, "   CE: (%3d,%3d) (%3d,%3d) %5d\n", Xmin, Ymin, Xmax, Ymax, Area);
}

/* This routine writes a blob entry (Xcom, Ycom, Xmin, Ymin, Xmax,
   Ymax, Area). */

void Write_Blob_Entry(FILE *Report, int Xcom, int Ycom, int Xmin, int Ymin, \
		      int Xmax, int Ymax, int Area) {
   fprintf(Report, "   BE: %3d, %3d, %3d, %3d, %3d, %3d, %5d\n", Xcom, Ycom, Xmin, Ymin, Xmax, Ymax, Area);
}

/* This routine closes the report. */

void Close_Report(FILE *Report) {

   fclose(Report);
   exit(0);
}

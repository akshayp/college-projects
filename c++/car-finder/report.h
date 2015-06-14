/*                  Car Tracker Report Writer

This library contains functions that generate a car tracking report.

(c) 2009 Scott & Linda Wills
*/

extern FILE *Create_Report(char *SeqName, int Start, int End, int MSpF);
extern void Mark_Frame_Number(FILE *Report, int FrameNum);
extern void Write_Contact_Entry(FILE *Report, int Xmin, int Ymin, int Xmax, int Ymax, int Area);
extern void Write_Blob_Entry(FILE *Report, int Xcom, int Ycom, int Xmin, int Ymin, \
			     int Xmax, int Ymax, int Area);
extern void Close_Report(FILE *Report);

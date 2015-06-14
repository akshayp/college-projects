/* vector.h - Routines to handle vectors.
 */

#pragma once

#include "myError.h"
/* We use the package wide standard error codes. */
 /* We return package standard error codes. */

/* What does a vector look like? */
typedef struct vector {
	int size;
	double *data;
} vector;

typedef struct {
	double value;
	int loc;
} dblint_t;


typedef double (*d_func_d_t)(double);
typedef double (*d_func_di_t)(double, int);
typedef double (*d_func_ddd_t)(double, double, double);

#include "matrix.h"

/* We make use of the matrix routines. */
/* --------------   Routines    ---------------------------- */
/* Note that input and output vectors CAN be the same.
 * So you can scale a vector with: vecscl (6, A, A, scale)
 *
 * All routines in this package can return
 * ERRnil  - You can't pass a nil pointer for a vector.
 * ERRsize - You have to have a positive size for a vector.
 * 
 * In addition, vecscl  and vecadds can return
 * ERRmeaning - Scaling by zero is not meaningful.
 *    (Although they will do the scale anyway)
 */
extern vector buildvector( int size );
extern vector buildvector( double x[], int size );

extern void concat( vector *vp, double x);
extern void concat( vector *vp, vector x);

/* Set a vector to zeros. */
extern void zero (vector result);
extern void one (vector result);

extern vector colon (double from, double inc, double to);
extern vector colon (double from, double to);
extern vector index( vector source, vector index );
extern vector linspace(double from, double to);
extern vector linspace(double from, double to, int number);
extern vector linearize(matrix m);
extern void copyvectordata(vector from, vector to);
extern vector applyFunction( d_func_d_t, vector v );
extern vector applyFunction( d_func_di_t, vector v, int n );
extern vector applyFunction( d_func_dd_t, vector v, double x );
/*
	A strange but useful function:  this goes down the source
	vector and replaces each value with the result of applying
	the given function to that value, the test vector value and
	the constant x.  It can be used, for example, to replace
	all the values in from that are greater than a limit with
	that limit with the following code:
				double gt(double x, double y) {
					if(x > y) return 1.0;
					else return 0.0;
				}
				double subIfTrue(double from, double test, double replace) {
					if(test > 0) return replace;
					else return from;
				}
				isgt = applyFunction( gt, sy, 2);
				applyInPlace(subIfTrue, sy, isgt, 2);

*/
extern void applyInPlace(d_func_ddd_t f, vector source, vector test, double x);
extern vector polyfit(vector x, vector y, int power);
extern vector polyval(vector coef, vector x);
extern double sum(vector A);
/*
	This is not the usual MATLAB style spline.  It assumes that
	the x data points are uniformly distributed, and operates
	strictly on the y values.  It will produce 10 times the
	original number of y values in the range of the original
	x values.  Therefore, to plot it, you need to create an x
	vector as follows:
					sx = linspace(1, N, sy.size);
	where N is the original number of y values, and sy is the
	vector produced by spline.
*/
extern vector spline(vector x);
extern double *getVectorData(vector v);

/* from is assumed to be a temporary vector whose data can be freed */

/* Copy */
   /* regular */
extern vector copy (vector source);
   /* Swap */
//extern void swap (vector A, vector B);

/* Scale a vector */
extern vector dotmult (vector given, double scalar);
/* Scale factor must be non-zero.  (ERRmeaning) */

/* Add vectors */
    /* regular */
extern vector operator+(vector A, double d);
extern vector operator+(vector A, vector B);
extern vector operator-(vector A, vector B);
extern vector dotmult(vector A, vector B);
extern vector dotmult(double s, vector v);
extern vector dotmult(vector v, double s);
extern vector dotdiv(vector A, vector B);
    /* scaled */
extern vector addscaled(vector A, vector B, double scale);
extern double interp1(vector x, vector y, double x0);
extern vector interp1(vector x, vector y, vector xi);

/* Dot product */
extern double dot (vector A, vector B);
extern void vfree(vector v);
extern void addInPlace( vector a, vector b);
extern vector randV(int sz);
extern double gt(double x, double y);
extern double lt(double x, double y);
extern double subIfTrue(double from, double test, double replace);

extern dblint_t maxV(vector v);
extern dblint_t minV(vector v);

#ifdef _DEBUG
extern void print(FILE *f, char *str, vector v);
#endif

/* ========================================================================== */

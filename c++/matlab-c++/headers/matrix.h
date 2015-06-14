/* matrix.h - Routines to handle matrices.
 * (C) Copyright 1999 by John Halleck
 * All rights reserved.
 */

#pragma once

/* Version of September 6th, 1999 */

#include "myError.h"
#include <stdio.h>

#define PI 3.14159265359

 /* We return package standard error codes. */
 /* All of the routines can return:
  * ERRnil - You can't pass nil pointers to the routines.
  * ERRsize - Matrices have to have positive numbers of rows and columns.
  *
  * matscl can return
  * ERRmeaning - The operation is meaningless (scaling by zero for example)
  */

/* What does a matrix look like? */
typedef struct matrix {
	int rows;
	int cols;
	double **data;
} matrix;

typedef double (*d_func_dd_t)(double, double);
typedef double (*d_func_ddi_t)(double, double, int);

#include "vector.h"

/* --------------   Routines    ---------------------------- */

extern matrix applyFunction( d_func_dd_t, matrix, matrix  );
extern matrix applyFunction( d_func_ddi_t, matrix, matrix, int );
extern matrix zeros(int, int);
extern matrix ones(int, int);
extern matrix index(matrix, vector, vector);
extern matrix buildmatrix( matrix);

extern matrix buildmatrix( int, int );
extern matrix buildmatrix(vector, vector, vector, vector);
extern void mfree(matrix);
extern matrix readmatrix(FILE *);

/* Make a matrix be the zero matrix */
extern void zero(matrix);

/* Make a matrix be the identity */
extern void identity(matrix);

/* Copy */
    /* regular */
extern matrix copy(matrix source);
    /* transpose */
extern matrix transpose (matrix source);
    /* rows and columns refer to result.*/

/* Scale a matrix */
extern matrix dotmult (matrix given, double scalar);
/* Scalar must be non-zero */

/* Add */
extern matrix add (matrix A, matrix B);
extern matrix sub (matrix A, matrix B);

/* Multiply */

/* Multiply  matrices.   result = matrix a multiplied by matrix b;  */
extern matrix mult (matrix A, matrix B);
                  /* result is rrows x rcols */
                  /* A      is rrows x isize */
                  /* B      is isize x rcols */
/* result MUST be a matrix distinct from either A or B. */


/* Multiply a matrix by the transpose of another matrix. */
extern matrix multt (matrix A, matrix B);
                      /* result is rrows x rcols        */
                      /* A      is rrows x isize        */
                      /* B      is rcols x isize < ===  */
/* result MUST be a matrix distinct from either A or B. */

#ifdef _DEBUG
extern void print(FILE *f, char *str, matrix m);
#endif

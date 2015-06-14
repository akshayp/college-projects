/* qinv.h
 * DMS
 */
#pragma once

#include "myError.h"
/* We use the package wide standard error codes. */
#include "vector.h"
/* We make use of the vector routines. */
#include "matrix.h"
/* We make use of the matrix routines. */

extern matrix inv (matrix given);
/* Invert non-singular array */
/* Inverse of a positive definite matrix. result and working must be
 * distinct arrays.
 * Note that positive definite matricies do not require pivoting or
 * other rearrangement to form the inverse.
 *
 *  * This is NOT a GENERAL matrix inverse *
 *
 * This does not make use of the fact that all the positive definite
 * matrices in this package are symmetrical
 */
extern matrix invns (matrix given);

/* myError.h  Routines to aid error handling.
 * (C) Copyright 1999 by John Halleck
 * All rights reserved
 */
/* Version of August 23rd, 1999 */

#include <stdio.h>

#pragma once
#define _CRT_SECURE_NO_DEPRECATE 1
#ifdef _DEBUG
extern FILE *debug;
#endif

extern void error(char *str); 





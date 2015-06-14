// graphicTools.h  header file
// author: David M. Smith
// 3/23/06
//

#pragma once
#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include "graphicData.h"
#include "graphicObject.h"

#ifdef _DEBUG
extern FILE *debug;
#endif

#include "vector.h"

extern void plot(vector x, vector y);
extern void plot(vector x, vector y, char *c);
extern void plot3(vector x, vector y, vector z);
extern void plot3(vector x, vector y, vector z, char *c);
extern void hold(bool setting);
extern void axes(bool setting);
extern char *input(char *prompt);
extern void meshgrid(vector, vector, matrix *, matrix *);
extern void mesh(matrix, matrix, matrix);
extern void testPlot();
extern void initalizeGraphicsData();
extern PLOT_DATA *newPlot2D(vector, vector, char *);
extern PLOT_DATA *newPlot3D(vector, vector, vector, char *);
extern PLOT_DATA *newPlaid(matrix, matrix, matrix, bool );

DWORD_PTR __declspec(dllimport) makeNewThread();
BOOL __declspec(dllimport) setWindowTitle(DWORD_PTR, char *);
BOOL __declspec(dllimport) addEntry(DWORD_PTR, DWORD_PTR);
BOOL __declspec(dllimport) updateFrame(DWORD_PTR, DWORD_PTR);

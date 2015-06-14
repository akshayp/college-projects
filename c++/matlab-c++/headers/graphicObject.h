/* graphicObject.h - Routines to graphic data.
 */

#pragma once
#include "vector.h"

typedef enum plot_type { NO_PLOT,
                         PLOT_2D,
                         PLOT_3D,
						 PLAID,
                         IMAGE } PLOT_TYPE;

typedef struct plot_2d_data {
	vector xv;
	vector yv;
	char color[4];
} PLOT_2D_DATA;

typedef struct plot_3d_data {
	vector xv;
	vector yv;
	vector zv;
	char color[4];
} PLOT_3D_DATA;

typedef struct plaid_data {
	matrix xx;
	matrix yy;
	matrix zz;
	bool flag;
} PLAID_DATA;


typedef struct plot_d {
	PLOT_TYPE type;
	int npts;
	double x;
	double y;
	double z;
	union {
	    PLOT_2D_DATA p2d;
	    PLOT_3D_DATA p3d;
		PLAID_DATA pld;
	};
} PLOT_DATA;


/* ========================================================================== */

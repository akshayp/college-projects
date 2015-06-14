/* graphicData.h - Routines to graphic data.
 */

#pragma once

#include "graphicObject.h"

typedef struct limit_t {
	double xMin;	// minimum x value
	double yMin;	// minimum y value
	double zMin;	// minimum z value
	double xMax;	// maximum x value
	double yMax;	// maximum y value
	double zMax;	// maximum z value
} LIMIT_TYPE;

typedef struct grData {
	LIMIT_TYPE limits;	// limits of the graphics data
	bool axes;
	bool hold;
	bool someData;
} GRAPHIC_DATA;

extern void graphicsDataAdd(GRAPHIC_DATA *g, PLOT_DATA *p);
void graphicsDataInit(GRAPHIC_DATA *g);
extern void graphicsDataShowLimits(GRAPHIC_DATA *g, FILE *f, char *str);
void graphicsDataUpdateLimits(GRAPHIC_DATA *g, PLOT_DATA *p);

/* ========================================================================== */

#ifndef PIXIE_EXTS_H
#define PIXIE_EXTS_H
#include "stdint.h"

/**********/
/* PVEC2F */
/**********/
typedef struct PVec2f
{
	float x;
	float y;

} PVec2f;

PVec2f 	PVec2fAdd(PVec2f v, PVec2f s);
PVec2f 	PVec2fSub(PVec2f v, PVec2f s);
PVec2f 	PVec2fMul(PVec2f v, float n);
PVec2f 	PVec2fDiv(PVec2f v, float n);
PVec2f 	PVec2fNormalize(PVec2f v);
PVec2f 	PVec2fRotate(PVec2f v, PVec2f s, float angle);
float 	PVec2fLen(PVec2f v);
float 	PVec2fDot(PVec2f v, PVec2f s);
float 	PVec2fCross(PVec2f v, PVec2f s);

/**********/
/* PVEC3F */
/**********/
typedef struct PVec3f
{
	float x;
	float y;
	float z;

} PVec3f;

PVec3f 	PVec3fAdd(PVec3f v, PVec3f s);
PVec3f 	PVec3fSub(PVec3f v, PVec3f s);
PVec3f 	PVec3fMul(PVec3f v, float n);
PVec3f 	PVec3fDiv(PVec3f v, float n);
PVec3f 	PVec3fNormalize(PVec3f v);
float 	PVec3fLen(PVec3f v);
float 	PVec3fDot(PVec3f v, PVec3f s);
PVec3f 	PVec3fCross(PVec3f v, PVec3f s);
void 	PVec3fxPMat4f(PVec3f v, float *m, PVec3f *rv);

/**********/
/* PMAT4F */
/**********/
typedef float PMat4f[16];

void PMat4fInit(PMat4f m);
void PMat4fScale(PMat4f m, float scale, PMat4f rm);
void PMat4fTranspose(PMat4f m, PMat4f rm);
void PMat4fTranslateX(PMat4f m, float pos, PMat4f rm);
void PMat4fTranslateY(PMat4f m, float pos, PMat4f rm);
void PMat4fTranslateZ(PMat4f m, float pos, PMat4f rm);
void PMat4fRotateX(PMat4f m, float degree, PMat4f rm);
void PMat4fRotateY(PMat4f m, float degree, PMat4f rm);
void PMat4fRotateZ(PMat4f m, float degree, PMat4f rm);
void PMat4fxPMat4f(PMat4f m, PMat4f mul, PMat4f rm);

/*************/
/* PTRIANGLE */
/*************/
typedef struct PTriangle
{
	PVec3f normal;
	PVec3f a;
	PVec3f b;
	PVec3f c;

} PTriangle;

/********/
/* PSTL */
/********/
typedef struct PStl
{
	float posx;
	float posy;
	float posz;

	float rotx;
	float roty;
	float rotz;

	float scale;

	void *data;
	void *proj;

} PStl;

typedef void (*Drawer)(void *elem);

void PStlLoad(char *file, PStl *stl);

void PStlClose(PStl *stl);

void PStlDrawWireframe(PStl *stl, Drawer func);

void PStlDrawSolid(PStl *stl, Drawer func);

#endif
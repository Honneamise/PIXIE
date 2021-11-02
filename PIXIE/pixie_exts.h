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

/*********/
/* PMATF */
/*********/
typedef struct PMatf
{
    int32_t rows;
    int32_t cols;
    
    float *data;

} PMatf;


PMatf PMatfInit(int32_t rows, int32_t cols, float *data);
void PMatfClose(PMatf *mat);

void PMatfSet(PMatf mat, int32_t row, int32_t col, float val);
float PMatfGet(PMatf mat, int32_t row, int32_t col);

void PMatfAdd(PMatf a, PMatf b, PMatf *c);
void PMatfSub(PMatf a, PMatf b, PMatf *c);
void PMatfMul(PMatf a, PMatf b, PMatf *c);

void PMatfAddScalar(PMatf mat, float num);
void PMatfSubScalar(PMatf mat, float num);
void PMatfMulScalar(PMatf mat, float num);
void PMatfDivScalar(PMatf mat, float num);

void PMatTranspose(PMatf mat, PMatf *res);

#endif
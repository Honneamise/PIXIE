/***********/
/* INCLUDE */
/***********/
//std
#include "stdlib.h"
#include "stdint.h"
#include "assert.h"

//math
#include "math.h"

//pixie
#include "pixie_base.h"
#include "pixie_exts.h"

/**********/
/* PVEC2F */
/**********/
PVec2f PVec2fAdd(PVec2f v, PVec2f s)
{
	return (PVec2f){v.x+s.x, v.y+s.y}; 
}

/**********/
PVec2f PVec2fSub(PVec2f v, PVec2f s)
{
	return (PVec2f){v.x-s.x, v.y-s.y}; 
}

/**********/
PVec2f PVec2fMul(PVec2f v, float n)
{
	return (PVec2f){v.x*n, v.y*n};
}

/**********/
PVec2f PVec2fDiv(PVec2f v, float n)
{
	return (PVec2f){v.x/n, v.y/n};
}

/**********/
PVec2f PVec2fNormalize(PVec2f v)
{
	float m = PVec2fLen(v);

	if (m != 0.0f) { return PVec2fDiv(v, m); };

	return (PVec2f){0.0f,0.0f};
}

/**********/
PVec2f PVec2fRotate(PVec2f v, PVec2f s, float angle)
{
	float x = v.x - s.x;
	float y = v.y - s.y;
	float radians = PRadiansf(-angle);

	float _x = s.x + x * cosf(radians) - y * sinf(radians);
	float _y = s.y + x * sinf(radians) + y * cosf(radians);

	return (PVec2f){_x,_y};
}

/**********/
float PVec2fLen(PVec2f v)
{
	float val = (v.x * v.x) + (v.y * v.y);

	return (float)sqrt(val);
}

/**********/
float PVec2fDot(PVec2f v, PVec2f s)
{
	return (v.x * s.x) + (v.y * s.y);
}

/**********/
float PVec2fCross(PVec2f v, PVec2f s)
{
	return (v.x * s.y) - (v.y * s.x);
}

/**********/
/* PVEC3F */
/**********/
PVec3f PVec3fAdd(PVec3f v, PVec3f s)
{
	return (PVec3f){v.x+s.x, v.y+s.y, v.z+s.z}; 
}

/**********/
PVec3f PVec3fSub(PVec3f v, PVec3f s)
{
	return (PVec3f){v.x-s.x, v.y-s.y, v.z-s.z }; 
}

/**********/
PVec3f PVec3fMul(PVec3f v, float n)
{
	return (PVec3f){v.x*n, v.y*n, v.z*n};
}

/**********/
PVec3f PVec3fDiv(PVec3f v, float n)
{
	return (PVec3f){v.x/n, v.y/n, v.z/n};
}

/**********/
PVec3f PVec3fNormalize(PVec3f v)
{
	float m = PVec3fLen(v);

	if (m != 0.0f) { return PVec3fDiv(v, m); };

	return (PVec3f){0.0f,0.0f,0.0f};
}

/**********/
float PVec3fLen(PVec3f v)
{
	float val = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);

	return (float)sqrt(val);
}

/**********/
float PVec3fDot(PVec3f v, PVec3f s)
{
	return (v.x * s.x) + (v.y * s.y) + (v.z * s.z);
}

/**********/
PVec3f PVec3fCross(PVec3f v, PVec3f s)
{
	return (PVec3f){ (v.y*s.z - v.z*s.y), (v.z*s.x - v.x*s.z), (v.x*s.y - v.y*s.x) };
}


/**********/
/* PMATF */
/**********/
PMatf PMatfInit(int32_t rows, int32_t cols, float *data)
{
    assert(rows>0 && cols>0);

    PMatf mat = { rows, cols, NULL};

    mat.data = PAlloc(rows*cols,sizeof(float));

    if(data!=NULL)
    {
        for(int32_t i=0;i<rows*cols;i++)
        {
            mat.data[i] = data[i];
        }
    }

    return mat;
}

/**********/
void PMatfClose(PMatf *mat)
{
    assert(mat!=NULL);

    mat->rows = 0;
    mat->cols = 0;

    PFree(mat->data);
    mat->data = NULL;
}

/**********/
void PMatfSet(PMatf mat, int32_t row, int32_t col, float val)
{
    assert(mat.data!=NULL && row<mat.rows && col<mat.cols);

    int32_t index = row*mat.cols + col;

    mat.data[index] = val;
}

/**********/
float PMatfGet(PMatf mat, int32_t row, int32_t col)
{
    assert(mat.data!=NULL && row<mat.rows && col<mat.cols);

    int32_t index = row*mat.cols + col;

    return mat.data[index];
}

/**********/
void PMatfAdd(PMatf a, PMatf b, PMatf *c)
{
    assert(a.rows==b.rows && a.cols==b.cols && a.data!=NULL && b.data!=NULL);

    PMatf mat = PMatfInit(a.rows,a.cols,NULL);
    
    for(int32_t k=0;k<mat.rows*mat.cols;k++)
    {
        mat.data[k] = a.data[k] + b.data[k];
    }

    c->rows = mat.rows;
    c->cols = mat.cols;
    PFree(c->data);
    c->data = mat.data;
}

/**********/
void QMatSub(PMatf a, PMatf b, PMatf *c)
{
    assert(a.rows==b.rows && a.cols==b.cols && a.data!=NULL && b.data!=NULL);

    PMatf mat = PMatfInit(a.rows,a.cols,NULL);
    
    for(int32_t k=0;k<mat.rows*mat.cols;k++)
    {
        mat.data[k] = a.data[k] - b.data[k];
    }

    c->rows = mat.rows;
    c->cols = mat.cols;
    PFree(c->data);
    c->data = mat.data;
}

/**********/
void PMatfMul(PMatf a, PMatf b, PMatf *c)
{
    assert(a.data!=NULL && b.data!=NULL && a.cols==b.rows);
    
    PMatf mat = PMatfInit(a.rows,b.cols,NULL);

    for(int32_t i=0;i<a.rows;i++)
    {
        for(int32_t j=0;j<b.cols;j++)
        {
            float sum = 0.0f;

            for(int32_t k=0;k<a.cols;k++)
            {
                float p = a.data[i*a.cols+k] * b.data[k*b.cols+j];
                sum += p;
            }

            mat.data[i*mat.cols+j] = sum;
        }
    }

    c->rows = mat.rows;
    c->cols = mat.cols;
    PFree(c->data);
    c->data = mat.data;
}

/**********/
void PMatfAddScalar(PMatf mat, float num)
{
    assert(mat.rows>0 && mat.cols>0 && mat.data!=NULL);

    for(int32_t k=0;k<mat.rows*mat.cols;k++)
    {
        mat.data[k] = mat.data[k] + num;
    }

}

/**********/
void PMatfSubScalar(PMatf mat, float num)
{
    assert(mat.rows>0 && mat.cols>0 && mat.data!=NULL);

    for(int32_t k=0;k<mat.rows*mat.cols;k++)
    {
        mat.data[k] = mat.data[k] - num;
    }

}

/**********/
void PMatfMulScalar(PMatf mat, float num)
{
    assert(mat.rows>0 && mat.cols>0 && mat.data!=NULL);

    for(int32_t k=0;k<mat.rows*mat.cols;k++)
    {
        mat.data[k] = mat.data[k] * num;
    }

}

/**********/
void PMatfDivScalar(PMatf mat, float num)
{
    assert(mat.rows>0 && mat.cols>0 && mat.data!=NULL);

    for(int32_t k=0;k<mat.rows*mat.cols;k++)
    {
        mat.data[k] = mat.data[k] / num;
    }

}

/**********/
void PMatTranspose(PMatf mat, PMatf *res)
{
    PMatf m = PMatfInit(mat.cols,mat.rows,NULL);

    int32_t count = 0;

	for (int32_t i=0;i<mat.cols; i++) 
	{
		for (int32_t j=0;j<mat.rows; j++) 
		{
			m.data[count] = mat.data[j*mat.cols + i];
			count++;
		}
	}

    res->rows = m.rows;
    res->cols = m.cols;
    PFree(res->data);
    res->data = m.data;
}

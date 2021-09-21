/***********/
/* INCLUDE */
/***********/
//std
#include "stdlib.h"

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
void PVec3fxPMat4f(PVec3f v, float *m, PVec3f *rv)
{
	float vector[4] = { v.x, v.y, v.z, 1.0f };

	float rvector[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	for (int i = 0; i < 16; i++)
	{
		rvector[i/4] += m[i] * vector[i % 4];
	}

	rv->x = rvector[0];
	rv->y = rvector[1];
	rv->z = rvector[2];
}

/**********/
/* PMAT4F */
/**********/
void PMat4fInit(PMat4f m)
{
	for(int i=0;i<16;i++){ m[i] = 0.0f; }

	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10] = 1.0f;
	m[15] = 1.0f;
}

/**********/
void PMat4fScale(PMat4f m, float scale, PMat4f rm)
{
	PMat4f mat = {0};

	PMat4fInit(mat);

	mat[0] = scale;
	mat[5] = scale;
	mat[10] = scale;

	PMat4fxPMat4f(m, mat, rm);
}

/**********/
void PMat4fTranspose(PMat4f m, PMat4f rm)
{
	PMat4f mat = {0};

	int dest = 0;

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			mat[dest] = m[j * 4 + i];
			dest++;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		rm[i] = mat[i];
	}
}

/**********/
void PMat4fTranslateX(PMat4f m, float pos, PMat4f rm)
{
	PMat4f mat = {0};

	PMat4fInit(mat);

	mat[3] = pos;

	PMat4fxPMat4f(m, mat, rm);

}

/**********/
void PMat4fTranslateY(PMat4f m, float pos, PMat4f rm)
{
	PMat4f mat = {0};

	PMat4fInit(mat);

	mat[7] = pos;

	PMat4fxPMat4f(m, mat, rm);
}

/**********/
void PMat4fTranslateZ(PMat4f m, float pos, PMat4f rm)
{
	PMat4f mat = {0};

	PMat4fInit(mat);

	mat[11] = pos;

	PMat4fxPMat4f(m, mat, rm);
}

/**********/
void PMat4fRotateX(PMat4f m, float degree, PMat4f rm)
{
	float radians = (float)(degree * M_PI / 180.0f);

	PMat4f mat = {0};

	PMat4fInit(mat);

	mat[5] = cosf(radians);
	mat[6] = -sinf(radians);
	mat[9] = sinf(radians);
	mat[10] = cosf(radians);

	PMat4fxPMat4f(m, mat, rm);

}

/**********/
void PMat4fRotateY(PMat4f m, float degree, PMat4f rm)
{
	float radians = (float)(degree * M_PI / 180.0f);

	PMat4f mat = {0};

	PMat4fInit(mat);

	mat[0] = cosf(radians);
	mat[2] = sinf(radians);
	mat[8] = -sinf(radians);
	mat[10] = cosf(radians);

	PMat4fxPMat4f(m, mat, rm);

}

/**********/
void PMat4fRotateZ(PMat4f m, float degree, PMat4f rm)
{
	float radians = (float)(degree * M_PI / 180.0f);

	PMat4f mat = {0};

	PMat4fInit(mat);

	mat[0] = cosf(radians);
	mat[1] = -sinf(radians);
	mat[4] = sinf(radians);
	mat[5] = cosf(radians);

	PMat4fxPMat4f(m, mat, rm);

}

/**********/
void PMat4fxPMat4f(PMat4f m, PMat4f mul, PMat4f rm)
{
	PMat4f mat = {0};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat[i * 4 + j] = 0.0f;

			for (int k = 0; k < 4; k++)
			{
				mat[i * 4 + j] += m[i * 4 + k] * mul[k * 4 + j];
			}

		}
	}

	for (int i = 0; i < 16; i++)
	{
		rm[i] = mat[i];
	}

}

/*************/
/* PTRIANGLE */
/*************/
PTriangle *PTriangleInit(PVec3f a, PVec3f b, PVec3f c)
{
	PTriangle *t = PAlloc(1, sizeof(PTriangle));

	t->normal = (PVec3f){ 0.0f, 0.0f, 0.0f };
	t->a = a;
	t->b = b;
	t->c = c;

	return t;
}

/**********/
void PTriangleClose(PTriangle *t)
{
	PFree(t);
}

/********/
/* PSTL */
/********/
static int PStlSorter(void *elem_a, void *elem_b)
{
	PTriangle *t0 = *(PTriangle **)elem_a;
   	PTriangle *t1 = *(PTriangle **)elem_b;
		
	float z0 = (t0->a.z + t0->b.z + t0->c.z)/3.0f;
	float z1 = (t1->a.z + t1->b.z + t1->c.z)/3.0f;

	if (z0 < z1) return -1;
    if (z0 > z1) return +1;

    return 0;
}

/**********/
void PStlLoad(char *file, PStl *stl)
{
	stl->data = PAlloc(1,sizeof(PArray));
	PArrayInit(stl->data);

	stl->proj = PAlloc(1,sizeof(PArray));
	PArrayInit(stl->proj);

	unsigned char *buffer = NULL;

	PBufferLoad(file,&buffer,NULL);

	unsigned char *buf = buffer + 0x50;//skip header

	unsigned int count = PBufferReadInt(&buf);

	for(unsigned int i=0;i<count;i++)
	{
		PVec3f normal = {0};
		normal.x = PBufferReadFloat(&buf);
		normal.y = PBufferReadFloat(&buf);
		normal.z = PBufferReadFloat(&buf);

		PVec3f a = {0};
		a.x = PBufferReadFloat(&buf);
		a.y = PBufferReadFloat(&buf);
		a.z = PBufferReadFloat(&buf);

		PVec3f b = {0};
		b.x = PBufferReadFloat(&buf);
		b.y = PBufferReadFloat(&buf);
		b.z = PBufferReadFloat(&buf);

		PVec3f c = {0};
		c.x = PBufferReadFloat(&buf);
		c.y = PBufferReadFloat(&buf);
		c.z = PBufferReadFloat(&buf);

		buf+=2;//short attrib value,not used

		PTriangle *t = PAlloc(1,sizeof(PTriangle));
		t->normal = normal;
		t->a = a;
		t->b = b;
		t->c = c;

		PArrayAdd(stl->data, t);

		PArrayAdd(stl->proj, PAlloc(1,sizeof(PTriangle)));
	}

	PFree(buffer);
}

/**********/
void PStlClose(PStl *stl)
{
	stl->posx = 0.0f;
	stl->posy = 0.0f;
	stl->posz = 0.0f;

	stl->rotx = 0.0f;
	stl->roty = 0.0f;
	stl->rotz = 0.0f;

	stl->scale = 0.0f;

	((PArray*)stl->proj)->elems_count = ((PArray*)stl->data)->elems_count;
	
	PArrayClose(((PArray*)stl->proj),PFree);

	PArrayClose(((PArray*)stl->data),PFree);

	PFree(stl->proj);
	PFree(stl->data);
}


/**********/
void PStlDrawWireframe(PStl *stl, Drawer func)
{
	PMat4f mat = {0};
	PMat4fInit(mat);

    PMat4fTranslateX(mat,stl->posx,mat);
    PMat4fTranslateY(mat,stl->posy,mat);
	PMat4fTranslateZ(mat,stl->posz,mat);

    PMat4fRotateX(mat,stl->rotx,mat);
    PMat4fRotateY(mat,stl->roty,mat); 
    PMat4fRotateZ(mat,stl->rotz,mat);          

    PMat4fScale(mat,stl->scale,mat);

	int count = 0;
	for(int i=0;i<((PArray*)stl->data)->elems_count;i++)
	{
		PTriangle *t = PArrayGet(((PArray*)stl->data),i);
		
		PTriangle *_t = ((PArray*)stl->proj)->elems[count];

		PVec3fxPMat4f(t->a,mat,&_t->a);
		PVec3fxPMat4f(t->b,mat,&_t->b);
		PVec3fxPMat4f(t->c,mat,&_t->c);

		count++;
	}

	((PArray*)stl->proj)->elems_count = count; //DANGER !!!
	
	PArrayPerform(((PArray*)stl->proj), func);
}

/**********/
void PStlDrawSolid(PStl *stl, Drawer func)
{
	PMat4f mat = {0};
	PMat4fInit(mat);

    PMat4fTranslateX(mat,stl->posx,mat);
    PMat4fTranslateY(mat,stl->posy,mat);
	PMat4fTranslateZ(mat,stl->posz,mat);

    PMat4fRotateX(mat,stl->rotx,mat);
    PMat4fRotateY(mat,stl->roty,mat); 
    PMat4fRotateZ(mat,stl->rotz,mat);          

    PMat4fScale(mat,stl->scale,mat);

	//retrieve drawable triangles
	int count = 0;
	for(int i=0;i<((PArray*)stl->data)->elems_count;i++)
	{
		PVec3f normal = {0};

		PTriangle *t = PArrayGet(((PArray*)stl->data),i);
		
		PVec3fxPMat4f(t->normal,mat,&normal);

		if(normal.z<0.0f){ continue; }

		PTriangle *_t = ((PArray*)stl->proj)->elems[count];

		PVec3fxPMat4f(t->a,mat,&_t->a);
		PVec3fxPMat4f(t->b,mat,&_t->b);
		PVec3fxPMat4f(t->c,mat,&_t->c);

		count++;
	}

	((PArray*)stl->proj)->elems_count = count; //DANGER !!!
	
	PArraySort(((PArray*)stl->proj), PStlSorter);

	PArrayPerform(((PArray*)stl->proj), func);
}

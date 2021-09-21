#ifndef PIXIE_BASE_H
#define PIXIE_BASE_H
#include "stdint.h"

#define M_PI 3.14159265358979323846

/********/
/* MATH */
/********/
void 		PSwap(int32_t *a, int32_t *b);
void 		PSwapf(float *a, float *b);
int32_t 	PMin(int32_t a, int32_t b);
float 		PMinf(float a, float b);
int32_t 	PMax(int32_t a, int32_t b);
float 		PMaxf(float a, float b);
int32_t 	PRand(int32_t min, int32_t max);
float 		PRandf(float min, float max);
int32_t 	PMap(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
float 		PMapf(float x, float in_min, float in_max, float out_min, float out_max);
int32_t 	PClamp(int32_t x, int32_t min, int32_t max);
float 		PClampf(float x, float min, float max);
float 		PRadiansf(float degrees);
float 		PDegreesf(float radians);

/**********/
/* MEMORY */
/**********/
void *PAlloc(size_t count, size_t size);
void *PRealloc(void* ptr, size_t size);
void  PFree(void *p);

/*********/
/* ARRAY */
/*********/
typedef struct PArray
{
	int32_t 	size;
	int32_t 	elems_count;
	void 	**elems;

} PArray;

typedef void (*Performer)(void *elem);
typedef int32_t (*Sorter)(void *elem_a, void *elem_b);

void PArrayInit(PArray *array);
void PArrayClose(PArray *array, void(*func)(void *elem) );
void PArrayAdd(PArray *array, void *elem);
void *PArrayRemove(PArray *array, int32_t index);
void *PArrayGet(PArray *array, int32_t index);
void PArrayPerform(PArray *array, Performer func );
void PArraySort(PArray *array, Sorter func);

/*********/
/* STACK */
/*********/
typedef PArray PStack;

void PStackInit(PStack *stack);
void PStackClose(PStack *stack, void(*func)(void *elem) );
void PStackPush(PStack *stack, void *elem);
void *PStackPop(PStack *stack);
void *PStackPeek(PStack *stack);

/*********/
/* QUEUE */
/*********/
typedef PArray PQueue;

void PQueueInit(PQueue *queue);
void PQueueClose(PQueue *queue, void(*func)(void *elem) );
void PQueueEnq(PQueue *queue, void *elem);
void *PQueueDeq(PQueue *queue);
void *PQueuePeek(PQueue *queue);

/***********/
/* BUFFERS */
/***********/
void PBufferLoad(char *file, uint8_t **buffer, int32_t *size);
uint8_t PBufferReadByte(uint8_t **buffer);
short PBufferReadShort(uint8_t **buffer);
int32_t PBufferReadInt(uint8_t **buffer);
float PBufferReadFloat(uint8_t **buffer);

/*********/
/* NOTES */
/*********/

//Example function for PArray Sorter prototype :
//
//  int32_t FUNC(void *elem_a, void *elem_b);
//	{
//		TYPE *a = *(TYPE **)elem_a;
//  	TYPE *b = *(TYPE **)elem_b;
//		
//		if (a->field < b->field) return -1;
//  	if (a->field > b->field) return +1;
//
//   	return 0;
//	}

#endif
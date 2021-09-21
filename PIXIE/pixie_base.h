#ifndef PIXIE_BASE_H
#define PIXIE_BASE_H

#define M_PI 3.14159265358979323846

/********/
/* MATH */
/********/
void PSwap(int *a, int *b);
void PSwapf(float *a, float *b);
int PMin(int a, int b);
float PMinf(float a, float b);
int PMax(int a, int b);
float PMaxf(float a, float b);
int PRand(int min, int max);
float PRandf(float min, float max);
int PMap(int x, int in_min, int in_max, int out_min, int out_max);
float PMapf(float x, float in_min, float in_max, float out_min, float out_max);
int PClamp(int x, int min, int max);
float PClampf(float x, float min, float max);
float PRadiansf(float degrees);
float PDegreesf(float radians);

/**********/
/* MEMORY */
/**********/
void *PAlloc(size_t count, size_t size);
void *PRealloc(void* ptr, size_t size);
void PFree(void *p);

/*********/
/* ARRAY */
/*********/
typedef struct PArray
{
	int 	size;
	int 	elems_count;
	void 	**elems;

} PArray;

typedef void (*Performer)(void *elem);
typedef int (*Sorter)(void *elem_a, void *elem_b);

void PArrayInit(PArray *array);
void PArrayClose(PArray *array, void(*func)(void *elem) );
void PArrayAdd(PArray *array, void *elem);
void *PArrayRemove(PArray *array, int index);
void *PArrayGet(PArray *array, int index);
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
void PBufferLoad(char *file, unsigned char **buffer, int *size);
unsigned char PBufferReadByte(unsigned char **buffer);
short PBufferReadShort(unsigned char **buffer);
int PBufferReadInt(unsigned char **buffer);
float PBufferReadFloat(unsigned char **buffer);

/*********/
/* NOTES */
/*********/

//Example function for PArray Sorter prototype :
//
//  int FUNC(void *elem_a, void *elem_b);
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
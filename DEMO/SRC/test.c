#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

//pixie
#include "../../PIXIE/pixie_exts.h"

/**********/
void PMatfPrint(PMatf mat)
{
    assert(mat.rows>0 && mat.cols>0 && mat.data!=NULL);

    printf("[rows=%d,cols=%d]\n",mat.rows,mat.cols);

    for(int32_t count=0;count<mat.rows*mat.cols;count++)
    {
        if(count>0 && count%mat.cols==0){ printf("\n"); }
        printf("%.2f", mat.data[count]);
        printf("\t");
    }

    printf("\n");
}

/**********/
int main(int argc,char **argv)
{
    float mata_data[6] = {5,8,-4,6,9,-5};

    PMatf mata = PMatfInit(2,3,mata_data);

    PMatfPrint(mata);
    
    PMatTranspose(mata,&mata);

    PMatfPrint(mata);

    PMatfClose(&mata);

    return 0;
}
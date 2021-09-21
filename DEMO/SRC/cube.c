//std
#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"

//pixie
#include "../../PIXIE/pixie_base.h"
#include "../../PIXIE/pixie_core.h"
#include "../../PIXIE/pixie_exts.h"

/**********/
void DrawQuad(PVec3f v0, PVec3f v1, PVec3f v2, PVec3f v3)
{
	PixieDrawLine((int)v0.x,(int)v0.y,(int)v1.x,(int)v1.y);
	PixieDrawLine((int)v1.x,(int)v1.y,(int)v2.x,(int)v2.y);
	PixieDrawLine((int)v2.x,(int)v2.y,(int)v3.x,(int)v3.y);
	PixieDrawLine((int)v3.x,(int)v3.y,(int)v0.x,(int)v0.y);
}

/**********/
int main(int argc, char **argv)
{
    PixieInit(800,600,0);

    int palette = 0;
    char msg[64] = {0};
    int fps = 0;
    int check_time = 0;

    PMat4f mat = {0};
    
    PVec3f v0 = (PVec3f){ -1.0f, +1.0f, -1.0f };
	PVec3f v1 = (PVec3f){ -1.0f, +1.0f, +1.0f };
	PVec3f v2 = (PVec3f){ +1.0f, +1.0f, +1.0f };
	PVec3f v3 = (PVec3f){ +1.0f, +1.0f, -1.0f };
	PVec3f v4 = (PVec3f){ -1.0f, -1.0f, -1.0f };
	PVec3f v5 = (PVec3f){ -1.0f, -1.0f, +1.0f };
	PVec3f v6 = (PVec3f){ +1.0f, -1.0f, +1.0f };
	PVec3f v7 = (PVec3f){ +1.0f, -1.0f, -1.0f };

	PVec3f vr0 = (PVec3f){ 0.0f,0.0f,0.0f };
	PVec3f vr1 = (PVec3f){ 0.0f,0.0f,0.0f };
	PVec3f vr2 = (PVec3f){ 0.0f,0.0f,0.0f };
	PVec3f vr3 = (PVec3f){ 0.0f,0.0f,0.0f };
	PVec3f vr4 = (PVec3f){ 0.0f,0.0f,0.0f };
	PVec3f vr5 = (PVec3f){ 0.0f,0.0f,0.0f };
	PVec3f vr6 = (PVec3f){ 0.0f,0.0f,0.0f };
	PVec3f vr7 = (PVec3f){ 0.0f,0.0f,0.0f };

    

    int font_size = 0;
    unsigned char *font_buffer = NULL;
    PixieFontLoad("RES/font1.bin",&font_buffer,&font_size);

    PixieFontSet(font_buffer,font_size);
    PixieFontSetScale(2);

    check_time = PixieTimeGet();

    int done = 0;
    while(!done)
    {
        while(PixieEventAvailable())
        {
            int type = PixieEventGetType();

            switch(type)
            {
                case P_QUIT:
                    done = 1;
                    break;
                
                case P_KEYPRESS:
                    if(PixieEventGetKey() == P_ESC) { done = 1; }
                    if(PixieEventGetKey() == P_F1) { PixiePaletteSet(++palette); }
                    if(PixieEventGetKey() == P_F2) { PixieToggleFullScreen(); }
                    if(PixieEventGetKey() == P_F3) { PixieToggleVsync(); }
                    break;

                default : break;
            }
        }

        //update
        PMat4fInit(mat);

        PMat4fTranslateX(mat,(float)PixieRenderGetWidth()/2.0f,mat);
        PMat4fTranslateY(mat,(float)PixieRenderGetHeight()/2.0f,mat);

        PMat4fRotateX(mat,(float)PixieTimeGet()/10.0f,mat);
        PMat4fRotateY(mat,(float)PixieTimeGet()/10.0f,mat); 
        PMat4fRotateZ(mat,(float)PixieTimeGet()/10.0f,mat);          

        PMat4fScale(mat,75.0f,mat);

        PVec3fxPMat4f(v0,mat,&vr0);
        PVec3fxPMat4f(v1,mat,&vr1);
        PVec3fxPMat4f(v2,mat,&vr2);
        PVec3fxPMat4f(v3,mat,&vr3);
        PVec3fxPMat4f(v4,mat,&vr4);
        PVec3fxPMat4f(v5,mat,&vr5);
        PVec3fxPMat4f(v6,mat,&vr6);
        PVec3fxPMat4f(v7,mat,&vr7);

        //fps
        fps++;
        if(PixieTimeGet() - check_time > 1000)
        {
            check_time = PixieTimeGet();
            sprintf(msg,"FPS : %d",fps);
            fps = 0;
        }

        //draw
        PixieRenderClear(P_COLOR_1);

        PixieColorSet(P_COLOR_0);
        
        DrawQuad(vr0,vr1,vr2,vr3);//front
		DrawQuad(vr4,vr5,vr6,vr7);//back
		DrawQuad(vr0,vr1,vr5,vr4);//left
		DrawQuad(vr3,vr2,vr6,vr7);//right
		DrawQuad(vr1,vr5,vr6,vr2);//top
		DrawQuad(vr0,vr4,vr7,vr3);//bot

        PixieDrawString("F1:CHANGE PALETTE\nF2:TOGGLE FULLSCREEN\nF3:TOGGLE VSYNC",0,0);

        PixieDrawString(msg,0,PixieRenderGetHeight()-PixieFontGetHeight());

        PixieRenderDraw();

    }

    PixieClose();

    PFree(font_buffer);

    return 0;
}
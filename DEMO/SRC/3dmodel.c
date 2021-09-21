//std
#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

//pixie
#include "../../PIXIE/pixie_base.h"
#include "../../PIXIE/pixie_core.h"
#include "../../PIXIE/pixie_exts.h"

//
void PTriangleDraw(void *elem)
{
    PTriangle *t = (PTriangle*)elem;
    
    PixieColorSet(P_COLOR_2);
    PixieDrawTri((int)t->a.x,(int)t->a.y,(int)t->b.x,(int)t->b.y,(int)t->c.x,(int)t->c.y);
}

//
void PTriangleDrawFill(void *elem)
{
    PTriangle *t = (PTriangle*)elem;
    
    PixieColorSet(P_COLOR_1);
    PixieDrawTriFill((int)t->a.x,(int)t->a.y,(int)t->b.x,(int)t->b.y,(int)t->c.x,(int)t->c.y);

    PixieColorSet(P_COLOR_2);
    PixieDrawTri((int)t->a.x,(int)t->a.y,(int)t->b.x,(int)t->b.y,(int)t->c.x,(int)t->c.y);

}

/**********/
int main(int argc, char **argv)
{
    int palette = 0;
    char msg[64] = {0};
    char info[256] = {0};
    int fps = 0;
    int check_time = 0;
    int wireframe = 1;
    int click = 0;
    int mx = 0;
    int my = 0;
    int mode = 0;
    int w = 0;

    char use[] ="MOUSE: rotate on axis\nWHEEL: scale model\nENTER: toggle wireframe\nSPACE: toggle animation\nCANC: reset";
    char notice[] = "F1:CHANGE PALETTE F2:TOGGLE FULLSCREEN F3:TOGGLE VSYNC";

    PixieInit(1024,768,0);

    PStl stl = {0};
    PStlLoad("RES/model.stl",&stl);

    stl.scale = 35.0f;    
    stl.posx = (float)PixieRenderGetWidth()/2.0f;
    stl.posy = (float)PixieRenderGetHeight()/2.0f;

    int font_size = 0;
    unsigned char *font_buffer = NULL;
    PixieFontLoad("RES/font1.bin",&font_buffer,&font_size);

    PixieFontSet(font_buffer,font_size);
    PixieFontSetScale(1);

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

                    if(PixieEventGetKey() == P_SPACE){ mode = !mode;}
                    if(PixieEventGetKey() == P_ENTER) { wireframe = !wireframe; } 
                    if(PixieEventGetKey() == P_CANCEL) 
                    { 
                        mode = 0;
                        stl.rotx = 0.0f;
                        stl.roty = 0.0f;
                        stl.rotz = 0.0f; 
                        stl.scale = 35.0f;
                    } 
                    break;

                case P_MOUSEPRESS:
                    click=1;
                    mx=PixieEventGetMouseX();
                    my=PixieEventGetMouseY();
                    break;

                case P_MOUSERELEASE:
                    click=0;
                    mx=PixieEventGetMouseX();
                    my=PixieEventGetMouseY();
                    break;

                case P_MOUSEMOVE:
                    if(click)
                    {
                        stl.roty += PixieEventGetMouseX() - mx;
                        stl.rotx -= PixieEventGetMouseY() - my;

                        mx = PixieEventGetMouseX();
                        my = PixieEventGetMouseY();
                    };
                    break;

                case P_MOUSEWHEEL:
                    if(PixieEventGetWheelDir()>0){ stl.scale += 2.5f; }
                    else { stl.scale -= 2.5f; }
                    break;

                default : break;
            }
        }

        //update model
        if(mode)
        {
            stl.rotx = (float)PixieTimeGet()/50.0f;
            stl.roty = (float)PixieTimeGet()/50.0f;
            stl.rotz = (float)PixieTimeGet()/50.0f;
        }

        //update fps
        fps++;
        if(PixieTimeGet() - check_time > 1000)
        {
            check_time = PixieTimeGet();
            sprintf(msg,"FPS : %d",fps);
            fps = 0;
        }

        //draw
        PixieRenderClear(P_COLOR_0);
        
        if(wireframe){ PStlDrawWireframe(&stl,PTriangleDraw);}
        else{ PStlDrawSolid(&stl,PTriangleDrawFill); }

        PixieColorSet(P_COLOR_3);

        PixieFontGetStringSize(use,&w,NULL);
        PixieDrawString(use,PixieRenderGetWidth()-w,0);

        PixieFontGetStringSize(notice,&w,NULL);
        PixieDrawString(notice,PixieRenderGetWidth()/2-w/2,0);

        PixieDrawString(msg,0,PixieRenderGetHeight()-PixieFontGetHeight());

        PArray *data_arr = (PArray *)stl.data;
        PArray *proj_arr = (PArray *)stl.proj;

        sprintf(info,"Triangles : %d\nDrawn: %d\nPosx: %.2f\nPosy: %.2f\nPosz: %.2f\nRotx: %.2f\nRoty: %.2f\nRotz: %.2f\nScale: %.2f",
        data_arr->elems_count,proj_arr->elems_count,stl.posx,stl.posy,stl.posz,stl.rotx,stl.roty,stl.rotz,stl.scale);

        PixieDrawString(info,0,0);

        PixieRenderDraw();

    }

    PFree(font_buffer);

    PStlClose(&stl);

    PixieClose();

    return 0;
}
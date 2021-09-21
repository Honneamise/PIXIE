//std
#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"

//pixie
#include "../../PIXIE/pixie_base.h"
#include "../../PIXIE/pixie_core.h"
#include "../../PIXIE/pixie_exts.h"

#define NUM_PARTICLES 512

typedef struct Particle
{
    PVec2f pos;
    PVec2f speed;
    int form;
    int color;

} Particle;

/**********/
int main(int argc, char **argv)
{
    int palette = 0;
    char msg[64] = {0};
    int fps = 0;
    int check_time = 0;

    Particle particles[NUM_PARTICLES] = {0};    

    PixieInit(800,600,0);

    int font_size = 0;
    unsigned char *font_buffer = NULL;
    PixieFontLoad("RES/font1.bin",&font_buffer,&font_size);

    PixieFontSet(font_buffer,font_size);
    PixieFontSetScale(3);

    check_time = PixieTimeGet();

    PixieTimeGetDelta();

    int done = 0;
    while(!done)
    {
        while(PixieEventAvailable())
        {
            if(PixieEventGetType() == P_QUIT) { done = 1; }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_ESC) { done = 1; }

            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_F1) { PixiePaletteSet(++palette); }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_F2) { PixieToggleFullScreen(); }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_F3) { PixieToggleVsync(); }
        }

        //update
        float dt = PixieTimeGetDelta();

        for(int i=0;i<NUM_PARTICLES;i++)
        {
            particles[i].speed = PVec2fAdd(particles[i].speed, PVec2fMul( ((PVec2f){ 0.0f, 75.0f }), dt) );

            particles[i].pos = PVec2fAdd(particles[i].pos, PVec2fMul(particles[i].speed, dt) );

            if(particles[i].pos.x<=0 || particles[i].pos.x>=PixieRenderGetWidth() || particles[i].pos.y<=0 || particles[i].pos.y>=PixieRenderGetHeight())
            {
                particles[i].pos = (PVec2f){ (float)PixieRenderGetWidth()/2, (float)PixieRenderGetHeight()/3}; 
                particles[i].speed = (PVec2f){ PRandf(-50.0f,+50.0f), PRandf(-75.0f,+75.0f)}; 
                particles[i].form = PRand(0,3);
                particles[i].color = PRand(1,3);
            }
        }

        fps++;
        if(PixieTimeGet() - check_time > 1000)
        {
            check_time = PixieTimeGet();
            sprintf(msg,"FPS : %d",fps);
            fps = 0;
        }

        //draw
        PixieRenderClear(P_COLOR_0);

        for(int i=0;i<NUM_PARTICLES;i++)
        {
            PixieColorSet(particles[i].color);

            switch(particles[i].form)
            {   case 0:
                PixieDrawCircle((int)particles[i].pos.x, (int)particles[i].pos.y, 5); 
                break;

                case 1:
                PixieDrawRect((int)particles[i].pos.x, (int)particles[i].pos.y, 10,10);
                break;

                case 2:
                PixieDrawCircleFill((int)particles[i].pos.x, (int)particles[i].pos.y, 5); 
                break;

                case 3:
                PixieDrawRectFill((int)particles[i].pos.x, (int)particles[i].pos.y, 10,10);
                break;

                default:
                break;
            }
        }

        PixieColorSet(P_COLOR_3);

        PixieDrawString("F1 - CHANGE PALETTE\nF2 - TOGGLE FULLSCREEN\nF3 - TOGGLE VSYNC",0,0);

        PixieDrawString(msg,0,PixieRenderGetHeight()-PixieFontGetHeight());

        PixieRenderDraw();

    }

    PixieClose();

    PFree(font_buffer);

    return 0;
}
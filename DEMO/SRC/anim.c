//std
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"

//pixie
#include "../../PIXIE/pixie_base.h"
#include "../../PIXIE/pixie_core.h"
#include "../../PIXIE/pixie_exts.h"

/**********/
int main(int argc, char **argv)
{
    int palette = 0;
    char msg[64] = {0};
    int fps = 0;
    int check_time = 0;

    PixieInit(800,600,0);

    int image_w = 0;
    int image_h = 0;
    unsigned char *image = NULL;
    PixieImageLoad("RES/sprite_image.ppm",&image,&image_w,&image_h);

    int mask_w = 0;
    int mask_h = 0;
    unsigned char *mask = NULL;
    PixieImageLoad("RES/sprite_mask.ppm",&mask,&mask_w,&mask_h);

    //we must know rows and cols
    int frame_num  = 0;
    int frame_time = 0;
    int frame_rows = 1;
    int frame_cols = 6;
    int frame_w    = image_w/frame_cols;
    int frame_h    = image_h/frame_rows;
    
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
        if(PixieTimeGet() - frame_time > 100)
        {
            frame_time = PixieTimeGet();
            frame_num++;
            if(frame_num>=frame_rows*frame_cols){ frame_num = 0; }

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

        int x = (frame_num%frame_cols)*frame_w;
        int y = (frame_num/frame_cols)*frame_h;

        PixieDrawImagePartMask(image, image_w, image_h, 
                                mask, mask_w, mask_h, 
                                x, y, frame_w, frame_h, 
                                PixieRenderGetWidth()/2-image_w/frame_cols/2, PixieRenderGetHeight()/2-image_h/frame_rows/2, 
                                P_COLOR_0);

        PixieColorSet(P_COLOR_3);

        PixieFontSetScale(3);

        PixieDrawString("F1 - CHANGE PALETTE\nF2 - TOGGLE FULLSCREEN\nF3 - TOGGLE VSYNC",0,0);

        PixieDrawString(msg,0,PixieRenderGetHeight()-PixieFontGetHeight());

        PixieRenderDraw();

    }

    PFree(image);

    PFree(mask);

    PixieClose();

    return 0;
}
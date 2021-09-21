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

    PixieInit(1280,720,0);

    int bufa_w,bufa_h = 0;
    unsigned char *bufa = NULL;
    PixieImageLoad("RES/image01.bmp",&bufa,&bufa_w,&bufa_h);

    int bufb_w,bufb_h = 0;
    unsigned char *bufb = NULL;
    PixieImageLoad("RES/image24.bmp",&bufb,&bufb_w,&bufb_h);

    int bufc_w,bufc_h = 0;
    unsigned char *bufc = NULL;
    PixieImageLoad("RES/image.pbm",&bufc,&bufc_w,&bufc_h);

    int bufd_w,bufd_h = 0;
    unsigned char *bufd = NULL;
    PixieImageLoad("RES/image.pgm",&bufd,&bufd_w,&bufd_h);

    int bufe_w,bufe_h = 0;
    unsigned char *bufe = NULL;
    PixieImageLoad("RES/image.ppm",&bufe,&bufe_w,&bufe_h);

    int font_size = 0;
    unsigned char *font_buffer = NULL;
    PixieFontLoad("RES/font1.bin",&font_buffer,&font_size);

    PixieFontSet(font_buffer,font_size);
    PixieFontSetScale(3);

    check_time = PixieTimeGet();

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
        fps++;
        if(PixieTimeGet() - check_time > 1000)
        {
            check_time = PixieTimeGet();
            sprintf(msg,"FPS : %d",fps);
            fps = 0;
        }

        //draw
        PixieRenderClear(P_COLOR_0);

        PixieDrawImage(bufa,bufa_w,bufa_h,0,0);

        PixieDrawImage(bufb,bufb_w,bufb_h,256,0);

        PixieDrawImage(bufc,bufc_w,bufc_h,512,0);

        PixieDrawImage(bufd,bufd_w,bufd_h,768,0);

        PixieDrawImage(bufe,bufe_w,bufe_h,1024,0);

        PixieColorSet(P_COLOR_3);

        PixieDrawString("BMP 1bit",0,bufa_h);
        PixieDrawString("BMP 24bit",256,bufa_h);
        PixieDrawString("PBM",512,bufa_h);
        PixieDrawString("PGM",768,bufa_h);
        PixieDrawString("PPM",1024,bufa_h);

        PixieDrawString("F1 - CHANGE PALETTE\nF2 - TOGGLE FULLSCREEN\nF3 - TOGGLE VSYNC",0,PixieRenderGetHeight()-PixieFontGetHeight()*4);

        PixieDrawString(msg,0,PixieRenderGetHeight()-PixieFontGetHeight());

        PixieRenderDraw();

    }

    PixieClose();

    PFree(bufa);
    PFree(bufb);
    PFree(bufc);
    PFree(bufd);
    PFree(bufe);

    PFree(font_buffer);

    return 0;
}
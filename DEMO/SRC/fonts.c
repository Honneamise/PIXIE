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

    int font_type = 0;
    int scale = 1;
    char str[64] = {0};

    PixieInit(1280,720,0);

    int font1_size = 0;
    unsigned char *font1 = NULL;
    PixieFontLoad("RES/font1.bin",&font1,&font1_size);

    int font2_size = 0;
    unsigned char *font2 = NULL;
    PixieFontLoad("RES/font2.bin",&font2,&font2_size);

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

            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_PLUS){ scale++; }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_MINUS)
            { 
                scale--; 
                if (scale<1) { scale = 1;}; 
            }

            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_SPACE)
            { 
                font_type++;
            }
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

        PixieColorSet(P_COLOR_3);

        PixieFontSetScale(scale);

        if((font_type%3)==0){ PixieFontSet(NULL,0); sprintf(str,"\"BUILTIN FONT\""); } 
        if((font_type%3)==1){ PixieFontSet(font1,font1_size); sprintf(str,"\"FONT1.BIN\""); } 
        if((font_type%3)==2){ PixieFontSet(font2,font2_size); sprintf(str,"\"FONT2.BIN\""); } 

        PixieDrawString(str,0,0);

        int counter = 0;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<32;j++)
            {
                PixieDrawChar(counter,j*PixieFontGetWidth(),(i+2)*PixieFontGetHeight());
                counter++;
            }
        }

        //
        PixieFontSet(NULL,0);

        PixieFontSetScale(2);

        PixieDrawString("NUMPAD +/- : CHANGE FONT SCALE\nSPACE : CHANGE FONT\nF1 : CHANGE PALETTE\nF2 : TOGGLE FULLSCREEN\nF3 : TOGGLE VSYNC",0,PixieRenderGetHeight()-PixieFontGetHeight()*7);

        PixieDrawString(msg,0,PixieRenderGetHeight()-PixieFontGetHeight());

        PixieRenderDraw();

    }

    PixieClose();

    PFree(font1);
    PFree(font2);

    return 0;
}
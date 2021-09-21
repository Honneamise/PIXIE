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
    float vol = 0.5f;
    char vol_str[24] = {0};
    sprintf(vol_str,"Volume:%.2f",vol);

    char nota[24] = {0};
    sprintf(nota,"...");

    PixieInit(800,600,0);

    int done = 0;
    while(!done)
    {
        while(PixieEventAvailable())
        {
            if(PixieEventGetType() == P_QUIT) { done = 1; }
            
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_ESC) { done = 1; }

            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_PLUS) { vol += 0.1f; vol = (vol>1.0f) ? 1.0f: vol; PixieAudioSetVolume(vol); sprintf(vol_str,"Volume:%.2f",vol);}
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_MINUS) { vol -= 0.1f; vol = (vol<0.0f) ? 0.0f: vol; PixieAudioSetVolume(vol); sprintf(vol_str,"Volume:%.2f",vol);}

            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetAsciiKey() == '1') { PixieAudioPlayTone(P_DO); sprintf(nota,"DO"); }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetAsciiKey() == '2') { PixieAudioPlayTone(P_RE); sprintf(nota,"RE"); }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetAsciiKey() == '3') { PixieAudioPlayTone(P_MI); sprintf(nota,"MI"); }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetAsciiKey() == '4') { PixieAudioPlayTone(P_FA); sprintf(nota,"FA"); }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetAsciiKey() == '5') { PixieAudioPlayTone(P_SO); sprintf(nota,"SOL");}
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetAsciiKey() == '6') { PixieAudioPlayTone(P_LA); sprintf(nota,"LA"); }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetAsciiKey() == '7') { PixieAudioPlayTone(P_SI); sprintf(nota,"SI"); }

            if(PixieEventGetType() == P_KEYRELEASE) { PixieAudioStopTone(); sprintf(nota,"..."); }
        }

        
        //draw
        PixieRenderClear(P_COLOR_0);

        PixieColorSet(P_COLOR_3);

        PixieFontSetScale(3);

        PixieDrawString("USE THE KEY 1-7 TO PLAY A NOTE\nUSE NUMPAD +/- TO ADJUST VOLUME",0,0);

        PixieDrawString(vol_str,0,PixieFontGetHeight()*3);

        PixieFontSetScale(10);

        int w = 0;
        int h = 0;
        PixieFontGetStringSize(nota,&w,&h);

        PixieDrawString(nota,PixieRenderGetWidth()/2-w/2,PixieRenderGetHeight()/2-h/2);

        PixieRenderDraw();

    }

    PixieClose();

    return 0;
}
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

    PixieInit(800,600,0);

    unsigned char *snd = NULL;
    int snd_size = 0;
    int snd_format = 0;
    int snd_rate = 0;
    PixieAudioLoadSound("RES/sound.wav", &snd, &snd_size, &snd_format, &snd_rate);

    int done = 0;
    while(!done)
    {
        while(PixieEventAvailable())
        {
            if(PixieEventGetType() == P_QUIT) { done = 1; }
            
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_ESC) { done = 1; }

            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_PLUS) { vol += 0.1f; vol = (vol>1.0f) ? 1.0f: vol; PixieAudioSetVolume(vol); sprintf(vol_str,"Volume:%.2f",vol);}
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_MINUS) { vol -= 0.1f; vol = (vol<0.0f) ? 0.0f: vol; PixieAudioSetVolume(vol); sprintf(vol_str,"Volume:%.2f",vol);}

            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_F1) { PixieAudioPlaySound(snd, snd_size, snd_format, snd_rate); };
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_F2) { PixieAudioLoopSound(snd, snd_size, snd_format, snd_rate); };
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_F3) { PixieAudioStopSound(); };
 
        }
  
        //draw
        PixieRenderClear(P_COLOR_0);

        PixieColorSet(P_COLOR_3);

        PixieFontSetScale(3);

        PixieDrawString("F1: play sound\nF2: loop sound\nF3: stop sound\nPLUS : increase volume\nMINUS: decrease volume",0,0);

        PixieDrawString(vol_str,0,PixieRenderGetHeight()/2);

        PixieDrawString("PIXIE PLAYBACK DEMO",0,PixieRenderGetHeight()-PixieFontGetHeight());

        PixieRenderDraw();

    }

    PFree(snd);

    PixieClose();

    return 0;
}
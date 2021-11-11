//std
#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"

//pixie
#include "../../PIXIE/pixie_base.h"
#include "../../PIXIE/pixie_core.h"
#include "../../PIXIE/pixie_gui.h"

/**********/
int main(int argc, char **argv)
{
    PixieInit(800,600,0);

    PixiePaletteSet(P_PALETTE_1_H);

    int32_t done = 0;

    char str[256] = {0};

    PixieGui gui  = PixieGuiInit("RES/font1.bin",2,P_COLOR_1, P_COLOR_2); 

    int32_t label = PixieGuiAddLabel(&gui, 100, 50, "multiline\nlabel\nexample");
    
    int32_t button = PixieGuiAddButton(&gui,100,150,"button");

    char edit_str[11] = "edit";
    int32_t edit = PixieGuiAddEdit(&gui, 100, 200, edit_str, 10);
    
    int radio_val = 0;
    int32_t radio = PixieGuiAddRadio(&gui, 100, 250, &radio_val);
    
    int toggle_val = 0;
    int32_t toggle = PixieGuiAddToggle(&gui, 100, 300, "toggle button", &toggle_val);
    
    int32_t slider_val = 50;
    int32_t slider = PixieGuiAddSlider(&gui, 100, 350, 10, &slider_val, 0, 100);

    int32_t selector_val = 0;
    char **selector_options = (char*[]){"selection 1","selection 2","selection 3"};
    int32_t selector = PixieGuiAddSelector(&gui, 100, 400, &selector_val, 3, selector_options);

    //main loop
    while(!done)
    {
        PixieGuiClearEvents(&gui);

        //events
        while(PixieEventAvailable())
        {
            if(PixieEventGetType() == P_QUIT) { done = 1; }
            if(PixieEventGetType() == P_KEYPRESS && PixieEventGetKey() == P_ESC) { done = 1; }

            PixieGuiHandleEvent(&gui);
        }

        //update
        if(label==PixieGuiGetActiveId(&gui))
        {
            
        }

        if(button==PixieGuiGetActiveId(&gui))
        {
	        sprintf(str,"Timestamp : %d\nbutton fired",PixieTimeGet());
        }

        if(edit==PixieGuiGetActiveId(&gui))
        {
            sprintf(str,"Timestamp : %d\nedit fired : %s",PixieTimeGet(),edit_str);
        }

	    if(radio==PixieGuiGetActiveId(&gui))
        {
            sprintf(str,"Timestamp : %d\nradio fired : %d",PixieTimeGet(),radio_val);
        }

	    if(toggle==PixieGuiGetActiveId(&gui))
        {
            sprintf(str,"Timestamp : %d\ntoggle fired : %d",PixieTimeGet(),toggle_val);
        }

        if(slider==PixieGuiGetActiveId(&gui))
        {
            sprintf(str,"Timestamp : %d\nslider fired : %d",PixieTimeGet(),slider_val);
        }

        if(selector==PixieGuiGetActiveId(&gui))
        {
            sprintf(str,"Timestamp : %d\nselector fired : %d",PixieTimeGet(),selector_val);
        }
        

        //draw
        PixieRenderClear(P_COLOR_0);

        PixieGuiDraw(&gui);

        PixieColorSet(P_COLOR_3);

        PixieDrawString(str, 400, 50);
        
        PixieRenderDraw();

    }

    PixieGuiClose(&gui);

    PixieClose();

    return 0;
}
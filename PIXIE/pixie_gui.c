/***********/
/* INCLUDE */
/***********/
//std
#include "string.h"
#include "stdio.h"//remove me
//pixie
#include "pixie_core.h"
#include "pixie_base.h"
#include "pixie_gui.h"

/***********/
/* DEFINES */
/***********/
//same from pixie_core.h (builtin font)
#define P_FONT_W            8
#define P_FONT_H            8
#define P_FONT_SCALE        1
#define P_FONT_SIZE         2048

#define P_LABEL         0
#define P_BUTTON        1
#define P_EDIT          2
#define P_RADIO		    3
#define P_TOGGLE	    4
#define P_SELECTOR	    5
#define P_SLIDER	    6

/************/
/* TYPEDEFS */
/************/
//label
typedef struct PLabel
{
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    char *text;    
}PLabel;

//button
typedef struct PButton
{
    int32_t over;

    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    char *text;    
}PButton;

//edit
typedef struct PEdit
{
    int32_t over;
    int32_t focus;

    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    char *text;    
    int32_t size;

}PEdit;

//radio
typedef struct PRadio
{
    int32_t over;
 
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    int32_t *val;

}PRadio;

//toggle
typedef struct PToggle
{
    int32_t over;
    int32_t *val;

    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    char *text;    
}PToggle;

//select
typedef struct PSelector
{
    int32_t over;
    int32_t *val;

    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    int32_t options_count;
    char **options;

}PSelector;

//slider
typedef struct PSlider
{
    int32_t over;

    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    int32_t *val;
    int32_t min_val;
    int32_t max_val;

    int32_t move;

}PSlider;

//widget
typedef struct PWidget
{
    int32_t id;
    int32_t type;
    int32_t enabled;

    union
    {
        PLabel      label;
        PButton     button;
        PEdit       edit;
        PRadio      radio;
        PToggle     toggle;
        PSelector   selector;
        PSlider     slider;
    };

}PWidget;


/*********/
/* LABEL */
/*********/
static void PLabelDraw(PixieGui *gui, PLabel *label)
{
    PixieColorSet(gui->bg_color);
    PixieDrawRectFill(label->x, label->y, label->w, label->h);

    PixieColorSet(gui->fg_color);
    PixieDrawString(label->text,label->x,label->y);
}

/**********/
static void PLabelEvent(PixieGui *gui, PLabel *label, int32_t id)
{
    
}

/**********/
/* BUTTON */
/**********/
static void PButtonDraw(PixieGui *gui, PButton *button)
{
    int32_t bgcolor = gui->bg_color;
    int32_t fgcolor = gui->fg_color;

    if(button->over)
    {
        bgcolor = gui->fg_color;
        fgcolor = gui->bg_color;
    }
    
    PixieColorSet(bgcolor);
    PixieDrawRectFill(button->x, button->y, button->w, button->h);

    PixieColorSet(fgcolor);
    PixieDrawString(button->text, button->x, button->y);
}

/**********/
static void PButtonEvent(PixieGui *gui, PButton *button, int32_t id)
{
    int32_t event = PixieEventGetType();

    switch(event)
    {
        case P_MOUSEMOVE:
            {
                int32_t x = PixieEventGetMouseX();
                int32_t y = PixieEventGetMouseY();
                button->over = x>button->x && x<button->x+button->w && y>button->y && y<button->y+button->h;
            }
            break;

        case P_MOUSEPRESS:
            if(button->over){ gui->active_id=id; };
            break;

        default: break;
    }
 
}

/********/
/* EDIT */
/********/
static void PEditDraw(PixieGui *gui, PEdit *edit)
{
    PixieColorSet(gui->bg_color);
    PixieDrawRectFill(edit->x, edit->y, edit->w, edit->h);

    PixieColorSet(gui->fg_color);
    PixieDrawString(edit->text, edit->x, edit->y);

    if( strlen(edit->text)<edit->size && edit->focus && (PixieTimeGet()%1000<500 ))
    {
        int32_t w = 0;
        PixieFontGetStringSize(edit->text, &w, NULL);

        PixieDrawChar(P_CHARSET_BLOCK, edit->x+w, edit->y);
        
    }
}

/**********/
static void PEditEvent(PixieGui *gui, PEdit *edit, int32_t id)
{
    int32_t event = PixieEventGetType();

    switch(event)
    {
        case P_MOUSEMOVE:
            {
                int32_t x = PixieEventGetMouseX();
                int32_t y = PixieEventGetMouseY();
                edit->over = x>edit->x && x<edit->x+edit->w && y>edit->y && y<edit->y+edit->h;
            }
            break;

        case P_MOUSEPRESS:
            if(edit->over){ edit->focus=1; }
            else{ edit->focus=0; }
            break;

        case P_KEYPRESS:
            if(!edit->focus){ break; }
            
            if(PixieEventGetKey()==P_ESC){ edit->focus=0; break; }
            if(PixieEventGetKey()==P_ENTER){ edit->focus=0; break; }

            int32_t len = (int32_t)strlen(edit->text);
            if(PixieEventGetKey()==P_BACKSPACE && len>0) { edit->text[len-1]=0; gui->active_id=id; break; }
            if(len<edit->size && PixieEventGetAsciiKey()!=0 ){ edit->text[len]=PixieEventGetAsciiKey(); gui->active_id=id; break; }
            break;

        default: break;
    }
 
}

/*********/
/* RADIO */
/*********/
static void PRadioDraw(PixieGui *gui, PRadio *radio)
{
    PixieColorSet(gui->bg_color);
    PixieDrawRectFill(radio->x, radio->y, radio->w, radio->h);

    if(*radio->val)
    {
        int32_t off = radio->w/4;

        PixieColorSet(gui->fg_color);
        PixieDrawRectFill(radio->x+off, radio->y+off, radio->w-off*2, radio->h-off*2);
    }
}

/**********/
static void PRadioEvent(PixieGui *gui, PRadio *radio, int32_t id)
{
    int32_t event = PixieEventGetType();

    switch(event)
    {
        case P_MOUSEMOVE:
            {
                int32_t x = PixieEventGetMouseX();
                int32_t y = PixieEventGetMouseY();
                radio->over = x>radio->x && x<radio->x+radio->w && y>radio->y && y<radio->y+radio->h;
            }
            break;

        case P_MOUSEPRESS:
            if(radio->over)
            { 
                *radio->val = !(*radio->val); 
                gui->active_id=id;
            }
            break;

        default: break;
    }
 
}

/**********/
/* TOGGLE */
/**********/
static void PToggleDraw(PixieGui *gui, PToggle *toggle)
{
    int32_t bgcolor = gui->bg_color;
    int32_t fgcolor = gui->fg_color;

    if(*toggle->val)
    {
        bgcolor = gui->fg_color;
        fgcolor = gui->bg_color;
    }
    
    PixieColorSet(bgcolor);
    PixieDrawRectFill(toggle->x, toggle->y, toggle->w, toggle->h);

    PixieColorSet(fgcolor);
    PixieDrawString(toggle->text, toggle->x, toggle->y);
}

/**********/
static void PToggleEvent(PixieGui *gui, PToggle *toggle, int32_t id)
{
    int32_t event = PixieEventGetType();

    switch(event)
    {
        case P_MOUSEMOVE:
            {
                int32_t x = PixieEventGetMouseX();
                int32_t y = PixieEventGetMouseY();
                toggle->over = x>toggle->x && x<toggle->x+toggle->w && y>toggle->y && y<toggle->y+toggle->h;
            }
            break;

        case P_MOUSEPRESS:
            if(toggle->over)
            {
                gui->active_id=id; 
                *toggle->val = !(*toggle->val); 
            }
            break;

        default: break;
    }
 
}

/**********/
/* SELECT */
/**********/
static void PSelectorDraw(PixieGui *gui, PSelector *selector)
{
    int32_t h = selector->h / selector->options_count;

    PixieColorSet(gui->bg_color);
    PixieDrawRectFill(selector->x, selector->y, selector->w, selector->h);

    for(int32_t i=0;i<selector->options_count;i++)
    {
        if(i==*selector->val)
        {
            PixieColorSet(gui->fg_color);
            PixieDrawRectFill(selector->x, selector->y+h*i, selector->w, h);

            PixieColorSet(gui->bg_color);
            PixieDrawString(selector->options[i], selector->x, selector->y+h*i);

        }
        else
        {
            PixieColorSet(gui->fg_color);
            PixieDrawString(selector->options[i], selector->x, selector->y+h*i);
        }
    }
    
}

/**********/
static void PSelectEvent(PixieGui *gui, PSelector *selector, int32_t id)
{
    int32_t event = PixieEventGetType();

    switch(event)
    {
        case P_MOUSEMOVE:
            {
                int32_t x = PixieEventGetMouseX();
                int32_t y = PixieEventGetMouseY();
                selector->over = x>selector->x && x<selector->x+selector->w && y>selector->y && y<selector->y+selector->h;
            }
            break;

        case P_MOUSEPRESS:
            if(selector->over)
            {
                gui->active_id=id;

                int32_t y = PixieEventGetMouseY();
                
                *selector->val = (y - selector->y) / ( selector->h / selector->options_count); 
            }
            break;

        default: break;
    }
 
}

/**********/
/* SLIDER */
/**********/
static void PSliderDraw(PixieGui *gui,PSlider *slider)//TODO : refactor
{
    PixieColorSet(gui->bg_color);
    PixieDrawRectFill(slider->x, slider->y, slider->w, slider->h);

    PixieColorSet(gui->fg_color);
    int32_t x = PMap(*slider->val, slider->min_val, slider->max_val, slider->x+slider->h/2, slider->x+slider->w-slider->h/2);

    PixieDrawRectFill(x-slider->h/4, slider->y+slider->h/4, slider->h/2, slider->h/2);
}

/**********/
static void PSliderEvent(PixieGui *gui, PSlider *slider, int32_t id)//TODO : refactor
{
    int32_t event = PixieEventGetType();

    switch(event)
    {
        case P_MOUSEMOVE:
            {
                int32_t x = PixieEventGetMouseX();
                int32_t y = PixieEventGetMouseY();
                    
                int32_t _x = PMap(*slider->val,slider->min_val,slider->max_val,slider->x+slider->h/2,slider->x+slider->w-slider->h/2);

                slider->over = x>_x-slider->h/4 && x<_x-slider->h/4+slider->h/2 && 
                                y>slider->y+slider->h/4 && y<slider->y+slider->h/4+slider->h/2;
                
                if(slider->move)
                {
                    *slider->val = PMap(x,slider->x+slider->h/4,slider->x+slider->w-slider->h/4,slider->min_val,slider->max_val);
                    
                    *slider->val = PMax(*slider->val,slider->min_val);
                    *slider->val = PMin(*slider->val,slider->max_val);                    
                    
                    gui->active_id=id;
                }
            }
            break;

        case P_MOUSEPRESS:
            if(slider->over)
            { 
                slider->move = 1;
            }
            break;

        case P_MOUSERELEASE:
            if(slider->move)
            { 
                slider->move = 0;
            }
            break;

        default: break;
    }
 
}

/************/
/* PixieGui */
/************/
PixieGui PixieGuiInit(char *font_file, int32_t font_scale, int32_t bg_color, int32_t fg_color)
{
    PixieGui gui = {0};

    if(font_file!=NULL)
    {
        PixieFontLoad(font_file,&gui.font,&gui.font_size);
    }
    else
    {
        gui.font = NULL;
        gui.font_size = 0;
    }

    gui.font_scale = font_scale;

    gui.bg_color = bg_color;

    gui.fg_color = fg_color;

    gui.active_id = -1;

    gui.id_generator = 0;

    PArrayInit(&gui.widgets);

    return gui;
}

/**********/
void PixieGuiClose(PixieGui *gui)
{
    for(int32_t i=gui->widgets.elems_count-1;i>=0;i--)
    {
        PWidget *widget = PArrayRemove(&gui->widgets,i);

        PFree(widget);
    }

    if(gui->font){ PFree(gui->font); }

    PArrayClose(&gui->widgets,NULL);
}

/**********/
void PixieGuiClearEvents(PixieGui *gui)
{
    gui->active_id = -1;
}

/**********/
void PixieGuiHandleEvent(PixieGui *gui)
{
    for(int32_t i=0;i<gui->widgets.elems_count;i++)
    {
        PWidget *widget = PArrayGet(&gui->widgets,i);

        if(!widget || !widget->enabled){ continue; }

        switch(widget->type)
        {
            case P_LABEL:
                PLabelEvent(gui,&widget->label,widget->id);
                break;

            case P_BUTTON:
                PButtonEvent(gui,&widget->button,widget->id);
                break;

            case P_EDIT:
                PEditEvent(gui,&widget->edit,widget->id);
                break;

            case P_RADIO:
                PRadioEvent(gui,&widget->radio,widget->id);
                break;
		
	        case P_TOGGLE:
                PToggleEvent(gui,&widget->toggle,widget->id);
                break;

            case P_SELECTOR:
                PSelectEvent(gui,&widget->selector,widget->id);
                break;

            case P_SLIDER:
                PSliderEvent(gui,&widget->slider,widget->id);
                break;

            default:
                break;
        }
    }
}


/**********/
void PixieGuiDraw(PixieGui *gui)
{
    PixieFontSet(gui->font,gui->font_size);

    PixieFontSetScale(gui->font_scale);

    for(int32_t i=0;i<gui->widgets.elems_count;i++)
    {
        PWidget *widget = PArrayGet(&gui->widgets,i);

        if(!widget || !widget->enabled){ continue; }

        switch(widget->type)
        {
            case P_LABEL:
                PLabelDraw(gui,&widget->label);
                break;

            case P_BUTTON:
                PButtonDraw(gui,&widget->button);
                break;

            case P_EDIT:
                PEditDraw(gui,&widget->edit);
                break;
	    
            case P_RADIO:
                PRadioDraw(gui,&widget->radio);
                break;

	        case P_TOGGLE:
                PToggleDraw(gui,&widget->toggle);
                break;

            case P_SELECTOR:
                PSelectorDraw(gui,&widget->selector);
                break;

            case P_SLIDER:
                 PSliderDraw(gui,&widget->slider);
                 break;

            default:
                break;
        }
    }
}

/**********/
int32_t PixieGuiGetActiveId(PixieGui *gui)
{
    return gui->active_id;
}

/**********/
void PixieGuiRemoveWidget(PixieGui *gui, int32_t id)
{

    for(int32_t i=0;i<gui->widgets.elems_count;i++)
    {
        PWidget *widget = PArrayGet(&gui->widgets,i);

        if(widget && widget->id==id)
        {
            PFree(widget);
            gui->widgets.elems[i] = NULL;
        }
    }
}

/**********/
int32_t PixieGuiAddLabel(PixieGui *gui, int32_t x, int32_t y, char *text)
{
    PLabel label = {0};

    label.x = x;
    label.y = y;

    PixieFontSet(gui->font,gui->font_size);
    PixieFontSetScale(gui->font_scale);
    PixieFontGetStringSize(text,&label.w,&label.h);

    label.text = text;

    PWidget *widget = PAlloc(1,sizeof(PWidget));

    widget->id = gui->id_generator++;
    widget->type = P_LABEL;
    widget->enabled = 1;
    widget->label = label;

    PArrayAdd(&gui->widgets,widget);

    return widget->id;
}

/**********/
int32_t PixieGuiAddButton(PixieGui *gui, int32_t x, int32_t y, char *text)
{
    PButton button = {0};

    button.x = x;
    button.y = y;
    
    PixieFontSet(gui->font,gui->font_size);
    PixieFontSetScale(gui->font_scale);
    PixieFontGetStringSize(text,&button.w,&button.h);

    button.text = text;

    PWidget *widget = PAlloc(1,sizeof(PWidget));

    widget->id = gui->id_generator++;
    widget->type = P_BUTTON;
    widget->enabled = 1;
    widget->button = button;

    PArrayAdd(&gui->widgets,widget);

    return widget->id;
}

/**********/
int32_t PixieGuiAddEdit(PixieGui *gui, int32_t x, int32_t y, char *text, int32_t size)
{
    PEdit edit = {0};

    edit.x = x;
    edit.y = y;

    PixieFontSet(gui->font,gui->font_size);
    PixieFontSetScale(gui->font_scale);

    edit.w = size * PixieFontGetWidth();
    edit.h = PixieFontGetHeight();

    edit.text = text;
    edit.size = size;

    PWidget *widget = PAlloc(1,sizeof(PWidget));

    widget->id = gui->id_generator++;
    widget->type = P_EDIT;
    widget->enabled = 1;
    widget->edit = edit;

    PArrayAdd(&gui->widgets,widget);

    return widget->id;
}

/**********/
int32_t PixieGuiAddRadio(PixieGui *gui, int32_t x, int32_t y, int *val)
{
    PRadio radio = {0};

    radio.x = x;
    radio.y = y;

    PixieFontSet(gui->font,gui->font_size);
    PixieFontSetScale(gui->font_scale);

    radio.w = PixieFontGetWidth();
    radio.h = PixieFontGetHeight();
    
    radio.val = val;

    PWidget *widget = PAlloc(1,sizeof(PWidget));

    widget->id = gui->id_generator++;
    widget->type = P_RADIO;
    widget->enabled = 1;
    widget->radio = radio;

    PArrayAdd(&gui->widgets,widget);

    return widget->id;
}

/**********/
int32_t PixieGuiAddToggle(PixieGui *gui, int32_t x, int32_t y, char *text, int *val)
{
    PToggle toggle = {0};

    toggle.x = x;
    toggle.y = y;
    
    PixieFontSet(gui->font,gui->font_size);
    PixieFontSetScale(gui->font_scale);
    PixieFontGetStringSize(text,&toggle.w,&toggle.h);

    toggle.val = val;
    toggle.text = text;

    PWidget *widget = PAlloc(1,sizeof(PWidget));

    widget->id = gui->id_generator++;
    widget->type = P_TOGGLE;
    widget->enabled = 1;
    widget->toggle = toggle;

    PArrayAdd(&gui->widgets,widget);

    return widget->id;
}

/**********/
int32_t PixieGuiAddSelector(PixieGui *gui, int32_t x, int32_t y, int32_t *val, int32_t options_count, char **options)
{
    PSelector selector = {0};

    selector.x = x;
    selector.y = y;

    selector.val = val;
    selector.options_count = options_count;
    selector.options = options;

    PixieFontSet(gui->font,gui->font_size);
    PixieFontSetScale(gui->font_scale);

    selector.w = (int32_t)strlen(options[0]) * PixieFontGetWidth();  

    for(int32_t i=1;i<options_count;i++)
    {
        int32_t len = (int32_t)strlen(options[i]) * PixieFontGetWidth();
        
        selector.w = PMax(selector.w,len); 
    }

    selector.h = options_count * PixieFontGetHeight();

    PWidget *widget = PAlloc(1,sizeof(PWidget));

    widget->id = gui->id_generator++;
    widget->type = P_SELECTOR;
    widget->enabled = 1;
    widget->selector = selector;

    PArrayAdd(&gui->widgets,widget);

    return widget->id;
}

/**********/
int32_t PixieGuiAddSlider(PixieGui *gui, int32_t x, int32_t y, int32_t size, int32_t *val, int32_t min_val, int32_t max_val)
{
    PSlider slider = {0};

    slider.x = x;
    slider.y = y;

    PixieFontSet(gui->font,gui->font_size);
    PixieFontSetScale(gui->font_scale);

    slider.w = size * PixieFontGetWidth();
    slider.h = PixieFontGetHeight();
    
    slider.val = val;
    slider.min_val = min_val;
    slider.max_val = max_val;

    PWidget *widget = PAlloc(1,sizeof(PWidget));

    widget->id = gui->id_generator++;
    widget->type = P_SLIDER;
    widget->enabled = 1;
    widget->slider = slider;

    PArrayAdd(&gui->widgets,widget);

    return widget->id;
}
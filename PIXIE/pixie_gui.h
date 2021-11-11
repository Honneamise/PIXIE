#ifndef PIXIE_GUI_H
#define PIXIE_GUI_H

#include "pixie_base.h"

/************/
/* PIXIEGUI */
/************/
typedef struct PixieGui
{
    PFont font;

    int32_t font_size;

    int32_t font_scale;

    int32_t bg_color;

    int32_t fg_color;
    
    int32_t active_id;

    int32_t id_generator;

    PArray widgets;

}PixieGui;

/**********/
PixieGui PixieGuiInit(char *font_file, int32_t font_scale, int32_t bg_color, int32_t fg_color);

void PixieGuiClose(PixieGui *gui);

void PixieGuiClearEvents(PixieGui *gui);

void PixieGuiHandleEvent(PixieGui *gui);

void PixieGuiDraw(PixieGui *gui);

int32_t PixieGuiGetActiveId(PixieGui *gui);

void PixieGuiRemoveWidget(PixieGui *gui, int32_t id);

int32_t PixieGuiAddLabel(PixieGui *gui, int32_t x, int32_t y, char *text);

int32_t PixieGuiAddButton(PixieGui *gui, int32_t x, int32_t y, char *text);

int32_t PixieGuiAddEdit(PixieGui *gui, int32_t x, int32_t y, char *text, int32_t size);

int32_t PixieGuiAddRadio(PixieGui *gui, int32_t x, int32_t y, int *val);

int32_t PixieGuiAddToggle(PixieGui *gui, int32_t x, int32_t y, char *text, int32_t *val);

int32_t PixieGuiAddSelector(PixieGui *gui, int32_t x, int32_t y, int32_t *val, int32_t options_count, char **options);

int32_t PixieGuiAddSlider(PixieGui *gui, int32_t x, int32_t y, int32_t size, int32_t *val, int32_t min_val, int32_t max_val);

#endif

#ifndef PIXIE_CORE_H
#define PIXIE_CORE_H
#include "stdint.h"

/**************/
/* DATA TYPES */
/**************/
typedef uint8_t *PImage;
typedef uint8_t *PFont;
typedef uint8_t *PSound;

/********/
/* CORE */
/********/
void PixieDebug();

void PixieInit(int32_t w, int32_t h, int32_t screen);

void PixieClose();

void PixieToggleFullScreen();

void PixieToggleVsync();

/**********/
/* RENDER */
/**********/
void PixieRenderClear(int32_t col);

void PixieRenderDraw();

int32_t PixieRenderGetWidth();

int32_t PixieRenderGetHeight();

void PixieRenderScreenshot(char *file);

/*********************/
/* COLOR AND PALETTE */
/*********************/
void PixieColorSet(int32_t col);

void PixiePaletteSet(int32_t pal);

/***********/
/* DRAWING */
/***********/
void PixieDrawPoint(int32_t x, int32_t y);

void PixieDrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1);

void PixieDrawTri(int32_t ax, int32_t ay, int32_t bx, int32_t by, int32_t cx, int32_t cy);

void PixieDrawTriFill(int32_t ax, int32_t ay, int32_t bx, int32_t by, int32_t cx, int32_t cy);

void PixieDrawRect(int32_t x, int32_t y, int32_t w, int32_t h);

void PixieDrawRectFill(int32_t x, int32_t y, int32_t w, int32_t h);

void PixieDrawCircle(int32_t x, int32_t y, int32_t r);

void PixieDrawCircleFill(int32_t x, int32_t y, int32_t r);

void PixieDrawImage(PImage buffer, int32_t w, int32_t h, int32_t x, int32_t y);

void PixieDrawImagePart(PImage buffer, int32_t bw, int32_t bh, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t x, int32_t y);

void PixieDrawImageMask(PImage buffer, int32_t bw, int32_t bh, PImage mask, int32_t mw, int32_t mh, int32_t x, int32_t y, int32_t col);

void PixieDrawImagePartMask(PImage buffer, int32_t bw, int32_t bh, PImage mask, int32_t mw, int32_t mh, int32_t sx, int32_t sy, int32_t sw, int32_t sh, int32_t x, int32_t y, int32_t col);

void PixieDrawChar(char c, int32_t x, int32_t y);

void PixieDrawString(char *s, int32_t x, int32_t y);

/**********/
/* IMAGES */
/**********/
void PixieImageLoad(char *file, PImage *buffer, int32_t *w, int32_t *h);

int32_t PixieImageIntersect(int32_t ax, int32_t ay, int32_t aw, int32_t ah, int32_t bx, int32_t by, int32_t bw, int32_t bh, int32_t *rx, int32_t *ry, int32_t *rw, int32_t *rh);

int32_t PixieImageOverlap(PImage a, int32_t ax, int32_t ay, int32_t aw, int32_t ah, PImage b, int32_t bx, int32_t by, int32_t bw, int32_t bh, int32_t col);

/*********/
/* FONTS */
/*********/
void PixieFontLoad(char *file, PFont *buffer, int32_t *size);

void PixieFontSet(PFont buffer, int32_t size);

void PixieFontSetScale(int32_t scale);

int32_t PixieFontGetWidth();

int32_t PixieFontGetHeight();

void PixieFontGetStringSize(char *s, int32_t *w, int32_t *h);

/*********/
/* AUDIO */
/*********/
void PixieAudioLoadSound(char *file, PSound *buffer, int32_t *size, int32_t *format, int32_t *rate);

void PixieAudioPlaySound(PSound buffer, int32_t size, int32_t format, int32_t rate);

void PixieAudioLoopSound(PSound buffer, int32_t size, int32_t format, int32_t rate);

void PixieAudioStopSound();

void PixieAudioPlayTone(int32_t tone);

void PixieAudioStopTone();

void PixieAudioSetVolume(float vol);

/********/
/* TIME */
/********/
int32_t PixieTimeGet();

float PixieTimeGetDelta();

/**********/
/* EVENTS */
/**********/
void PixieEventClearQueue();

int32_t PixieEventAvailable();

int32_t PixieEventGetType();

int32_t PixieEventGetKey();

int32_t PixieEventGetAsciiKey();

int32_t PixieEventGetMouseX();

int32_t PixieEventGetMouseY();

int32_t PixieEventGetMouseButton();

int32_t PixieEventGetWheelDir();

/***********/
/* DEFINES */
/***********/
//colors
#define P_NUM_COLORS    4

#define P_COLOR_0       0
#define P_COLOR_1       1
#define P_COLOR_2       2
#define P_COLOR_3       3

//palettes
#define P_NUMPALETTES       7

#define P_PALETTE_MONO      0
#define P_PALETTE_0_L       1
#define P_PALETTE_0_H       2
#define P_PALETTE_1_L       3
#define P_PALETTE_1_H       4
#define P_PALETTE_2_L       5
#define P_PALETTE_2_H       6

//charset
#define P_CHARSET_BLANK     0x00
#define P_CHARSET_BLOCK     0xDB

//tones
#define P_NUM_TONES        7

#define P_DO               0
#define P_RE               1
#define P_MI               2
#define P_FA               3
#define P_SO               4
#define P_LA               5
#define P_SI               6

//events type
#define P_NONE		        0 
#define P_QUIT		        1
#define P_KEYPRESS 	        2
#define P_KEYRELEASE	    3  
#define P_MOUSEMOVE	        4
#define P_MOUSEPRESS	    5
#define P_MOUSERELEASE	    6
#define P_MOUSEWHEEL        7

//mouse button
#define P_BUT_LEFT	        0
#define P_BUT_MID	        1
#define P_BUT_RIGHT         2

//mouse wheel
#define P_WHEEL_UP          0
#define P_WHEEL_DOWN        1

//common keys
#define P_ESC		        0x1B
#define P_ENTER		        0x0D
#define P_CANCEL	        0x2E
#define P_BACKSPACE	        0x08
#define P_SPACE		        0x20

//numpad
#define P_PLUS	            0x6B
#define P_MINUS	            0x6D

//arrow keys
#define P_UP	            0x26
#define P_DOWN	            0x28
#define P_LEFT	            0x25
#define P_RIGHT	            0x27

//function key
#define P_F1	            0x70 
#define P_F2	            0x71
#define P_F3	            0x72
#define P_F4	            0x73

#endif
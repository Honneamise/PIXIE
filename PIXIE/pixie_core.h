#ifndef PIXIE_CORE_H
#define PIXIE_CORE_H

/**************/
/* DATA TYPES */
/**************/
typedef unsigned char *PImage;
typedef unsigned char *PFont;
typedef unsigned char *PSound;

/*********/
/* PIXIE */
/*********/
void PixieDebug();

void PixieInit(int w, int h, int screen);

void PixieClose();

void PixieToggleFullScreen();

void PixieToggleVsync();

/**********/
/* RENDER */
/**********/
void PixieRenderClear(int col);

void PixieRenderDraw();

int PixieRenderGetWidth();

int PixieRenderGetHeight();

/*********************/
/* COLOR AND PALETTE */
/*********************/
void PixieColorSet(int col);

void PixiePaletteSet(int pal);

/***********/
/* DRAWING */
/***********/
void PixieDrawPoint(int x, int y);

void PixieDrawLine(int x0, int y0, int x1, int y1);

void PixieDrawTri(int ax, int ay, int bx, int by, int cx, int cy);

void PixieDrawTriFill(int ax, int ay, int bx, int by, int cx, int cy);

void PixieDrawRect(int x, int y, int w, int h);

void PixieDrawRectFill(int x, int y, int w, int h);

void PixieDrawCircle(int x, int y, int r);

void PixieDrawCircleFill(int x, int y, int r);

void PixieDrawImage(PImage buffer, int w, int h, int x, int y);

void PixieDrawImagePart(PImage buffer, int bw, int bh, int sx, int sy, int sw, int sh, int x, int y);

void PixieDrawImageMask(PImage buffer, int bw, int bh, PImage mask, int mw, int mh, int x, int y, int col);

void PixieDrawImagePartMask(PImage buffer, int bw, int bh, PImage mask, int mw, int mh, int sx, int sy, int sw, int sh, int x, int y, int col);

void PixieDrawChar(char c, int x, int y);

void PixieDrawString(char *s, int x, int y);

/**********/
/* IMAGES */
/**********/
void PixieImageLoad(char *file, PImage *buffer, int *w, int *h);

int PixieImageIntersect(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh, int *rx, int *ry, int *rw, int *rh);

int PixieImageOverlap(PImage a, int ax, int ay, int aw, int ah, PImage b, int bx, int by, int bw, int bh, int col);

/*********/
/* FONTS */
/*********/
void PixieFontLoad(char *file, PFont *buffer, int *size);

void PixieFontSet( PFont buffer, int size);

void PixieFontSetScale(int scale);

int PixieFontGetWidth();

int PixieFontGetHeight();

void PixieFontGetStringSize(char *s, int *w, int *h);

/*********/
/* AUDIO */
/*********/
void PixieAudioLoadSound(char *file,  PSound *buffer, int *size, int *format, int *rate);

void PixieAudioPlaySound(PSound buffer, int size, int format, int sample_rate);

void PixieAudioLoopSound(PSound buffer, int size, int format, int sample_rate);

void PixieAudioStopSound();

void PixieAudioPlayTone(int tone);

void PixieAudioStopTone();

void PixieAudioSetVolume(float vol);

/********/
/* TIME */
/********/
int PixieTimeGet();

float PixieTimeGetDelta();

/**********/
/* EVENTS */
/**********/
int PixieEventAvailable();

int PixieEventGetType();

int PixieEventGetKey();

int PixieEventGetAsciiKey();

int PixieEventGetMouseX();

int PixieEventGetMouseY();

int PixieEventGetMouseButton();

int PixieEventGetWheelDir();

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
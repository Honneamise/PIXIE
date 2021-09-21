/***********/
/* INCLUDE */
/***********/
#define _CRT_SECURE_NO_WARNINGS

//debug
#if defined(_MSC_VER) && defined(_DEBUG)
#include "crtdbg.h"
#endif

//windows
#include "windows.h"
#include "windowsx.h"

//opengl
#include "OPENGL/INCLUDE/gl.h"
#include "OPENGL/INCLUDE/glext.h"
#include "OPENGL/INCLUDE/wglext.h"
#include "OPENGL/INCLUDE/khrplatform.h"

//openal
#include "OPENAL/INCLUDE/al.h"
#include "OPENAL/INCLUDE/alc.h"

//std
#include "stdio.h"

//math
#define _USE_MATH_DEFINES
#include "math.h"

//time
#include "time.h"

//pixie
#include "pixie_base.h"
#include "pixie_core.h"

/***********/
/* DEFINES */
/***********/
#define P_VERSION "PIXIE 1.0"//LOL

//endianness
#define P_BIG_ENDIAN        0
#define P_LITTLE_ENDIAN     1

//window class name
#define P_WND_CLASS "PIXIE_WND_CLASS"

//opengl
#define P_OPENGL_MAJOR         4
#define P_OPENGL_MINOR         5

#define P_PALETTE_SIZE         12
#define P_PALETTE_LOCATION     "Palette"

#define P_TEXTURE_NUM_VERTICES 16

//openal
#define P_DEFAULT_VOLUME      0.5f
#define P_TONE_SAMPLE_RATE    44100

//ppm image filter
#define P_IMAGE_R_FILTER 0.212671f
#define P_IMAGE_G_FILTER 0.715160f
#define P_IMAGE_B_FILTER 0.072169f

//charset
#define P_CHARSET_SIZE         256
#define P_CHARSET_UNKNOW       0x3F

#define P_CHARSET_SP           0x20
#define P_CHARSET_TAB          0x09
#define P_CHARSET_CR           0x0D
#define P_CHARSET_LF           0x0A
#define P_CHARSET_HASH         0x23

//ibm cga font 8x8
#define P_FONT_W               8
#define P_FONT_H               8
#define P_FONT_SCALE           1
#define P_FONT_SIZE            2048

/************/
/* TYPEDEFS */
/************/
typedef struct Pixie
{
    int width;
    int height;
    int client_w;
    int client_h;
    int viewport_w;
    int viewport_h;

    unsigned char *buffer;

    int screen;

    HDC hdc;
    HWND hwnd;
    WINDOWPLACEMENT hwndPlacement;
    HGLRC hglrc;

    GLuint vao;
    GLuint vbo;
    GLuint vert_shader;
    GLuint frag_shader;
    GLuint prog_shader;
    GLuint texture;

    ALCdevice *adev;
    ALCcontext *actx;
    ALuint asrc_snd;
    ALuint asrc_tone;
    ALuint asnd;
    ALuint atones[P_NUM_TONES];

    unsigned char color;

    int font_w;
    int font_h;
    int font_scale;
    unsigned char *font;

    LARGE_INTEGER freq;
    LARGE_INTEGER timer;
    int delta;

    int event;
	int key;
    int ascii_key;
	int mousex;
	int mousey;
	int button;
	int wheel_dir;
    
} Pixie;

/**********/
/* STATIC */
/**********/
static Pixie *pixie = NULL;

static const float P_PALETTES[] = 
{
	0x00, 0x00, 0x00,//PALETTE_MONO
	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,

	0x00, 0x00, 0x00,//PALETTE_0_L
	0x00, 0xAA, 0x00,
	0xAA, 0x00, 0x00,
	0xAA, 0x55, 0x00,

	0x00, 0x00, 0x00,//PALETTE_0_H
	0x55, 0xFF, 0x55,
	0xFF, 0x55, 0x55,
	0xFF, 0xFF, 0x55,

	0x00, 0x00, 0x00,//PALETTE_1_L
	0x00, 0xAA, 0xAA,
	0xAA, 0x00, 0xAA,
	0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00,//PALETTE_1_H
	0x55, 0xFF, 0xFF,
	0xFF, 0x55, 0xFF,
	0xFF, 0xFF, 0xFF,

	0x00, 0x00, 0x00,//PALETTE_2_L
	0x00, 0xAA, 0xAA,
	0xAA, 0x00, 0x00,
	0xAA, 0xAA, 0xAA,

	0x00, 0x00, 0x00,//PALETTE_2_H
	0x55, 0xFF, 0xFF,
	0xFF, 0x55, 0x55,
	0xFF, 0xFF, 0xFF
};

static const float P_TEXTURE_VERTICES[P_TEXTURE_NUM_VERTICES] = 
{
-1.0f, -1.0f, 0.0f, 1.0f,
-1.0f,  1.0f, 0.0f, 0.0f,
 1.0f, -1.0f, 1.0f, 1.0f,
 1.0f,  1.0f, 1.0f, 0.0f
};

static const char P_VERT_SRC[] = "\
#version 450\n\
\n\
layout (location = 0) in vec2 Pos;\n\
layout (location = 1) in vec2 Uv;\n\
\n\
out vec2 Tex;\n \
\n\
void main()\n\
{\n\
    gl_Position = vec4(Pos, 0.0f, 1.0f);\n\
\n\
    Tex = Uv;\n\
}\n";

static const char P_FRAG_SRC[] = "\
#version 450\n\
\n\
in vec2 Tex;\n\
\n\
uniform sampler2D Texture;\n\
\n\
uniform vec3 Palette[4];\n\
\n\
out vec4 out_color;\n\
\n\
void main()\n\
{\n\
	float index = texture2D(Texture,Tex).r*255.0f;\n\
\n\
    out_color = vec4(Palette[int(index)], 1.0f);\n\
}\n";

static const float P_TONE_FREQS[7] = 
{
    //261.63f, 293.66f, 329.63f, 349.23f, 392.00f, 440.00f, 493.88f
    262.00f, 294.00f, 330.00f, 350.00f, 392.00f, 440.00f, 494.00f
};

static const unsigned char P_FONT[2048] = 
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E, 
0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E, 0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00, 
0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00, 0x38, 0x7C, 0x38, 0xFE, 0xFE, 0xD6, 0x10, 0x38,
0x10, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x10, 0x38, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00, 
0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF, 0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00, 
0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF, 0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78,
0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0, 
0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0, 0x18, 0xDB, 0x3C, 0xE7, 0xE7, 0x3C, 0xDB, 0x18, 
0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00, 0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00,
0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,
0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00, 0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0xCC, 0x78, 
0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF,
0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00, 
0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00, 
0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00, 0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00,
0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x78, 0x78, 0x30, 0x30, 0x00, 0x30, 0x00, 
0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00,
0x30, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x30, 0x00, 0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00, 
0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00, 0x60, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00, 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,
0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,
0x7C, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0x7C, 0x00, 0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00, 
0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00, 0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00, 
0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00, 0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00,
0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00, 0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00, 
0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00, 0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00, 
0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60,
0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0x00, 
0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x78, 0xCC, 0x0C, 0x18, 0x30, 0x00, 0x30, 0x00, 
0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x78, 0x00, 0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00,
0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00, 
0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00, 
0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00, 0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0x00,
0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00, 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 
0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00, 0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00, 
0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,
0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, 
0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0xDC, 0x78, 0x1C, 0x00, 
0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00, 0x78, 0xCC, 0x60, 0x30, 0x18, 0xCC, 0x78, 0x00,
0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00, 
0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00, 
0xC6, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00,
0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00, 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00, 
0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, 
0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, 
0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00, 
0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
0x38, 0x6C, 0x60, 0xF0, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, 
0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00, 0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00,
0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xC6, 0x00, 
0x00, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 
0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E,
0x00, 0x00, 0xDC, 0x76, 0x66, 0x60, 0xF0, 0x00, 0x00, 0x00, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x00, 
0x10, 0x30, 0x7C, 0x30, 0x30, 0x34, 0x18, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 
0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, 0x00, 0x00, 0xC6, 0xD6, 0xFE, 0xFE, 0x6C, 0x00,
0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, 
0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00, 0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00, 
0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, 0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00,
0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00, 
0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x18, 0x0C, 0x78, 0x00, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00, 
0x1C, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 0x7E, 0xC3, 0x3C, 0x06, 0x3E, 0x66, 0x3F, 0x00,
0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00, 0xE0, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00, 
0x30, 0x30, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00, 0x00, 0x00, 0x78, 0xC0, 0xC0, 0x78, 0x0C, 0x38, 
0x7E, 0xC3, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00, 0xCC, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
0xE0, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, 0xCC, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
0x7C, 0xC6, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00, 0xE0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, 
0xC6, 0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00, 0x30, 0x30, 0x00, 0x78, 0xCC, 0xFC, 0xCC, 0x00,
0x1C, 0x00, 0xFC, 0x60, 0x78, 0x60, 0xFC, 0x00, 0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00, 
0x3E, 0x6C, 0xCC, 0xFE, 0xCC, 0xCC, 0xCE, 0x00, 0x78, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
0x00, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0xE0, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 
0x78, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00, 0x00, 0xE0, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00, 
0x00, 0xCC, 0x00, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, 0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00, 
0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x18, 0x18, 0x7E, 0xC0, 0xC0, 0x7E, 0x18, 0x18,
0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00, 0xCC, 0xCC, 0x78, 0xFC, 0x30, 0xFC, 0x30, 0x30, 
0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC7, 0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70, 
0x1C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x7E, 0x00, 0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
0x00, 0x1C, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x1C, 0x00, 0xCC, 0xCC, 0xCC, 0x7E, 0x00, 
0x00, 0xF8, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0x00, 0xFC, 0x00, 0xCC, 0xEC, 0xFC, 0xDC, 0xCC, 0x00, 
0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00,
0x30, 0x00, 0x30, 0x60, 0xC0, 0xCC, 0x78, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00, 0xC3, 0xC6, 0xCC, 0xDE, 0x33, 0x66, 0xCC, 0x0F, 
0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6F, 0xCF, 0x03, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00,
0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00, 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 
0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 
0xDB, 0x77, 0xDB, 0xEE, 0xDB, 0x77, 0xDB, 0xEE, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 
0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36, 
0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36,
0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36, 
0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00, 
0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18, 
0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36, 
0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36, 
0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36,
0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 
0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36, 0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 
0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18,
0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00, 
0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 
0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36,
0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00, 0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0, 
0x00, 0xFC, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,
0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00, 0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00, 
0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0, 0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00, 
0xFC, 0x30, 0x78, 0xCC, 0xCC, 0x78, 0x30, 0xFC, 0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0x6C, 0x38, 0x00,
0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00, 0x1C, 0x30, 0x18, 0x7C, 0xCC, 0xCC, 0x78, 0x00, 
0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00, 0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0, 
0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,
0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0xFC, 0x00, 
0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00, 0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00, 
0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70,
0x30, 0x30, 0x00, 0xFC, 0x00, 0x30, 0x30, 0x00, 0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00, 
0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C,
0x78, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x70, 0x18, 0x30, 0x60, 0x78, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//log
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = NULL;

//program
static PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
static PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
static PFNGLUSEPROGRAMPROC glUseProgram = NULL;
static PFNGLATTACHSHADERPROC glAttachShader = NULL;
static PFNGLDETACHSHADERPROC glDetachShader = NULL;
static PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
static PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
 
//shader
static PFNGLCREATESHADERPROC glCreateShader = NULL;
static PFNGLDELETESHADERPROC glDeleteShader = NULL;
static PFNGLSHADERSOURCEPROC glShaderSource = NULL;
static PFNGLCOMPILESHADERPROC glCompileShader = NULL;
static PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
 
//VBO
static PFNGLGENBUFFERSPROC glGenBuffers = NULL;
static PFNGLBINDBUFFERPROC glBindBuffer = NULL;
static PFNGLBUFFERDATAPROC glBufferData = NULL;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
static PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
 
//VAO
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
 
//wgl
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
static PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL; 

/************/
/* LOG UTIL */
/************/
static void PixieError(char *str, ...)
{
    #if defined(_MSC_VER) && defined(_DEBUG)
        int flag = 0;

        flag &= ~_CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flag);

        flag &= ~_CRTDBG_LEAK_CHECK_DF;
        _CrtSetDbgFlag(flag);
    #endif

	fprintf(stdout,"(*ERROR*)");

	if (str != NULL)
	{
		va_list argp;
		va_start(argp, str);
		vfprintf(stdout, str, argp);
		va_end(argp);
	};

    fprintf(stdout,"\n");

	exit(EXIT_FAILURE);
}

/****************/
/* BUFFER UTILS */
/****************/
static int PixieBufferReadNumber(unsigned char **buffer)
{
   int val = 0;

    while(isdigit(**buffer))
    {
        val = (val*10) + (**buffer - 0x30);
        *buffer += 1;
    };

    return val;
} 

/**********/
static void PixieBufferSkipBlanks(unsigned char **buffer)
{
    while(**buffer==P_CHARSET_SP ||
            **buffer==P_CHARSET_TAB ||
            **buffer==P_CHARSET_CR ||
            **buffer==P_CHARSET_LF)
    {
        *buffer += 1;
    }
}

/**********/
static void PixieBufferSkipLine(unsigned char **buffer)
{
    while(**buffer != P_CHARSET_LF)
    {
        *buffer += 1;
    }

    *buffer += 1;
}

/***************/
/* IMAGE UTILS */
/***************/
static void PixieImageVertFlip(unsigned char *buffer, int w, int h)
{
    int start = 0;
    int end = h-1;

    unsigned char *t = PAlloc(w,sizeof(unsigned char));

    while(start<end)
    {
        memcpy(t,&buffer[start*w],w);

        memcpy(&buffer[start*w],&buffer[end*w],w);

        memcpy(&buffer[end*w],t,w);

        start++;
        end--;
    }

    PFree(t);
}

/**********/
static void PixieImageNormalize(unsigned char *buffer, int w, int h)
{
    unsigned char d = 0xFF / (P_NUM_COLORS+1);

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			unsigned char oldPixel = buffer[i*w+j];

			unsigned char newPixel = 0x00;

            newPixel = (oldPixel/d)*d;

			buffer[i*w+j] = newPixel;

			unsigned char quant_error = (oldPixel - newPixel);

			if(j<w-1)
            { 
                buffer[i*w+j+1] = buffer[i*w+j+1] + quant_error * 7 / 16 ;
            }

			if(i<h-1)
			{
				buffer[(i+1)*w+j] = buffer[(i+1)*w+j] + quant_error * 5 / 16; 
			}

			if(i<h-1 && j>0)
			{
				buffer[(i+1)*w+j-1] = buffer[(i+1)*w+j-1] + quant_error * 3 / 16; 
			}

			if(i<h-1 && j<w-1)
        	{
				buffer[(i+1)*w+j+1] = buffer[(i+1)*w+j+1] + quant_error * 1 / 16; 
			}

		}
	}

    unsigned char q = 0xFF / P_NUM_COLORS;

	for(int k=0;k<w*h;k++)
	{
        buffer[k] = buffer[k]/q;
	}
	
}

/**********/
static void PixieImageLoadPbm(unsigned char **buf, unsigned char **buffer, int *w, int *h)
{
	int _w = 0;
	int _h = 0;
	
    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    _w = PixieBufferReadNumber(buf);

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    _h = PixieBufferReadNumber(buf);

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }

	//allocate data
	*buffer = PAlloc( _w*_h, sizeof(unsigned char));
	
	//read data
	int liner = 0;
	int line_counter = 0;
    int elem_counter = 0;
	unsigned char byte = 0x00;

	while(line_counter<_h)
	{
        byte = **buf;

		byte = ~byte;

		for(int i=7;i>=0;i--)
		{
			unsigned char b = ((byte>>i) & 1 ) * 0xFF;

            ((unsigned char *)(*buffer))[elem_counter] = (b==0x0) ? P_COLOR_0 : P_COLOR_3;

			elem_counter++;

			liner++;

			if(liner>=_w){ liner=0; line_counter++; break; }
		}

        *buf += 1;
	}

	if(w!=NULL){ *w=_w; }
	if(h!=NULL){ *h=_h; }

}

/**********/
static void PixieImageLoadPgm(unsigned char **buf, unsigned char **buffer, int *w, int *h)
{
	int _w = 0;
	int _h = 0;

	PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    _w = PixieBufferReadNumber(buf);

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    _h = PixieBufferReadNumber(buf);

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    int depth = PixieBufferReadNumber(buf);
	if(depth!=255){ PixieError("[%s]: depth format not supported", __func__); }

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }

	//allocate data
	*buffer = PAlloc( _w*_h, sizeof(unsigned char));
	
	//read data
	for(int i=0;i<_w*_h;i++)
	{
		((unsigned char *)(*buffer))[i] = **buf;

        *buf += 1;
	}

	if(w!=NULL){ *w=_w; }
	if(h!=NULL){ *h=_h; }

}

/**********/
static void PixieImageLoadPpm(unsigned char **buf, unsigned char **buffer, int *w, int *h)
{
	int _w = 0;
	int _h = 0;
	
    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    _w = PixieBufferReadNumber(buf);

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    _h = PixieBufferReadNumber(buf);

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }
    int depth = PixieBufferReadNumber(buf);
	if(depth!=255){ PixieError("[%s]: depth format not supported", __func__); }

    PixieBufferSkipBlanks(buf);
    while(**buf==P_CHARSET_HASH) { PixieBufferSkipLine(buf); }

	//allocate data
	*buffer = PAlloc( _w*_h, sizeof(unsigned char));
	
	//read data
	for(int i=0;i<_w*_h;i++)
	{
        unsigned char r = **buf;
        *buf += 1;
        unsigned char g = **buf;
        *buf += 1;
        unsigned char b = **buf;
        *buf += 1;

		unsigned char byte = (unsigned char)( r * P_IMAGE_R_FILTER + g * P_IMAGE_G_FILTER + b * P_IMAGE_B_FILTER);

        ((unsigned char *)(*buffer))[i] = byte;

	}

	if(w!=NULL){ *w=_w; }
	if(h!=NULL){ *h=_h; }

}

/**********/
static void PixieImageLoadBmp1(unsigned char *buf, int width, int height, int bits, unsigned char **buffer)
{
    int row = (bits * width + 31) / 32 * 4;

	*buffer = PAlloc(width*height, sizeof(unsigned char));
	
    int counter = 0;
    int elems_counter = 0;
	for(int i=height-1;i>=0;i--)
	{
        counter = width*i;

        for(int j=0;j<width/8;j++)
        {
            unsigned char byte = buf[j];

            for(int k=7;k>=0;k--)
            {
                unsigned char b = ((byte>>k) & 1 ) * 0xFF;

                ((unsigned char *)(*buffer))[counter] = (b==0x0) ? P_COLOR_0 : P_COLOR_3;

                counter++;

                if(elems_counter>=width){ elems_counter=0; break;}
            }

        }

        buf += row;   
	}
}

/**********/
static void PixieImageLoadBmp24(unsigned char *buf, int width, int height, int bits,  unsigned char **buffer)
{
    int row = (bits * width + 31) / 32 * 4;

	*buffer = PAlloc( width*height, sizeof(unsigned char));
	
    int counter = 0;

	for(int i=height-1;i>=0;i--)
	{
        counter = width*i;

        for(int j=0;j<width*3;j+=3)
        {
            unsigned char r = buf[j];
            unsigned char g = buf[j+1];
            unsigned char b = buf[j+2];
        
		    unsigned char byte = (unsigned char)(r*P_IMAGE_R_FILTER + g*P_IMAGE_G_FILTER + b*P_IMAGE_B_FILTER);

            ((unsigned char *)(*buffer))[counter] = byte;
            
            counter++;
        }

        buf += row;   
	}
}

/**********/
static void PixieImageLoadBmp(unsigned char **buf, unsigned char **buffer, int *w, int *h)
{
    unsigned char *b = *buf;

    PBufferReadInt(buf);                            //file size, not used
    PBufferReadInt(buf);                            //reserved fields, not used

    unsigned int offset = PBufferReadInt(buf);      //data offset
    PBufferReadInt(buf);                            //info size, not used
    
    int _w = PBufferReadInt(buf);                   //image width
    int _h = PBufferReadInt(buf);                   //image height

    PBufferReadShort(buf);                          //planes, not used

    unsigned short bits = PBufferReadShort(buf);    //bits per pixel
    unsigned int comp = PBufferReadInt(buf);        //compression type

    if(comp!=0){ PixieError("[%s]: Compressed bitmap not supported", __func__); }

    b += offset - 2;//cause we already have removed the magic bytes

    switch(bits)
    {
        case 1:
            PixieImageLoadBmp1(b,_w,abs(_h),bits,buffer);
            if(_h<0){ PixieImageVertFlip(*buffer,_w,abs(_h)); }
            break;
        case 24:
            PixieImageLoadBmp24(b,_w,abs(_h),bits,buffer);
            if(_h<0){ PixieImageVertFlip(*buffer,_w,abs(_h)); }
            PixieImageNormalize(*buffer,_w,_h);
            break;
        default:
            PixieError("[%s]: Unsupported bits depth (%d)", __func__, bits);
            break;
    }

    if(w!=NULL){ *w=_w; }
	if(h!=NULL){ *h=abs(_h); }
}


/****************/
/* OPENGL UTILS */
/****************/
static void PixieOpenglLoadExtensions()
{
    //log
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");

    //program
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
 
    //shader
    glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
 
    //VBO
    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
 
    //VAO
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");  

}

/**********/
static void PixieOpenglCreateContext()
{
    pixie->hdc = GetDC(pixie->hwnd);
    
    PIXELFORMATDESCRIPTOR pfd = {0};

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    int pixel_format = ChoosePixelFormat(pixie->hdc, &pfd);
    if(pixel_format==0) { PixieError("[%s]: Failed to select pixel format", __func__); }
    
    int res = SetPixelFormat(pixie->hdc, pixel_format, &pfd);
    if(res==0) { PixieError("[%s]: Failed to set pixel format", __func__); }

    pixie->hglrc = wglCreateContext(pixie->hdc);
    if(!pixie->hglrc){ PixieError("[%s]: Failed to create std opengl context", __func__); }

    if(!wglMakeCurrent(pixie->hdc, pixie->hglrc)){ PixieError("[%s]: Failed to set std opengl context", __func__); }

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

    if( !wglCreateContextAttribsARB || 
        !wglSwapIntervalEXT ||
        !wglGetSwapIntervalEXT )
        { PixieError("[%s]: Failed to get wgl functions", __func__);  return; }

    int attributes[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, P_OPENGL_MAJOR, 
    WGL_CONTEXT_MINOR_VERSION_ARB, P_OPENGL_MINOR,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0 };

    HGLRC hglrc = wglCreateContextAttribsARB(pixie->hdc, NULL, attributes);
    if(!hglrc) { PixieError("[%s]: Failed to create opengl context", __func__); }
    
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(pixie->hglrc);
    if(!wglMakeCurrent(pixie->hdc, hglrc) ) { PixieError("[%s]: Failed to set opengl context", __func__); }

}

/**********/
#if defined(_MSC_VER) && defined(_DEBUG)
static char *PixieOpenglDebugSource(GLenum source)
{
    switch (source)
	{
        case GL_DEBUG_SOURCE_API: 				return "GL_DEBUG_SOURCE_API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: 	return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: 	return "GL_DEBUG_SOURCE_SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: 		return "GL_DEBUG_SOURCE_THIRDPARTY";
        case GL_DEBUG_SOURCE_APPLICATION: 		return "GL_DEBUG_SOURCE_APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: 			return "GL_DEBUG_SOURCE_OTHER";
	}

	return "UNKNOW";
}
#endif

/*********/
#if defined(_MSC_VER) && defined(_DEBUG)
static char *PixieOpenglDebugType(GLenum type)
{
	switch (type)
	{
        case GL_DEBUG_TYPE_ERROR: 				return "GL_DEBUG_TYPE_ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: 	return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: 		return "GL_DEBUG_TYPEPORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: 		return "GL_DEBUG_TYPEPERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: 				return "GL_DEBUG_TYPE_MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP: 			return "GL_DEBUG_TYPEPUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP: 			return "GL_DEBUG_TYPEPOP_GROUP";
        case GL_DEBUG_TYPE_OTHER: 				return "GL_DEBUG_TYPE_OTHER";
	}

	return "UNKNOW";
}
#endif

/**********/
#if defined(_MSC_VER) && defined(_DEBUG)
static char *PixieOpenglDebugSeverity(GLenum severity)
{
	switch (severity)
	{
        case GL_DEBUG_SEVERITY_HIGH: 			return "GL_DEBUG_SEVERITY_HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM: 			return "GL_DEBUG_SEVERITY_MEDIUM";
        case GL_DEBUG_SEVERITY_LOW: 			return "GL_DEBUG_SEVERITY_LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION: 	return "GL_DEBUG_SEVERITY_NOTIFICATION";
	}

	return "UNKNOW";
}
#endif

/**********/
#if defined(_MSC_VER) && defined(_DEBUG)
static void APIENTRY PixieOpenglDebugCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* param)
{
	if(severity!=GL_DEBUG_SEVERITY_NOTIFICATION)//too much spam
	{
		printf("[%s] Source : %s, Type : %s, Severity : %s, Message : %s\n",
				__func__,
				PixieOpenglDebugSource(source),
				PixieOpenglDebugType(type),
				PixieOpenglDebugSeverity(severity),
				message
			);
	}
}
#endif

/**********/
static GLuint PixieOpenglShader(GLenum type, char *src)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, (const GLchar**)&src, NULL);

	glCompileShader(shader);

	GLint check = GL_FALSE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &check);

	if (check == GL_FALSE)
	{
		int len = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		char* msg = PAlloc(len, sizeof(char));

		glGetShaderInfoLog(shader, len, &len, msg);

		PixieError("[%s]: Shader compile failed : %s", __func__, msg);
	}

	return shader;
}

/**********/
static GLuint PixieOpenglProgramShader(GLuint vert_shader, GLuint frag_shader)
{
	GLuint prog_shader = glCreateProgram();

	glAttachShader(prog_shader, vert_shader);

	glAttachShader(prog_shader, frag_shader);

	glLinkProgram(prog_shader);

	GLint check = GL_FALSE;

	glGetProgramiv(prog_shader, GL_LINK_STATUS, &check);

	if (check == GL_FALSE)
	{
		int len = 0;

		glGetProgramiv(prog_shader, GL_INFO_LOG_LENGTH, &len);

		char* msg = PAlloc(len, sizeof(char));

		glGetProgramInfoLog(prog_shader, len, &len, msg);

		PixieError("[%s]: Shader link failed : %s", __func__, msg);
	}

	return prog_shader;
}

/**********/
static void PixieOpenglInit()
{
    #if defined(_MSC_VER) && defined(_DEBUG)
	    glEnable(GL_DEBUG_OUTPUT);
	    glDebugMessageCallback(PixieOpenglDebugCallback, NULL);
    #endif

	glGenVertexArrays(1, &pixie->vao);
    glBindVertexArray(pixie->vao); 

	glGenBuffers(1, &pixie->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pixie->vbo);

	glEnableVertexAttribArray(0);//vertex 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
    glEnableVertexAttribArray(1);//texture 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)) );

	glBufferData(GL_ARRAY_BUFFER, P_TEXTURE_NUM_VERTICES*sizeof(float), P_TEXTURE_VERTICES, GL_STATIC_DRAW);	
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);//fix for w not multiple of 4

	glGenTextures(1, &pixie->texture);

	glBindTexture(GL_TEXTURE_2D, pixie->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixie->width, pixie->height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	pixie->vert_shader = PixieOpenglShader(GL_VERTEX_SHADER, (char*)P_VERT_SRC);

    pixie->frag_shader = PixieOpenglShader(GL_FRAGMENT_SHADER, (char*)P_FRAG_SRC);

    pixie->prog_shader = PixieOpenglProgramShader(pixie->vert_shader, pixie->frag_shader);

	glUseProgram(pixie->prog_shader);

    wglSwapIntervalEXT(0);

}

/**********/
static void PixieOpenglViewport()
{
	int x = 0;
	int y = 0;

	float ratio = (float)pixie->width/(float)pixie->height;

	if( pixie->client_w/ratio < pixie->client_h)
	{
		pixie->viewport_w = pixie->client_w;
		pixie->viewport_h = (int)((float)pixie->viewport_w/ratio);
	}
	else
	{
		pixie->viewport_h = pixie->client_h;
		pixie->viewport_w = (int)((float)pixie->viewport_h*ratio);
	}

	x = (pixie->client_w-pixie->viewport_w)/2;
	y = (pixie->client_h-pixie->viewport_h)/2;
		
	glViewport(x, y, pixie->viewport_w, pixie->viewport_h);	
}

/**********/
static void PixieOpenglClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

/**********/
static void PixieOpenglSwapbuffers()
{
    SwapBuffers(pixie->hdc);
}

/****************/
/* OPENAL UTILS */
/****************/
static char *PixieOpenalError(int err)
{
    switch(err) 
    {
        case AL_NO_ERROR :          return NULL;
        case AL_INVALID_NAME:       return "AL_INVALID_NAME";
        case AL_INVALID_ENUM:       return "AL_INVALID_ENUM";
        case AL_INVALID_VALUE:      return "AL_INVALID_VALUE";
        case AL_INVALID_OPERATION:  return "AL_INVALID_OPERATION";
        case AL_OUT_OF_MEMORY:      return "AL_OUT_OF_MEMORY";
    }

	return "UNKNOW OPEN AL ERROR";
}

/**********/
static void PixieOpenalCheckError()
{
    int err = alGetError();

	if(err!=0)
    {
        PixieError("[%s]: Shader link failed : %s", __func__, PixieOpenalError(err));
    }
}

/**********/
static void PixieOpenalGenerateTone(unsigned char *buf, int sample_rate, float freq)
{
    for(int i=0;i<sample_rate;i++)
    {
        buf[i] = (unsigned char)( 0xFF * sin( M_PI*freq/sample_rate*i ) );
    }
}

/**********/
static void PixieOpenalInit()
{
    char *name = (char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    
    pixie->adev = alcOpenDevice(name);

    if(!pixie->adev){ PixieError("[%s]: Audio device open failed", __func__); };

    pixie->actx = alcCreateContext(pixie->adev, NULL);

    if(!pixie->actx){ PixieError("[%s]: Audio context creation failed"); }

    if(!alcMakeContextCurrent(pixie->actx)){ PixieError("[%s]: Setting audio context failed", __func__); }

    alGenSources(1, &pixie->asrc_snd);
    PixieOpenalCheckError();

    alGenSources(1, &pixie->asrc_tone);
    PixieOpenalCheckError();

    alGenBuffers(1, &pixie->asnd);
    PixieOpenalCheckError();

    alGenBuffers(P_NUM_TONES, pixie->atones);
    PixieOpenalCheckError();

    //tones generation
    unsigned char *buf = PAlloc(P_TONE_SAMPLE_RATE,sizeof(unsigned char));

    for(int i=0;i<P_NUM_TONES;i++)
    {
        PixieOpenalGenerateTone(buf, P_TONE_SAMPLE_RATE, P_TONE_FREQS[i]);
        alBufferData(pixie->atones[i], AL_FORMAT_MONO8, buf, P_TONE_SAMPLE_RATE, P_TONE_SAMPLE_RATE);
    } 

    PFree(buf); 

    alSourcef(pixie->asrc_snd, AL_GAIN, P_DEFAULT_VOLUME);
    alSourcef(pixie->asrc_tone, AL_GAIN, P_DEFAULT_VOLUME);

}

/**********/
static void PixieOpenalClose()
{
    int status = 0;

    alGetSourcei(pixie->asrc_snd, AL_SOURCE_STATE, &status);
    if(status!=AL_STOPPED) { alSourceStop(pixie->asrc_snd); }
    
    alGetSourcei(pixie->asrc_tone, AL_SOURCE_STATE, &status);
    if(status!=AL_STOPPED) { alSourceStop(pixie->asrc_tone); }

    alDeleteBuffers(1, &pixie->asnd);

    alDeleteBuffers(7, pixie->atones);

    alDeleteSources(1, &pixie->asrc_snd);

    alDeleteSources(1, &pixie->asrc_tone);

    alcMakeContextCurrent(NULL);

    alcDestroyContext(pixie->actx);

    alcCloseDevice(pixie->adev);
}

/****************/
/* WINDOW UTILS */
/****************/
static int PixieWndVKtoAscii(int vk)
{
    int scancode = 0;
    unsigned short key = 0;
    unsigned char keyboard[256] = {0};

    scancode = MapVirtualKey(vk,MAPVK_VK_TO_VSC);

    if( GetKeyboardState(keyboard) && ToAscii(vk, scancode, keyboard, &key, 0) && isprint(key) ) 
    { 
        return key; 
    }

    return 0;
}

/**********/
static LRESULT CALLBACK PixieWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
        case WM_CLOSE:
            pixie->event = P_QUIT;
            PostQuitMessage(0);
            return 0;

        case WM_DESTROY:
            pixie->event = P_QUIT;
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            pixie->client_w = (int)LOWORD(lParam);
            pixie->client_h = (int)HIWORD(lParam);
            PixieOpenglViewport();
            break;

        case WM_KEYDOWN: 
            pixie->event = P_KEYPRESS;
            pixie->key = (int)wParam;
            pixie->ascii_key = PixieWndVKtoAscii(pixie->key);
            break;

        case WM_KEYUP: 
            pixie->event = P_KEYRELEASE;
            pixie->key = (int)wParam;
            pixie->ascii_key = PixieWndVKtoAscii(pixie->key);
            break;

        case WM_MOUSEMOVE:
            pixie->event = P_MOUSEMOVE;
            pixie->mousex = GET_X_LPARAM(lParam);
            pixie->mousey = GET_Y_LPARAM(lParam);
            break;

        case WM_LBUTTONDOWN:
            pixie->event = P_MOUSEPRESS;
            pixie->button = P_BUT_LEFT;
            pixie->mousex = GET_X_LPARAM(lParam);
            pixie->mousey = GET_Y_LPARAM(lParam);
            break;

        case WM_MBUTTONDOWN:
            pixie->event = P_MOUSEPRESS;
            pixie->button = P_BUT_MID;
            pixie->mousex = GET_X_LPARAM(lParam);
            pixie->mousey = GET_Y_LPARAM(lParam);
            break;

        case WM_RBUTTONDOWN:
            pixie->event = P_MOUSEPRESS;
            pixie->button = P_BUT_RIGHT;
            pixie->mousex = GET_X_LPARAM(lParam);
            pixie->mousey = GET_Y_LPARAM(lParam);
            break;

        case WM_LBUTTONUP:
            pixie->event = P_MOUSERELEASE;
            pixie->button = P_BUT_LEFT;
            pixie->mousex = GET_X_LPARAM(lParam);
            pixie->mousey = GET_Y_LPARAM(lParam);
            break;

        case WM_MBUTTONUP:
            pixie->event = P_MOUSERELEASE;
            pixie->button = P_BUT_MID;
            pixie->mousex = GET_X_LPARAM(lParam);
            pixie->mousey = GET_Y_LPARAM(lParam);
            break;

        case WM_RBUTTONUP:
            pixie->event = P_MOUSERELEASE;
            pixie->button = P_BUT_RIGHT;
            pixie->mousex = GET_X_LPARAM(lParam);
            pixie->mousey = GET_Y_LPARAM(lParam);
            break;

        case WM_MOUSEWHEEL:
            pixie->event = P_MOUSEWHEEL;
            pixie->wheel_dir = GET_WHEEL_DELTA_WPARAM(wParam);
            break;

        default:
            pixie->event = P_NONE;
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

/**********/
static void PixieWndRegisterClass()
{
    WNDCLASS wc = {0};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpszClassName = (LPCSTR)(P_WND_CLASS);
    wc.lpfnWndProc = PixieWndProc;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);

    if( !RegisterClass(&wc) ) { PixieError("[%s]: Failed to register class\n", __func__); }
}

/**********/
static void PixieWndCreateWindow()
{
    RECT rect = {0, 0, pixie->width, pixie->height};

    if(!AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0) ) { PixieError("[%s]: Unable to adjust client area", __func__); }

    pixie->hwnd = CreateWindow((LPCSTR)(P_WND_CLASS), P_VERSION, WS_OVERLAPPEDWINDOW , CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, NULL, NULL);

    if(!pixie->hwnd) { PixieError("[%s]: Failed to create window", __func__); return; }

    GetWindowPlacement(pixie->hwnd, &pixie->hwndPlacement);    
}

/**********/
static void PixieWndShowInitialWindow()
{
    long style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    if(pixie->screen)
    { 
        style &= ~WS_DLGFRAME;
        style |= WS_POPUP;
        SetWindowLong(pixie->hwnd, GWL_STYLE, style);
        ShowWindow(pixie->hwnd, SW_SHOWMAXIMIZED );
        SetActiveWindow(pixie->hwnd); 
    }
    else
    {
        SetWindowLong(pixie->hwnd, GWL_STYLE, style);
        ShowWindow(pixie->hwnd, SW_SHOW); 
        SetActiveWindow(pixie->hwnd);
    }
}

/*********/
/* PIXIE */
/*********/
void PixieDebug()
{
    #if defined(_MSC_VER) && defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    #endif
}

/**********/
void PixieInit(int w, int h, int screen)
{
    #if defined(_MSC_VER) && defined(_DEBUG)
        PixieDebug();
    #endif

    pixie = PAlloc(1, sizeof(Pixie));

    pixie->width = w;
    pixie->height = h;
    pixie->client_w = w;
    pixie->client_h = h;
    pixie->viewport_w = w;
    pixie->viewport_h = h;

    pixie->screen = screen;

    pixie->buffer = PAlloc(w*h, sizeof(unsigned char));

    pixie->color = P_COLOR_3;

	pixie->font_w = P_FONT_W;
	pixie->font_h = P_FONT_H;
	pixie->font_scale = P_FONT_SCALE;
	pixie->font = (unsigned char *)P_FONT;
	
    PixieWndRegisterClass();

    PixieWndCreateWindow();

    PixieOpenglCreateContext();

    PixieOpenglLoadExtensions();

    PixieOpenglInit();

    PixieOpenglViewport();

    PixieOpenglClear();

    PixieOpenglSwapbuffers();

    PixiePaletteSet(P_PALETTE_MONO);

    PixieOpenalInit();

    QueryPerformanceFrequency(&pixie->freq); 

    QueryPerformanceCounter(&pixie->timer);

    PixieWndShowInitialWindow();
}

/**********/
void PixieClose()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

    glDeleteTextures(1, &pixie->texture);

    glDisableVertexAttribArray(0);

    glDisableVertexAttribArray(1);

    glDeleteVertexArrays(1, &pixie->vao);

    glDeleteBuffers(1, &pixie->vbo);

	glDetachShader(pixie->prog_shader, pixie->vert_shader);
	glDeleteShader(pixie->vert_shader);

	glDetachShader(pixie->prog_shader, pixie->frag_shader);
	glDeleteShader(pixie->frag_shader);

	glDeleteProgram(pixie->prog_shader);

    wglMakeCurrent(NULL, NULL);
    
    wglDeleteContext(pixie->hglrc);

    DeleteDC(pixie->hdc);

    DestroyWindow(pixie->hwnd);

    PixieOpenalClose();

    PFree(pixie->buffer);

    PFree(pixie);
    
    srand((unsigned int)time(NULL));
}

/**********/
void PixieToggleFullScreen()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

    long style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    if(pixie->screen)
    {
        SetWindowLong(pixie->hwnd, GWL_STYLE, style);
        SetWindowPlacement(pixie->hwnd, &pixie->hwndPlacement);
        ShowWindow(pixie->hwnd, SW_SHOWNORMAL);
    }
    else
    {
        GetWindowPlacement(pixie->hwnd, &pixie->hwndPlacement);

        style &= ~WS_DLGFRAME;
        style |= WS_POPUP;
        SetWindowLong(pixie->hwnd, GWL_STYLE, style);
        ShowWindow(pixie->hwnd, SW_SHOWMAXIMIZED );
    }

    pixie->screen = !pixie->screen;
}

/**********/
void PixieToggleVsync()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); }

    int val = wglGetSwapIntervalEXT();

    if(val==0) { wglSwapIntervalEXT(1); }
    else { wglSwapIntervalEXT(0); }
}

/**********/
/* RENDER */
/**********/
void PixieRenderClear(int col)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	unsigned char _col = col % P_NUM_COLORS;

	memset(pixie->buffer,_col,pixie->width*pixie->height);
}

/**********/
void PixieRenderDraw()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pixie->width, pixie->height, GL_RED, GL_UNSIGNED_BYTE, pixie->buffer);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    SwapBuffers(pixie->hdc);
}

/**********/
int PixieRenderGetWidth()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

	return pixie->width;
}

/**********/
int PixieRenderGetHeight()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

	return pixie->height;
}

/*********************/
/* COLOR AND PALETTE */
/*********************/
void PixieColorSet(int col)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	pixie->color = col % P_NUM_COLORS;
}

/**********/
void PixiePaletteSet(int pal)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	int index = (pal % P_NUMPALETTES)*P_PALETTE_SIZE;

    float v[P_PALETTE_SIZE]={0};
	
	for(int i=0;i<P_PALETTE_SIZE;i++)
	{
		v[i] = (float)P_PALETTES[index+i] / 255.0f;
    }

	int loc = glGetUniformLocation(pixie->prog_shader, P_PALETTE_LOCATION);

    glUniform3fv(loc, 4, v);

}

/***********/
/* DRAWING */
/***********/
void PixieDrawPoint(int x, int y)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	if(x>=0 && x<pixie->width && y>=0 && y<pixie->height)
	{
		pixie->buffer[y*pixie->width+x] = pixie->color;
	}
}

/**********/
void PixieDrawLine(int srcx, int srcy, int dstx, int dsty)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

    if(srcy==dsty)//h-line
    {
        if(srcy<0 || srcy>=pixie->height){ return; }

        int start = PMin(srcx,dstx);
        start = PMax(start,0);

        int end = PMax(srcx,dstx+1);
        end = PMin(end,pixie->width);

        if(end-start<=0){ return; }

        unsigned char *p = pixie->buffer;
        p += pixie->width * srcy + start;

        memset(p,pixie->color,end-start);

        return;
    }

    if(srcx==dstx)//v-line
    {
        if(srcx<0 || srcx>=pixie->width){ return; }

        int start = PMin(srcy,dsty);
        start = PMax(start,0);

        int end = PMax(srcy,dsty+1);
        end = PMin(end,pixie->height);

        if(end-start<=0){ return; }

        for(int i=0;i<end-start;i++)
        { 
            pixie->buffer[(start+i)*pixie->width+srcx] = pixie->color;
        };

        return;
    }

    int dx = abs(dstx-srcx);
    int sx = srcx<dstx ? 1 : -1;
    int dy = abs(dsty-srcy);
    int sy = srcy<dsty ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;
 
    for(;;)
    {
        PixieDrawPoint(srcx, srcy);
        if (srcx==dstx && srcy==dsty) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; srcx += sx; }
        if (e2 < dy) { err += dx; srcy += sy; }
    }
}

/**********/
void PixieDrawTri(int ax, int ay, int bx, int by, int cx, int cy)
{
    PixieDrawLine(ax,ay,bx,by);
    PixieDrawLine(bx,by,cx,cy);
    PixieDrawLine(cx,cy,ax,ay);
}

/**********/
static void PixieDrawTriFillBotFlat(int ax, int ay, int bx, int by, int cx, int cy)
{
    //note +0.55f just for round

    float left = (float)(bx-ax)/(float)(by-ay);
    float right = (float)(cx-ax)/(float)(cy-ay);

    float start = (float)ax;
    float end = (float)ax;

    for(int i=ay;i<=by;i++)
    {
        PixieDrawLine((int)(start+0.55f),i,(int)(end+0.55f),i);

        start += left;
        end += right;
    }
}

/**********/
static void PixieDrawTriFillTopFlat(int ax, int ay, int bx, int by, int cx, int cy)
{
    //note +0.55f just for round

    float left = (float)(cx-ax)/(float)(cy-ay);
    float right = (float)(cx-bx)/(float)(cy-by);

    float start = (float)cx;
    float end = (float)cx;

    for(int i=cy;i>ay;i--)
    {
        PixieDrawLine((int)(start+0.55f),i,(int)(end+0.55f),i);

        start -= left;
        end -= right;
    }
}

/**********/
void PixieDrawTriFill(int ax, int ay, int bx, int by, int cx, int cy)
{
    //sort vertices by y
    if (ay > by){ PSwap(&ay, &by); PSwap(&ax, &bx); }
    if (ay > cy){ PSwap(&ay, &cy); PSwap(&ax, &cx); }
    if (by > cy){ PSwap(&by, &cy); PSwap(&bx, &cx); }

    if(by==cy)
    {
        PixieDrawTriFillBotFlat(ax,ay,bx,by,cx,cy);
    }
    else if (ay==by)
    {
        PixieDrawTriFillTopFlat(ax,ay,bx,by,cx,cy);
    }
    else
    {
        int dx = (int)( (float)ax + ((float)(by - ay) / (float)(cy - ay)) * (float)(cx - ax));
        int dy = by;

        PixieDrawTriFillBotFlat(ax,ay,bx,by,dx,dy);
        PixieDrawTriFillTopFlat(bx,by,dx,dy,cx,cy);
    }
}

/**********/
void PixieDrawRect(int x, int y, int w, int h)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); }

    PixieDrawLine(    x,     y,  x+w-1,       y);
    PixieDrawLine(x+w-1,     y,  x+w-1,   y+h-1);
    PixieDrawLine(x+w-1, y+h-1,      x,   y+h-1);
    PixieDrawLine(    x, y+h-1,      x,       y);

}

/**********/
void PixieDrawRectFill(int x, int y, int w, int h)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); }

    for(int i=0;i<h;i++)
    {
        PixieDrawLine(x,y+i,x+w-1,y+i);
    }
}

/**********/
void PixieDrawCircle(int x, int y, int r)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); }

	int offsetx = 0; 
    int offsety = r;
	int d = r-1;
	
    while (offsety >= offsetx) 
    {
        PixieDrawPoint(x + offsetx, y + offsety);
        PixieDrawPoint(x + offsety, y + offsetx);
        PixieDrawPoint(x - offsetx, y + offsety);
        PixieDrawPoint(x - offsety, y + offsetx);
        PixieDrawPoint(x + offsetx, y - offsety);
        PixieDrawPoint(x + offsety, y - offsetx);
        PixieDrawPoint(x - offsetx, y - offsety);
        PixieDrawPoint(x - offsety, y - offsetx);
	
        if (d >= 2*offsetx) 
        {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
	    else if (d < 2 * (r - offsety)) 
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else 
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

/**********/
void PixieDrawCircleFill(int x, int y, int r)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); }

	int offsetx = 0; 
    int offsety = r;
	int d = r-1;
	
    while (offsety >= offsetx) 
    {	
        PixieDrawLine(x - offsety, y + offsetx, x + offsety, y + offsetx);
        PixieDrawLine(x - offsetx, y + offsety, x + offsetx, y + offsety);
        PixieDrawLine(x - offsetx, y - offsety, x + offsetx, y - offsety);
        PixieDrawLine(x - offsety, y - offsetx, x + offsety, y - offsetx);
        
        if (d >= 2*offsetx) 
        {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (r - offsety)) 
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else 
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

/**********/
void PixieDrawImage(unsigned char *buffer, int w, int h, int x, int y)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

    int rx = 0;
    int ry = 0;
    int rw = 0;
    int rh = 0;

    int res = PixieImageIntersect(x, y, w, h, 
                                    0, 0, pixie->width, pixie->height,
                                    &rx, &ry, &rw, &rh);
    if(!res) { return; }

    unsigned char *src = buffer;
    unsigned char *dst = pixie->buffer;

    src += w*(ry-y) + (rx-x);
    dst += pixie->width*(ry) + (rx);

    for(int i=0;i<rh;i++)
    {
        memcpy(dst,src,rw);
        src += w;
        dst += pixie->width;
    }

}

/********/
void PixieDrawImagePart(unsigned char *buffer, int bw, int bh, int sx, int sy, int sw, int sh, int x, int y)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

    int check = 0;

    int srcx = 0;
    int srcy = 0;
    int srcw = 0;
    int srch = 0;
    check = PixieImageIntersect(sx,sy,sw,sh,
                                    0,0,bw,bh,
                                    &srcx,&srcy,&srcw,&srch);

    if(!check){ return; }

    int dstx = 0;
    int dsty = 0;
    int dstw = 0;
    int dsth = 0;
    check = PixieImageIntersect(x,y,sw,sh,
                                    0,0,pixie->width,pixie->height,
                                    &dstx,&dsty,&dstw,&dsth);

    if(!check){ return; }

    int size_w = PMin(srcw,dstw);
    int size_h = PMin(srch,dsth);

    unsigned char *src = buffer;
    unsigned char *dst = pixie->buffer;

    src += bw*srcy + srcx;
    dst += pixie->width*dsty + dstx;

    if(x<0){ src += srcw - dstw; }
    if(y<0){ src += bw*(srch - dsth); }

    for(int i=0;i<size_h;i++)
    {
        memcpy(dst,src,size_w);

        src += bw;
        dst += pixie->width;
    }
    
}

/**********/
void PixieDrawImageMask(unsigned char *buffer, int bw, int bh, unsigned char *mask, int mw, int mh, int x, int y, int col)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	if(bw!=mw || bh!=mh){ PixieError("[%s]: Buffers sizes mismatch\n", __func__); };
	
	int rx = 0;
    int ry = 0;
    int rw = 0;
    int rh = 0;

    int res = PixieImageIntersect(x, y, bw, bh, 
                                    0, 0, pixie->width, pixie->height,
                                    &rx, &ry, &rw, &rh);
    if(!res) { return; }

    unsigned char _col = col%P_NUM_COLORS;

    unsigned char *src = buffer;
    unsigned char *msk = mask;
    unsigned char *dst = pixie->buffer;

    src += bw*(ry-y) + (rx-x);
    msk += mw*(ry-y) + (rx-x);
    dst += pixie->width*(ry) + (rx);

    for(int i=0;i<rh;i++)
    {
        for(int j=0;j<rw;j++)
        {
            if(msk[j]!=_col) { dst[j] = src[j]; }
        } 

        src += bw;
        msk += mw;
        dst += pixie->width;
    }
}

/**********/
void PixieDrawImagePartMask(unsigned char *buffer, int bw, int bh, unsigned char *mask, int mw, int mh, int sx, int sy, int sw, int sh, int x, int y, int col)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

    if(bw!=mw || bh!=mh){ PixieError("[%s]: Buffers sizes mismatch\n", __func__); };

    int check = 0;

    int srcx = 0;
    int srcy = 0;
    int srcw = 0;
    int srch = 0;
    check = PixieImageIntersect(sx,sy,sw,sh,
                                    0,0,bw,bh,
                                    &srcx,&srcy,&srcw,&srch);

    if(!check){ return; }

    int dstx = 0;
    int dsty = 0;
    int dstw = 0;
    int dsth = 0;
    check = PixieImageIntersect(x,y,sw,sh,
                                    0,0,pixie->width,pixie->height,
                                    &dstx,&dsty,&dstw,&dsth);

    if(!check){ return; }

    unsigned char _col = col%P_NUM_COLORS;

    int size_w = PMin(srcw,dstw);
    int size_h = PMin(srch,dsth);

    unsigned char *src = buffer;
    unsigned char *msk = mask;
    unsigned char *dst = pixie->buffer;
    
    src += bw*srcy + srcx;
    msk += mw*srcy + srcx;
    dst += pixie->width*dsty + dstx;

    if(x<0)
    { 
        src += srcw - dstw;
        msk += srcw - dstw;
    }

    if(y<0)
    { 
        src += bw*(srch - dsth); 
        msk += bw*(srch - dsth); 
    }

    for(int i=0;i<size_h;i++)
    {
        for(int j=0;j<size_w;j++)
        {
            if(msk[j]!=_col){ dst[j]=src[j]; }
        }

        src += bw;
        msk += mw;
        dst += pixie->width;
    }

}

/**********/
void PixieDrawChar(char c, int x, int y)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	int index = (unsigned char)c;//lost my life on this cast !!!

    if(index>=P_CHARSET_SIZE){ index = (unsigned char)P_CHARSET_UNKNOW; }
	
	int offset = index * pixie->font_h;

	unsigned char *bytes = pixie->font + offset;

	for(int i=0;i<pixie->font_h;i++)
    {
		unsigned char byte = bytes[i];

		for(int j=0;j<pixie->font_w;j++)
    	{
			unsigned char val = (byte>>j) & 1;

			if(val == 0x00){ continue; }

			int start_i = y+(pixie->font_scale*i);
			int start_j = x+(pixie->font_scale*(pixie->font_w-j-1));

            if(start_i>=pixie->height) { continue; }
            if(start_j>=pixie->width) { continue; }

            int size_h = pixie->font_scale;
            int size_w = pixie->font_scale; 

            if(start_i<0)
            { 
                size_h+=start_i; 
                if(size_h<0){ continue; }
                start_i=0; 
            }
            
            if(start_j<0)
            { 
                size_w+=start_j; 
                if(size_w<0){ continue; }
                start_j=0; 
            }
            
            size_h = PMin(size_h, pixie->height-start_i);
            size_w = PMin(size_w, pixie->width-start_j);

            unsigned char *p = pixie->buffer;
            p += start_i*pixie->width + start_j;

            for(int k=0;k<size_h;k++)
            {
                memset(p,pixie->color,size_w);

                p += pixie->width;

            }
		}
    }
}

void PixieDrawString(char *s, int x, int y)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	if(s==NULL){ return; }

	size_t len = strlen(s);
	int posx = x;
	int posy = y;

	for(size_t i=0;i<len;i++)
	{	
		if(s[i]==P_CHARSET_LF){ posy += pixie->font_scale*pixie->font_h; posx = x; continue; };

		PixieDrawChar(s[i], posx, posy); 

		posx += pixie->font_scale*pixie->font_w;
	}
}

/**********/
/* IMAGES */
/**********/
void PixieImageLoad(char *file, unsigned char **buffer, int *w, int *h)
{
    int size = 0;
    unsigned char *buf = NULL;
    
    PBufferLoad(file,&buf,&size);

    unsigned char *b = buf;

    unsigned short type = (PBufferReadByte(&b)<<8) + PBufferReadByte(&b);

    switch (type)
    {
        case 0x5034://P4
            PixieImageLoadPbm(&b,buffer,w,h);
            break;

        case 0x5035://P5
            PixieImageLoadPgm(&b,buffer,w,h);
            PixieImageNormalize(*buffer, *w, *h);
            break;

        case 0x5036://P6
            PixieImageLoadPpm(&b,buffer,w,h);
            PixieImageNormalize(*buffer, *w, *h);
            break;

        case 0x424D://BMP
            PixieImageLoadBmp(&b,buffer,w,h);
            break;
        
        default:
            PFree(buf);
            PixieError("[%s]: invalid file format", __func__);
            break;
    }

    PFree(buf);
}

/*********/
int PixieImageIntersect(int ax, int ay, int aw, int ah, 
                            int bx, int by, int bw, int bh,
                            int *rx, int *ry, int *rw, int *rh)
{
	
    int _rx = 0;
    int _ry = 0;
    int _rw = 0;
    int _rh = 0;

    int Amin = 0;
    int Amax = 0;
    int Bmin = 0;
    int Bmax = 0;

    // Horizontal intersection 
    Amin = ax;
    Amax = Amin + aw;
    Bmin = bx;
    Bmax = Bmin + bw;
    if (Bmin > Amin) { Amin = Bmin; }
    _rx = Amin;
    if (Bmax < Amax) { Amax = Bmax; }
    _rw = Amax - Amin;

    // Vertical intersection
    Amin = ay;
    Amax = Amin + ah;
    Bmin = by;
    Bmax = Bmin + bh;
    if (Bmin > Amin) { Amin = Bmin; }
    _ry = Amin;
    if (Bmax < Amax) { Amax = Bmax; }
    _rh = Amax - Amin;

    if(_rw<=0 || _rh<=0)
    { 
        if(rx){ *rx = 0; }
        if(ry){ *ry = 0; }
        if(rw){ *rw = 0; }
        if(rh){ *rh = 0; }

        return 0; 
    }

    if(rx){ *rx = _rx; }
    if(ry){ *ry = _ry; }
    if(rw){ *rw = _rw; }
    if(rh){ *rh = _rh; }

    return 1;
}

/**********/
int PixieImageOverlap(unsigned char *a, int ax, int ay, int aw, int ah, 
                    unsigned char *b, int bx, int by, int bw, int bh, 
                    int col)
{
    int rx = 0;
    int ry = 0;
    int rw = 0;
    int rh = 0;

	int res = PixieImageIntersect(ax,ay,aw,ah,bx,by,bw,bh,&rx,&ry,&rw,&rh);
    
    if(res)
    {
        unsigned char _col = col % P_NUM_COLORS;

        unsigned char *_a = a;
        unsigned char *_b = b;

        _a += (ry-ay)*aw + (rx-ax);
        _b += (ry-by)*bw + (rx-bx);

        for(int i=0;i<rh;i++)
        {
            for(int j=0;j<rw;j++)
            {
                if(_a[j]==_col && _b[j]==_col) { return 1; }
            } 
            _a+=aw;
            _b+=bw;
        }
    }

    return 0;
}

/*********/
/* FONTS */
/*********/
void PixieFontLoad(char *file, unsigned char **buffer, int *size)
{
    PBufferLoad(file,buffer,size);
}

/**********/
void PixieFontSet(unsigned char *buffer,int size)
{	
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	if(!buffer || size<256 || size%256!=0)
	{
		pixie->font   = (unsigned char *)P_FONT;
		pixie->font_w = P_FONT_W;
		pixie->font_h = P_FONT_H;
	}
	else
	{
		pixie->font = buffer;
		pixie->font_w = P_FONT_W;
		pixie->font_h = size/P_CHARSET_SIZE;
	}
}

/**********/
void PixieFontSetScale(int scale)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	pixie->font_scale = (scale < 1) ? 1 : scale;
}


/**********/
int PixieFontGetWidth()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

	return pixie->font_w*pixie->font_scale;
}

/**********/
int PixieFontGetHeight()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

	return pixie->font_h*pixie->font_scale;
}

/**********/
void PixieFontGetStringSize(char *s, int *w, int *h)
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return; }

	int _w = 0;
	int _h = 1;
	int count = 0;

	for (size_t i = 0; i < strlen(s); i++)
	{
		count++;

		if (s[i] == P_CHARSET_LF) { count = 0; _h++; }

		if (count > _w) { _w = count; };
	};

	if(w) { *w = pixie->font_w *_w * pixie->font_scale; }
	if(h) { *h = pixie->font_h *_h * pixie->font_scale; }

}

/*********/
/* AUDIO */
/*********/
void PixieAudioLoadSound(char *file, unsigned char **buffer, int *size, int *format, int *rate)
{
    int buf_size = 0;
    unsigned char *buf = NULL;
    PBufferLoad(file,&buf,&buf_size);

    unsigned char *b = buf;

    if(memcmp(b,"RIFF", 4)!=0){ PixieError("[%s]: file type not supported", __func__); }
    b+=4;
    
    b+=4;//size of file, not used

    if(memcmp(b,"WAVE", 4)!=0){ PixieError("[%s]: file format not supported", __func__); }
    b+=4;

    if(memcmp(b,"fmt ", 4)!=0){ PixieError("[%s]: fmt marker not found", __func__); }
    b+=4;

    int comp = PBufferReadInt(&b);
    if(comp!=16){ PixieError("[%s]: compressed data not supported", __func__); }

    short pcm = PBufferReadShort(&b);
    if(pcm!=1){ PixieError("[%s]: file not in pcm format", __func__); }

    int _channels = (int)PBufferReadShort(&b);
    
    int _sample_rate = PBufferReadInt(&b);

    b+=4;//block align, not used
    b+=2;//byte rate, not used

    int _bps = (int)PBufferReadShort(&b);

    //read until we get "data" chunk or buf is exausted
    while(memcmp(b,"data", 4)!=0)
    {
        b+=4;

        int off = PBufferReadInt(&b);

        b+=off;
        
        if(b-buf>=buf_size){ PixieError("[%s]: buffer exausted", __func__); }
    }

    b+=4;

    int _size = PBufferReadInt(&b);
    if(_size<=0){ PixieError("[%s]: error getting data size", __func__); }

    *buffer = PAlloc(_size,sizeof(unsigned char));
    memcpy(*buffer,b,_size);
    
    int _format = 0;
    if(_channels==1 && _bps==8){ _format = AL_FORMAT_MONO8; }
    else if(_channels==1 && _bps==16){ _format = AL_FORMAT_MONO16; }
    else if(_channels==2 && _bps==8){ _format = AL_FORMAT_STEREO8; }
    else if(_channels==2 && _bps==16){ _format = AL_FORMAT_STEREO16; }
    else{ PixieError("[%s]: unable to select audio format", __func__); }

    *format = _format;
    *size = _size;
    *rate = _sample_rate;

    PFree(buf);
}

/**********/
void PixieAudioPlaySound(unsigned char *buffer, int size, int format, int sample_rate)
{
    int status = 0;

    alGetSourcei(pixie->asrc_snd,AL_SOURCE_STATE,&status);
    
    if(status==AL_PLAYING) { return; }

    alDeleteBuffers(1, &pixie->asnd);

    alGenBuffers(1, &pixie->asnd);

    alBufferData(pixie->asnd, format, buffer, size, sample_rate);

    alSourcei(pixie->asrc_snd, AL_BUFFER, pixie->asnd);

    alSourcePlay(pixie->asrc_snd);
}

/**********/
void PixieAudioLoopSound(unsigned char *buffer, int size, int format, int sample_rate)
{
    int status = 0;

    alGetSourcei(pixie->asrc_snd,AL_SOURCE_STATE,&status);
    
    if(status==AL_PLAYING) { return; }

    alDeleteBuffers(1, &pixie->asnd);

    alGenBuffers(1, &pixie->asnd);

    alBufferData(pixie->asnd, format, buffer, size, sample_rate);

    alSourcei(pixie->asrc_snd, AL_BUFFER, pixie->asnd);

    alSourcei(pixie->asrc_snd, AL_LOOPING, 1);

    alSourcePlay(pixie->asrc_snd);
}

/**********/
void PixieAudioStopSound()
{
    int status = 0;

    alGetSourcei(pixie->asrc_snd,AL_SOURCE_STATE,&status);

    if(status==AL_PLAYING) 
    { 
        alSourceStop(pixie->asrc_snd); 
        alSourcei(pixie->asrc_snd, AL_LOOPING, 0);
    }
}

/**********/
void PixieAudioPlayTone(int tone)
{ 
    int status = 0;

    alGetSourcei(pixie->asrc_tone,AL_SOURCE_STATE,&status);
    
    if(status==AL_PLAYING) { return; }

    int _tone = tone % P_NUM_TONES;

    alSourcei(pixie->asrc_tone, AL_BUFFER, pixie->atones[_tone]);

    alSourcei(pixie->asrc_tone, AL_LOOPING, 1);

    alSourcePlay(pixie->asrc_tone);
}

/**********/
void PixieAudioStopTone()
{
    int status = 0;

    alGetSourcei(pixie->asrc_tone,AL_SOURCE_STATE,&status);

    if(status==AL_PLAYING) 
    { 
        alSourceStop(pixie->asrc_tone); 
        alSourcei(pixie->asrc_tone, AL_LOOPING, 0);
    }
}

/**********/
void PixieAudioSetVolume(float vol)
{
    alSourcef(pixie->asrc_snd, AL_GAIN, vol);
    alSourcef(pixie->asrc_tone, AL_GAIN, vol);
}

/********/
/* TIME */
/********/
int PixieTimeGet()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

    LARGE_INTEGER t = {0};
    LONGLONG elapsed = 0;

	QueryPerformanceCounter(&t);

    elapsed = t.QuadPart - pixie->timer.QuadPart;

    double val = (double)elapsed * 1000.0f / (double)pixie->freq.QuadPart;

    return (int)val;
}

/**********/
float PixieTimeGetDelta()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }
    
    int t = PixieTimeGet();

	float val = ( t - pixie->delta )/1000.0f;

	pixie->delta = t;

	return val;
}

/**********/
/* EVENTS */
/**********/
int PixieEventAvailable()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

    MSG msg = {0};

    if( PeekMessage(&msg, pixie->hwnd, 0, 0, PM_REMOVE ) )
    {
        DispatchMessage(&msg);
        return 1;
    }

    return 0;
}

/**********/
int PixieEventGetType()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

    return pixie->event;
}

/**********/
int PixieEventGetKey()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

    return pixie->key;
}

/**********/
int PixieEventGetAsciiKey()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

    return pixie->ascii_key;
}

/**********/
int PixieEventGetMouseX()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

	int val = pixie->mousex -(pixie->client_w-pixie->viewport_w)/2;
	
	val = PMap(val,0,pixie->viewport_w,0,pixie->width);

	val = PClamp(val, 0, pixie->width-1);

	return val;

    return pixie->mousex;
}

/**********/
int PixieEventGetMouseY()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

	int val = pixie->mousey -(pixie->client_h-pixie->viewport_h)/2;
	
	val = PMap(val,0,pixie->viewport_h,0,pixie->height);

	val = PClamp(val, 0, pixie->height-1);

	return val;

    return pixie->mousey;
}

/**********/
int PixieEventGetMouseButton()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

    return pixie->button;
}

/**********/
int PixieEventGetWheelDir()
{
    if(!pixie) { PixieError("[%s]: PIXIE not initialized", __func__); return 0; }

    return (pixie->wheel_dir>0) ? P_WHEEL_UP : P_WHEEL_DOWN;
}




# DATA TYPES

#### PImage

> puntatore a bytes contenenti i dati di un' immagine

#### PFont

> puntatore a bytes contenenti i dati di un font

#### PSound

> puntatore a bytes contenenti i dati di un suono

---
# CORE

#### void PixieDebug()

> Effettua la verifica di potenziali memory leaks
> Se rilevati ne verra' effettuato il dump in console al termine del programma
> Per questa funzionalità i file sorgente devono essere compilati con MSVC in configurazione debug  

<br>

#### void PixieInit(int w, int h, int screen)

> **w** : larghezza ( deve essere > 0 )  
> **h** : altezza ( deve essere > 0 )  
> **screen** : 0 finestra, 1 schermo intero  
>
> Inizializza Pixie creando una finestra contenente una superficie disegnabile di larghezza w ed altezza h
> Vengono allocati internamente alcuni blocchi di memoria, di conseguenza sarà necessario effettuare successivamente una chiamata a ***PixieClose*** prima di terminare il programma
> Si noti che verra' mantenuto il rapporto tra larghezza e altezza impostato, se necessario, lo spazio di riempimento verrà automaticamente colmato con bande verticali/orizzontali di colore nero equamente distribuite ai margini dell' area di disegno centrale

<br>

#### void PixieClose()
>
> Termina Pixie liberando i blocchi di memoria allocati internamente.
> Deve essere chiamata prima di terminare il programma.

<br>

#### void PixieToggleFullScreen()

> Passa dalla modalita' finestra a schermo intero e viceversa

<br>

#### void PixieToggleVsync()

> Abilita/disabilita il Vsync (default off)

---
# RENDER

#### void PixieRenderClear(int col)

> **col** : colore
>
> Pulisce la superficie disegnabile con il colore selezionato.

<br>

#### void PixieRenderDraw()

> Aggiorna la superficie disegnabile con tutti gli elementi che sono stati generati dalle precedenti chiamate a funzioni grafiche.

<br>

#### int PixieRenderGetWidth()

> **return** : larghezza
>
>  Restituisce la larghezza in pixels della superfcie disegnabile

<br>

#### int PixieRenderGetHeight()

> **return** : altezza
>
>  Restituisce l'altezza in pixels della superfcie disegnabile

---
# COLOR AND PALETTE

#### void PixieColorSet(int col)

<br>

#### void PixiePaletteSet(int pal)

---
# DRAWING

#### void PixieDrawPoint(int x, int y)

<br>

#### void PixieDrawLine(int x0, int y0, int x1, int y1)

<br>

#### void PixieDrawTri(int ax, int ay, int bx, int by, int cx, int cy)

<br>

#### void PixieDrawTriFill(int ax, int ay, int bx, int by, int cx, int cy)

<br>

#### void PixieDrawRect(int x, int y, int w, int h)

<br>

#### void PixieDrawRectFill(int x, int y, int w, int h)

<br>

#### void PixieDrawCircle(int x, int y, int r)
> **x** : coordinata x del centro  
> **y** : coordinata y del centro  
> **r** : raggio
>
> Disegna una circonferenza sulla superficie disegnabile
> Il colore della circonferenza sarà quello impostato tramite ***PixieColorSet***

<br>

#### void PixieDrawCircleFill(int x, int y, int r)

<br>

#### void PixieDrawImage(PImage buffer, int w, int h, int x, int y)

<br>

#### void PixieDrawImagePart(PImage buffer, int bw, int bh, int sx, int sy, int sw, int sh, int x, int y)

<br>

#### void PixieDrawImageMask(PImage buffer, int bw, int bh, PImage mask, int mw, int mh, int x, int y, int col)

<br>

#### void PixieDrawImagePartMask(PImage buffer, int bw, int bh, PImage mask, int mw, int mh, int sx, int sy, int sw, int sh, int x, int y, int col)

<br>

#### void PixieDrawChar(char c, int x, int y)

<br>

#### void PixieDrawString(char *s, int x, int y)

---
# IMAGES

#### void PixieImageLoad(char *file, PImage *buffer, int *w, int *h)

<br>

#### int PixieImageIntersect(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh, int *rx, int *ry, int *rw, int *rh)

<br>

#### int PixieImageOverlap(PImage a, int ax, int ay, int aw, int ah, PImage b, int bx, int by, int bw, int bh, int col)

---
# FONTS

#### void PixieFontLoad(char *file, PFont *buffer, int *size)

<br>

#### void PixieFontSet( PFont buffer, int size)

<br>

#### void PixieFontSetScale(int scale)

<br>

#### int PixieFontGetWidth()

<br>

#### int PixieFontGetHeight()

<br>

#### void PixieFontGetStringSize(char *s, int *w, int *h)

---
# AUDIO

#### void PixieAudioLoadSound(char *file,  PSound *buffer, int *size, int *format, int *rate)

<br>

#### void PixieAudioPlaySound(PSound buffer, int size, int format, int sample_rate)

<br>

#### void PixieAudioLoopSound(PSound buffer, int size, int format, int sample_rate)

<br>

#### void PixieAudioStopSound()

<br>

#### void PixieAudioPlayTone(int tone)

<br>

#### void PixieAudioStopTone()

<br>

#### void PixieAudioSetVolume(float vol)

---
# TIME

#### int PixieTimeGet()

<br>

#### float PixieTimeGetDelta()

---
# EVENTS

#### int PixieEventAvailable()

<br>

#### int PixieEventGetType()

<br>

#### int PixieEventGetKey()

<br>

#### int PixieEventGetAsciiKey()

<br>

#### int PixieEventGetMouseX()

<br>

#### int PixieEventGetMouseY()

<br>

#### int PixieEventGetMouseButton()

<br>

#### int PixieEventGetWheelDir()

---
# DEFINES

#### COLORS

> P_NUM_COLORS  
> P_COLOR_0  
> P_COLOR_1  
> P_COLOR_2  
> P_COLOR_3  

#### PALETTES

>P_NUM_PALETTES  
>P_PALETTE_MONO  
>P_PALETTE_0_L  
>P_PALETTE_0_H  
>P_PALETTE_1_L  
>P_PALETTE_1_H  
>P_PALETTE_2_L  
>P_PALETTE_2_H  

#### CHARSET

> P_CHARSET_BLANK  
> P_CHARSET_BLOCK  

#### TONES

> P_NUM_TONES  
> P_DO  
> P_RE  
> P_MI  
> P_FA  
> P_SO  
> P_LA  
> P_SI  

#### EVENTS

> P_NONE  
> P_QUIT  
> P_KEYPRESS  
> P_KEYRELEASE  
> P_MOUSEMOVE  
> P_MOUSEPRESS  
> P_MOUSERELEASE  
> P_MOUSEWHEEL  

#### MOUSE BUTTONS

> P_BUT_LEFT  
> P_BUT_MID  
> P_BUT_RIGHT  

#### MOUSE WHEEL

> P_WHEEL_UP  
> P_WHEEL_DOWN  

#### COMMON KEYS

> P_ESC  
> P_ENTER  
> P_CANCEL  
> P_BACKSPACE  
> P_SPACE  

#### NUMPAD KEYS

> P_PLUS  
> P_MINUS  

#### ARROW KEYS

> P_UP  
> P_DOWN  
> P_LEFT  
> P_RIGHT  

#### FUNCTION KEYS

> P_F1  
> P_F2  
> P_F3  
> P_F4  

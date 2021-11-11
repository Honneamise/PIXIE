REM gcc -Wall DEMO/SRC/formats.c PIXIE/*.c -o DEMO/formats.exe -lopengl32 -lgdi32 PIXIE/OPENAL/LIB/OpenAL32.lib -Wl,-subsystem,windows 
REM gcc -Wall DEMO/SRC/anim.c PIXIE/*.c -o DEMO/anim.exe -lopengl32 -lgdi32 PIXIE/OPENAL/LIB/OpenAL32.lib -Wl,-subsystem,windows 
REM gcc -Wall DEMO/SRC/fonts.c PIXIE/*.c -o DEMO/fonts.exe -lopengl32 -lgdi32 PIXIE/OPENAL/LIB/OpenAL32.lib -Wl,-subsystem,windows
REM gcc -Wall DEMO/SRC/particles.c PIXIE/*.c -o DEMO/particles.exe -lopengl32 -lgdi32 PIXIE/OPENAL/LIB/OpenAL32.lib -Wl,-subsystem,windows
REM gcc -Wall DEMO/SRC/tones.c PIXIE/*.c -o DEMO/tones.exe -lopengl32 -lgdi32 PIXIE/OPENAL/LIB/OpenAL32.lib -Wl,-subsystem,windows 
REM gcc -Wall DEMO/SRC/sound.c PIXIE/*.c -o DEMO/sound.exe -lopengl32 -lgdi32 PIXIE/OPENAL/LIB/OpenAL32.lib -Wl,-subsystem,windows 

gcc -Wall DEMO/SRC/gui.c PIXIE/*.c -o DEMO/gui.exe -lopengl32 -lgdi32 PIXIE/OPENAL/LIB/OpenAL32.lib -Wl,-subsystem,windows 

cl.exe /analyze /W3 /MTd /TC DEMO/SRC/formats.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:DEMO/formats.exe 
cl.exe /analyze /W3 /MTd /TC DEMO/SRC/anim.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:DEMO/anim.exe 
cl.exe /analyze /W3 /MTd /TC DEMO/SRC/fonts.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:DEMO/fonts.exe 
cl.exe /analyze /W3 /MTd /TC DEMO/SRC/particles.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:DEMO/particles.exe 
cl.exe /analyze /W3 /MTd /TC DEMO/SRC/tones.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:DEMO/tones.exe 
cl.exe /analyze /W3 /MTd /TC DEMO/SRC/sound.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:DEMO/sound.exe 

cl.exe /analyze /W3 /MTd /TC DEMO/SRC/test.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE /MACHINE:X64 /OUT:DEMO/test.exe 

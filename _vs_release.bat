cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/formats.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/formats.exe
cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/anim.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/anim.exe
cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/fonts.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/fonts.exe
cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/particles.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/particles.exe
cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/tones.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/tones.exe
cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/cube.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/cube.exe
cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/3dmodel.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/3dmodel.exe
cl.exe /O2 /Oi /W3 /MT /TC DEMO/SRC/sound.c PIXIE/*.c /link user32.lib gdi32.lib opengl32.lib PIXIE/OPENAL/LIB/OpenAL32.lib /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS /MACHINE:X64 /OUT:DEMO/sound.exe

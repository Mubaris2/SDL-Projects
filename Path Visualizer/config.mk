# config.mk - configuration for SDL2 paths

# Adjust these paths according to your system and SDL2 installation
SDL_CFLAGS := -IC:/msys64/ucrt64/include/SDL2
SDL_LDLIBS := -LC:/msys64/ucrt64/lib
LIBS = -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_ttf
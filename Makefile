# Taken from http://nuclear.mutantstargoat.com/articles/make/

CXX = g++

# Main list of files
ccsrc = $(wildcard src/*.cpp) \
        $(wildcard src/components/*.cpp) \
        $(wildcard src/ui/*.cpp) \
        $(wildcard src/actions/*.cpp) \
        $(wildcard src/prefabs/enemies/*.cpp) \
        $(wildcard src/prefabs/weapons/*.cpp) \
        $(wildcard src/prefabs/misc/*.cpp) \
        $(wildcard src/map/*.cpp) \
        $(wildcard src/map/generators/*.cpp)

obj = $(ccsrc:.cpp=.o)

# Files for sdl
sdl_samples_ccsrc = src/samples/sdl_overlay.cpp \
					$(wildcard src/sdl/*.cpp)

sdl_samples_obj = $(sdl_samples_ccsrc:.cpp=.o)

mc_ccsrc = src/samples/multiple_consoles.cpp
mc_obj = $(mc_ccsrc:.cpp=.o)

# TODO check this guy
LDFLAGS = -Isrc -L. -ltcod -lSDL2 -g -Wl,-rpath=.
LDFLAGS_SDL = -Isrc -L. -ltcod -lSDL2 -lSDL2_image -lSDL2_ttf -g -Wl,-rpath=.

titan: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(obj)

mc: $(mc_obj)
	$(CXX) -o $@ $^ $(LDFLAGS_SDL)

sdl_samples: $(sdl_samples_obj)
	$(CXX) -o $@ $^ $(LDFLAGS_SDL)

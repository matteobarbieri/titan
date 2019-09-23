# Taken from http://nuclear.mutantstargoat.com/articles/make/

CXX = g++

ccsrc = $(wildcard src/*.cpp) \
        $(wildcard src/components/*.cpp) \
        $(wildcard src/ui/*.cpp) \
        $(wildcard src/actions/*.cpp) \
        $(wildcard src/skills/*.cpp) \
        $(wildcard src/prefabs/enemies/*.cpp) \
        $(wildcard src/prefabs/weapons/*.cpp) \
        $(wildcard src/prefabs/misc/*.cpp) \
        $(wildcard src/map/*.cpp) \
        $(wildcard src/map/generators/*.cpp)

obj = $(ccsrc:.cpp=.o)

# TODO check this guy
LDFLAGS = -Isrc -L. -ltcod -g -Wl,-rpath=.
LDFLAGS_SDL = -Isrc -L. -ltcod -lSDL2 -lSDL2_image -lSDL2_ttf -g -Wl,-rpath=.

#titan: $(obj)
	#$(CXX) -o $@ $^ $(LDFLAGS)

titan: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS_SDL)

clean:
	rm -f $(obj)

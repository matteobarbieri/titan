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
sdl_samples_ccsrc = $(wildcard src/samples/*.cpp) \
					$(wildcard src/sdl/*.cpp)

sdl_samples_obj = $(sdl_samples_ccsrc:.cpp=.o)

# to delete
text := hello a b c

comma := ${null},${null}
space := ${null} ${null}
${space} := ${space} # ${ } is a space. Neat huh?


sobj = $(subst  $(space),$(comma),$(strip $(obj)))

# TODO check this guy
LDFLAGS = -Isrc -L. -ltcod -lSDL2 -g -Wl,-rpath=.

titan: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(obj)

wclean:
	rm -Force -ErrorAction Ignore $(sobj)

sdl_samples: $(sdl_samples_obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

testc:
	cmd /c del /q -ErrorAction Ignore .\\src\\Consoles.o

testd:
	powershell "Remove-Item -ErrorAction Ignore $(sobj)"

teste:
	powershell "echo aaa"

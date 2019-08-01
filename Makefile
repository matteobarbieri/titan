# Taken from http://nuclear.mutantstargoat.com/articles/make/

CXX = g++

ccsrc = $(wildcard src/*.cpp) \
        $(wildcard src/components/*.cpp) \
        $(wildcard src/ui/*.cpp) \
        $(wildcard src/actions/*.cpp) \
        $(wildcard src/prefabs/enemies/*.cpp) \
        $(wildcard src/map/*.cpp) \
        $(wildcard src/map/generators/*.cpp) 

obj = $(ccsrc:.cpp=.o)

# TODO check this guy
LDFLAGS = -Isrc -L. -ltcod -g -Wl,-rpath=.

titan: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

#clean: $(obj)
	#rm $^
clean:
	rm -f $(obj)

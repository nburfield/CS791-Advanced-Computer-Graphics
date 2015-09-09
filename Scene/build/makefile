#Check for OS Type
UNAME_S := $(shell uname -s)

#Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
	LIBS= -lSDL2 -lGLEW -lGL -lassimp `Magick++-config --cppflags --cxxflags --ldflags --libs`

#Mac
else
	CC=clang++
	LIBS= -lSDL2 -lgdal -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -stdlib=libc++ -lassimp `Magick++-config --cppflags --cxxflags --ldflags --libs`
endif

#Paths, used on Mac; will not hurt other compiling
PATHI= -I/usr/local/include/ 
INCLUDE= -I../include/
PATHL= -L/usr/local/lib/

#Compiler Flags
CXXFLAGS= -g -Wall -std=c++0x -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16

# .o Compilation
#O_FILES = main.o camera.o technique.o texture.o mesh.o water_technique.o terrain_technique.o window.o
O_FILES = main.o window.o camera.o mesh.o texture.o utility.o house.o water_technique.o technique.o
# main to be compiled
MAIN_FILE = main

all: clean $(O_FILES)
	$(CC) $(CXXFLAGS) -o ../bin/$(MAIN_FILE) $(O_FILES) $(LIBS) $(PATHI) $(PATHL)

main.o: ../src/$(MAIN_FILE).cpp
	$(CC) $(CXXFLAGS) -c ../src/$(MAIN_FILE).cpp -o main.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

camera.o: ../src/camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/camera.cpp -o camera.o $(INCLUDE)

technique.o: ../src/technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/technique.cpp -o technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

texture.o: ../src/texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/texture.cpp -o texture.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

mesh.o: ../src/mesh.cpp
	$(CC) $(CXXFLAGS) -c ../src/mesh.cpp -o mesh.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

water_technique.o: ../src/water_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/water_technique.cpp -o water_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

terrain_technique.o: ../src/terrain_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/terrain_technique.cpp -o terrain_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

window.o: ../src/window.cpp
	$(CC) $(CXXFLAGS) -c ../src/window.cpp -o window.o $(INCLUDE)

utility.o: ../src/utility.cpp
	$(CC) $(CXXFLAGS) -c ../src/utility.cpp -o utility.o $(INCLUDE)

house.o: ../src/house.cpp
	$(CC) $(CXXFLAGS) -c ../src/house.cpp -o house.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

clean:
	-@if rm main.o 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi

clean-all:
	rm *.o ../bin/$(MAIN_FILE)
 

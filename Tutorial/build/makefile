#Check for OS Type
UNAME_S := $(shell uname -s)

#Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
	LIBS= -lSDL2 -lGLEW -lGL -lassimp `Magick++-config --cppflags --cxxflags --ldflags --libs`

#Mac
else
	CC=clang++
	LIBS= -lSDL2 -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -stdlib=libc++ -lassimp `Magick++-config --cppflags --cxxflags --ldflags --libs`
endif

#Paths, used on Mac; will not hurt other compiling
PATHI= -I/usr/local/include/ -I../include/
PATHL= -L/usr/local/lib/

#Compiler Flags
CXXFLAGS= -g -Wall -std=c++0x -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16

all: main.o shader.o camera.o object.o mesh.o texture.o geom_pass.o gbuffer.o programHandle.o directional_pass.o light_pass.o point_pass.o
	$(CC) $(CXXFLAGS) -o ../bin/tutorial main.o shader.o camera.o object.o mesh.o texture.o geom_pass.o gbuffer.o programHandle.o directional_pass.o light_pass.o point_pass.o $(LIBS) $(PATHI) $(PATHL)

main.o: ../src/interpol.cpp
	$(CC) $(CXXFLAGS) -c ../src/interpol.cpp -o main.o $(PATHI) $(PATHL) $(LIBS)

shader.o: ../src/shader.cpp
	$(CC) $(CXXFLAGS) -c ../src/shader.cpp -o shader.o $(PATHI) $(PATHL) $(LIBS)

camera.o: ../src/camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/camera.cpp -o camera.o $(PATHI) $(PATHL) $(LIBS)

object.o: ../src/object.cpp
	$(CC) $(CXXFLAGS) -c ../src/object.cpp -o object.o $(PATHI) $(PATHL) $(LIBS)

mesh.o: ../src/mesh.cpp
	$(CC) $(CXXFLAGS) -c ../src/mesh.cpp -o mesh.o $(PATHI) $(PATHL) $(LIBS)

texture.o: ../src/texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/texture.cpp -o texture.o $(PATHI) $(PATHL) $(LIBS)

geom_pass.o: ../src/geom_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/geom_pass.cpp -o geom_pass.o $(PATHI) $(PATHL) $(LIBS)

gbuffer.o: ../src/gbuffer.cpp
	$(CC) $(CXXFLAGS) -c ../src/gbuffer.cpp -o gbuffer.o $(PATHI) $(PATHL) $(LIBS)

programHandle.o: ../src/programHandle.cpp
	$(CC) $(CXXFLAGS) -c ../src/programHandle.cpp -o programHandle.o $(PATHI) $(PATHL) $(LIBS)

directional_pass.o: ../src/directional_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/directional_pass.cpp -o directional_pass.o $(PATHI) $(PATHL) $(LIBS)

light_pass.o: ../src/light_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/light_pass.cpp -o light_pass.o $(PATHI) $(PATHL) $(LIBS)

point_pass.o: ../src/point_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/point_pass.cpp -o point_pass.o $(PATHI) $(PATHL) $(LIBS)

clean:
	rm *.o ../bin/tutorial

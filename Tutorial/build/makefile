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

all: main.o shader.o camera.o object.o mesh.o texture.o geom_pass.o gbuffer.o programHandle.o directional_pass.o light_pass.o point_pass.o cube_map_texture.o skybox.o skybox_tech.o billboard_technique.o particle_system.o random_texture.o ps_update.o opengl_data.o null_technique.o basic_mesh.o terrain.o gishandler.o buffer.o renderer.o shader2.o framerenderer.o
	$(CC) $(CXXFLAGS) -o ../bin/tutorial main.o shader.o camera.o object.o mesh.o texture.o geom_pass.o gbuffer.o programHandle.o directional_pass.o light_pass.o point_pass.o cube_map_texture.o skybox.o skybox_tech.o billboard_technique.o particle_system.o random_texture.o ps_update.o opengl_data.o null_technique.o basic_mesh.o terrain.o gishandler.o buffer.o renderer.o shader2.o framerenderer.o $(LIBS) $(PATHI) $(PATHL)

#main.o: ../src/interpol.cpp
#	$(CC) $(CXXFLAGS) -c ../src/interpol.cpp -o main.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

#main.o: ../src/particle.cpp
#	$(CC) $(CXXFLAGS) -c ../src/particle.cpp -o main.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

main.o: ../src/drycreek.cpp
	$(CC) $(CXXFLAGS) -c ../src/drycreek.cpp -o main.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

shader.o: ../src/shader.cpp
	$(CC) $(CXXFLAGS) -c ../src/shader.cpp -o shader.o $(INCLUDE) 

camera.o: ../src/camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/camera.cpp -o camera.o $(INCLUDE)

object.o: ../src/object.cpp
	$(CC) $(CXXFLAGS) -c ../src/object.cpp -o object.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

mesh.o: ../src/mesh.cpp
	$(CC) $(CXXFLAGS) -c ../src/mesh.cpp -o mesh.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

basic_mesh.o: ../src/mesh2.cpp
	$(CC) $(CXXFLAGS) -c ../src/mesh2.cpp -o basic_mesh.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

#basic_mesh.o: ../src/basic_mesh.cpp
#	$(CC) $(CXXFLAGS) -c ../src/basic_mesh.cpp -o basic_mesh.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

texture.o: ../src/texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/texture.cpp -o texture.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

geom_pass.o: ../src/geom_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/geom_pass.cpp -o geom_pass.o $(INCLUDE)

gbuffer.o: ../src/gbuffer.cpp
	$(CC) $(CXXFLAGS) -c ../src/gbuffer.cpp -o gbuffer.o $(INCLUDE)

programHandle.o: ../src/programHandle.cpp
	$(CC) $(CXXFLAGS) -c ../src/programHandle.cpp -o programHandle.o $(INCLUDE)

directional_pass.o: ../src/directional_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/directional_pass.cpp -o directional_pass.o $(INCLUDE)

light_pass.o: ../src/light_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/light_pass.cpp -o light_pass.o $(INCLUDE)

point_pass.o: ../src/point_pass.cpp
	$(CC) $(CXXFLAGS) -c ../src/point_pass.cpp -o point_pass.o $(INCLUDE)

cube_map_texture.o: ../src/cube_map_texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/cube_map_texture.cpp -o cube_map_texture.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

skybox.o: ../src/skybox.cpp
	$(CC) $(CXXFLAGS) -c ../src/skybox.cpp -o skybox.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

skybox_tech.o: ../src/skybox_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/skybox_tech.cpp -o skybox_tech.o $(INCLUDE)

billboard_technique.o: ../src/billboard_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/billboard_technique.cpp -o billboard_technique.o $(INCLUDE)

particle_system.o: ../src/particle_system.cpp
	$(CC) $(CXXFLAGS) -c ../src/particle_system.cpp -o particle_system.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

random_texture.o: ../src/random_texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/random_texture.cpp -o random_texture.o $(INCLUDE)

ps_update.o: ../src/ps_update.cpp
	$(CC) $(CXXFLAGS) -c ../src/ps_update.cpp -o ps_update.o $(INCLUDE)

opengl_data.o: ../src/opengl_data.cpp
	$(CC) $(CXXFLAGS) -c ../src/opengl_data.cpp -o opengl_data.o $(INCLUDE)

null_technique.o: ../src/null_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/null_technique.cpp -o null_technique.o $(INCLUDE)

terrain.o: ../src/terrain.cpp
	$(CC) $(CXXFLAGS) -c ../src/terrain.cpp -o terrain.o $(INCLUDE)

gishandler.o: ../src/gishandler.cpp
	$(CC) $(CXXFLAGS) -c ../src/gishandler.cpp -o gishandler.o $(INCLUDE)

buffer.o: ../src/buffer.cpp
	$(CC) $(CXXFLAGS) -c ../src/buffer.cpp -o buffer.o $(INCLUDE)

renderer.o: ../src/renderer.cpp
	$(CC) $(CXXFLAGS) -c ../src/renderer.cpp -o renderer.o $(INCLUDE)

shader2.o: ../src/shader2.cpp
	$(CC) $(CXXFLAGS) -c ../src/shader2.cpp -o shader2.o $(INCLUDE)

framerenderer.o: ../src/framerenderer.cpp
	$(CC) $(CXXFLAGS) -c ../src/framerenderer.cpp -o framerenderer.o $(INCLUDE)

clean:
	rm *.o ../bin/tutorial

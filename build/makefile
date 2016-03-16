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
O_FILES = skybox_tech.o skybox.o cube_map_texture.o ds_point_light_pass_tech.o null_technique.o ds_geom_pass_tech.o ds_dir_light_pass_tech.o ds_light_pass_tech.o gbuffer.o main.o billboard_list.o triangle.o window.o camera.o mesh.o texture.o utility.o house.o water_technique.o flag_technique.o technique.o terrain_technique.o particle.o random_texture.o ps_update_technique.o billboard_technique.o

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

flag_technique.o: ../src/flag_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/flag_technique.cpp -o flag_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

terrain_technique.o: ../src/terrain_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/terrain_technique.cpp -o terrain_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

window.o: ../src/window.cpp
	$(CC) $(CXXFLAGS) -c ../src/window.cpp -o window.o $(INCLUDE)

utility.o: ../src/utility.cpp
	$(CC) $(CXXFLAGS) -c ../src/utility.cpp -o utility.o $(INCLUDE)

gbuffer.o: ../src/gbuffer.cpp
	$(CC) $(CXXFLAGS) -c ../src/gbuffer.cpp -o gbuffer.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_light_pass_tech.o: ../src/ds_light_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_light_pass_tech.cpp -o ds_light_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_geom_pass_tech.o: ../src/ds_geom_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_geom_pass_tech.cpp -o ds_geom_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_dir_light_pass_tech.o: ../src/ds_dir_light_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_dir_light_pass_tech.cpp -o ds_dir_light_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

house.o: ../src/house.cpp
	$(CC) $(CXXFLAGS) -c ../src/house.cpp -o house.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

triangle.o: ../src/triangle.cpp
	$(CC) $(CXXFLAGS) -c ../src/triangle.cpp -o triangle.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

particle.o: ../src/particle.cpp
	$(CC) $(CXXFLAGS) -c ../src/particle.cpp -o particle.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

random_texture.o: ../src/random_texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/random_texture.cpp -o random_texture.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ps_update_technique.o: ../src/ps_update_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/ps_update_technique.cpp -o ps_update_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

billboard_technique.o: ../src/billboard_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/billboard_technique.cpp -o billboard_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

billboard_list.o: ../src/billboard_list.cpp
	$(CC) $(CXXFLAGS) -c ../src/billboard_list.cpp -o billboard_list.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_point_light_pass_tech.o: ../src/ds_point_light_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_point_light_pass_tech.cpp -o ds_point_light_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

null_technique.o: ../src/null_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/null_technique.cpp -o null_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

skybox_tech.o: ../src/skybox_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/skybox_tech.cpp -o skybox_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

skybox.o: ../src/skybox.cpp
	$(CC) $(CXXFLAGS) -c ../src/skybox.cpp -o skybox.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

cube_map_texture.o: ../src/cube_map_texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/cube_map_texture.cpp -o cube_map_texture.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

clean:
	-@if rm main.o 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi

clean-all:
	rm *.o ../bin/$(MAIN_FILE)
 

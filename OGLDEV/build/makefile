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

# main to be compiled
MAIN_FILE = shadow

all: clean ds_dir_light_pass_tech.o ds_geom_pass_tech.o ds_light_pass_tech.o ds_point_light_pass_tech.o gbuffer.o null_technique.o ogldev_camera.o ogldev_pipeline.o technique.o ogldev_basic_mesh.o ogldev_texture.o main.o ogldev_util.o ogldev_math_3d.o billboard_technique.o particle_system.o ps_update_technique.o random_texture.o mesh.o ogldev_basic_lighting.o billboard_list.o lighting_technique.o shadow_map_fbo.o shadow_map_technique.o water_technique.o terrain.o
	$(CC) $(CXXFLAGS) -o ../bin/$(MAIN_FILE) ds_dir_light_pass_tech.o ds_geom_pass_tech.o ds_light_pass_tech.o ds_point_light_pass_tech.o gbuffer.o null_technique.o ogldev_camera.o ogldev_pipeline.o technique.o ogldev_basic_mesh.o ogldev_texture.o main.o ogldev_util.o ogldev_math_3d.o billboard_technique.o particle_system.o ps_update_technique.o random_texture.o mesh.o ogldev_basic_lighting.o billboard_list.o lighting_technique.o shadow_map_fbo.o shadow_map_technique.o water_technique.o terrain.o $(LIBS) $(PATHI) $(PATHL)

main.o: ../src/$(MAIN_FILE).cpp
	$(CC) $(CXXFLAGS) -c ../src/$(MAIN_FILE).cpp -o main.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_dir_light_pass_tech.o: ../src/ds_dir_light_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_dir_light_pass_tech.cpp -o ds_dir_light_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_geom_pass_tech.o: ../src/ds_geom_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_geom_pass_tech.cpp -o ds_geom_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_light_pass_tech.o: ../src/ds_light_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_light_pass_tech.cpp -o ds_light_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ds_point_light_pass_tech.o: ../src/ds_point_light_pass_tech.cpp
	$(CC) $(CXXFLAGS) -c ../src/ds_point_light_pass_tech.cpp -o ds_point_light_pass_tech.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

gbuffer.o: ../src/gbuffer.cpp
	$(CC) $(CXXFLAGS) -c ../src/gbuffer.cpp -o gbuffer.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

null_technique.o: ../src/null_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/null_technique.cpp -o null_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ogldev_camera.o: ../src/ogldev_camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/ogldev_camera.cpp -o ogldev_camera.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ogldev_pipeline.o: ../src/ogldev_pipeline.cpp
	$(CC) $(CXXFLAGS) -c ../src/ogldev_pipeline.cpp -o ogldev_pipeline.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

technique.o: ../src/technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/technique.cpp -o technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ogldev_basic_mesh.o: ../src/ogldev_basic_mesh.cpp
	$(CC) $(CXXFLAGS) -c ../src/ogldev_basic_mesh.cpp -o ogldev_basic_mesh.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ogldev_texture.o: ../src/ogldev_texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/ogldev_texture.cpp -o ogldev_texture.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ogldev_util.o: ../src/ogldev_util.cpp
	$(CC) $(CXXFLAGS) -c ../src/ogldev_util.cpp -o ogldev_util.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ogldev_math_3d.o: ../src/ogldev_math_3d.cpp
	$(CC) $(CXXFLAGS) -c ../src/ogldev_math_3d.cpp -o ogldev_math_3d.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

billboard_technique.o: ../src/billboard_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/billboard_technique.cpp -o billboard_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

particle_system.o: ../src/particle_system.cpp
	$(CC) $(CXXFLAGS) -c ../src/particle_system.cpp -o particle_system.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ps_update_technique.o: ../src/ps_update_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/ps_update_technique.cpp -o ps_update_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

random_texture.o: ../src/random_texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/random_texture.cpp -o random_texture.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

mesh.o: ../src/mesh.cpp
	$(CC) $(CXXFLAGS) -c ../src/mesh.cpp -o mesh.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

ogldev_basic_lighting.o: ../src/ogldev_basic_lighting.cpp
	$(CC) $(CXXFLAGS) -c ../src/ogldev_basic_lighting.cpp -o ogldev_basic_lighting.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

billboard_list.o: ../src/billboard_list.cpp
	$(CC) $(CXXFLAGS) -c ../src/billboard_list.cpp -o billboard_list.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

lighting_technique.o: ../src/lighting_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/lighting_technique.cpp -o lighting_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

shadow_map_fbo.o: ../src/shadow_map_fbo.cpp
	$(CC) $(CXXFLAGS) -c ../src/shadow_map_fbo.cpp -o shadow_map_fbo.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

shadow_map_technique.o: ../src/shadow_map_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/shadow_map_technique.cpp -o shadow_map_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

water_technique.o: ../src/water_technique.cpp
	$(CC) $(CXXFLAGS) -c ../src/water_technique.cpp -o water_technique.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

terrain.o: ../src/terrain.cpp
	$(CC) $(CXXFLAGS) -c ../src/terrain.cpp -o terrain.o $(INCLUDE) `Magick++-config --cppflags --cxxflags --ldflags --libs`

clean:
	-@if rm main.o 2>/dev/null || true; then echo "Main Removed"; else echo "No Main"; fi

clean-all:
	rm *.o ../bin/$(MAIN_FILE)
 

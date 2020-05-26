LDLIBS = -lGL -lX11
CFLAGS = -Wall -g
CXXFLAGS = -Wall -g
OBJ = main.o mat.o mat_util.o mesh.o player.o target.o terrain.o vec.o gl.o gl_x11.o shader.o window_x11.o 
main: $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) $(LDLIBS) -o main
.PHONY: clean
clean:
	rm -f $(OBJ)
main.o: main.cpp gl.h window.h vec.h mesh.h player.h terrain.h target.h 
mat.o: mat.cpp vec.h mat.h 
mat_util.o: mat_util.cpp vec.h mat.h 
mesh.o: mesh.cpp vec.h mesh.h 
player.o: player.cpp vec.h mesh.h player.h constants.h 
target.o: target.cpp gl.h shader.h target.h 
terrain.o: terrain.cpp gl.h shader.h vec.h mat.h mat_util.h constants.h mesh.h player.h terrain.h 
vec.o: vec.cpp vec.h 
gl.o: gl.c gl.h 
gl_x11.o: gl_x11.c 
shader.o: shader.c gl.h 
window_x11.o: window_x11.c window.h 

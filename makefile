# Makefile for mygllib
# Y. Liow
#------------------------------------------------------------------------------
# Macros
#------------------------------------------------------------------------------
CXX       = g++
CXXFLAGS  = -g -Wall
LINK      = g++
LINKFLAGS = -lGL -lGLU -lglut -lGLEW
OBJS      = mygllib/config.o \
	    mygllib/Reshape.o \
	    mygllib/View.o \
	    mygllib/SingletonView.o \
	    Maze.o \
	    main.o
	    #GameObject.o 

main.exe: $(OBJS)
	$(LINK) $(OBJS) -o main.exe $(LINKFLAGS)

a asan: $(OBJS)
	$(LINK) $(OBJS) -g -fsanitize=address -o main.exe $(LINKFLAGS)
#------------------------------------------------------------------------------
# Object files
#------------------------------------------------------------------------------
# config.o: config.h config.cpp  
# 	$(CXX) $(CXXFLAGS) config.cpp -c -o config.o

# View.o: View.h View.cpp
# 	$(CXX) $(CXXFLAGS) View.cpp -c -o View.o

# SingletonView.o: SingletonView.h SingletonView.cpp
# 	$(CXX) $(CXXFLAGS) SingletonView.cpp -c -o SingletonView.o

# Reshape.o: Reshape.h Reshape.cpp
# 	$(CXX) $(CXXFLAGS) Reshape.cpp -c -o Reshape.o

# Keyboard.o: Keyboard.h Keyboard.cpp
# 	$(CXX) $(CXXFLAGS) Keyboard.cpp -c -o Keyboard.o

# SingletonView.o: SingletonView.h SingletonView.cpp
# 	$(CXX) $(CXXFLAGES) SingletonView.cpp -c -o SingletonView.o

main.o:		main.cpp mygllib/
# Light.h mygllib/gl3d.h mygllib/Viewport.h
	$(CXX) $(CXXFLAGS) main.cpp -c -o main.o -I mygllib/

#------------------------------------------------------------------------------
# Utilities
#------------------------------------------------------------------------------
r:
	./main.exe
clean:
	rm -f main.exe
c:
	rm -f main.exe

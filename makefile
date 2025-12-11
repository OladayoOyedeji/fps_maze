CXX       = g++
CXXFLAGS  = -g -Wall -Iinclude -Imygllib
LDFLAGS   = -lGL -lGLU -lglut
ASAN = -g -fsanitize=address

SRC       = $(wildcard src/*.cpp) $(wildcard mygllib/*.cpp)
OBJ       = $(SRC:.cpp=.o)
TARGET    = main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o $@; make clean

a asan:  $(OBJ)
	$(CXX) $(ASAN) $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o $@; make clean

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

r run:
	./main.exe

clean:
	rm -f $(OBJ) #$(TARGET)

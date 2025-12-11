CXX       = g++
CXXFLAGS  = -g -Wall -Iinclude -Imygllib
LDFLAGS   = -lGL -lGLU -lglut
ASAN = -fsanitize=address

SRC       = $(wildcard src/*.cpp) $(wildcard mygllib/*.cpp)
OBJ       = $(SRC:.cpp=.o)
TARGET    = main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o $@; make clean

a asan:  $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) -fsanitize=address $(LDFLAGS) -o $@; make clean

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

r run:
	./main.exe

clean:
	rm -f $(OBJ) #$(TARGET)

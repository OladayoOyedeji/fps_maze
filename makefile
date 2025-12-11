CXX       = g++
CXXFLAGS  = -g -Wall -Iinclude -Imygllib
LDFLAGS   = -lGL -lGLU -lglut

ASANFLAGS = -fsanitize=address -fno-omit-frame-pointer

SRC       = $(wildcard src/*.cpp) $(wildcard mygllib/*.cpp)
OBJ       = $(SRC:.cpp=.o)

TARGET    = main.exe
ASANTGT   = main-asan.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o $@

a asan: CXXFLAGS += $(ASANFLAGS)
a asan: LDFLAGS += $(ASANFLAGS)
a asan: $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o $(ASANTGT); make clean

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

r run:
	./main.exe

r-a run-asan:
	./main-asan.exe

clean:
	rm -f $(OBJ) #$(TARGET) $(ASANTGT)

m exe: main.cpp
	g++ *.cpp stack/*.h maze/*.cpp -Istack -Imaze
a asan: main.cpp
	g++ -g -fsanitize=address *.cpp stack/*.h maze/*.cpp -Istack -Imaze
r run:
	./a.out

c clear:
	-rm ./a.out
	find . -name '*.gch' -exec rm {} \;
 

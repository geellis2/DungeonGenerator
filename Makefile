OBJECTS = main.o beastiary.o 

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lpng -lc++abi -o

dungeonGen : $(OBJECTS) folder
	$(LD) $(OBJECTS) $(LDFLAGS) DG

main.o : main.cpp beastiary.h
	$(CXX) $(CXXFLAGS) main.cpp

beastiary.o : beastiary.cpp beastiary.h 
	$(CXX) $(CXXFLAGS) beastiary.cpp

folder :
	mkdir -p dungeons

clean : clearDungeons
	rm -f *.o DG *~ 

clearDungeons :
	rm -f -r dungeons
